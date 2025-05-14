// WaveManagerComponent.cpp
#include "WaveManagerComponent.h"

#include "Kismet/GameplayStatics.h"

#include "../Planet.h"
#include "EnemyFireManagerComponent.h"
#include "WaveConfigDataAsset.h"
#include "EnemyPawn.h"
#include "EnemySpawnCelestial.h"
#include "ObjectPoolManagerComponent.h"

UWaveManagerComponent::UWaveManagerComponent(): Config_EnemySpawnInterval(5.0f), Config_DifficultyInterval(5.0f),
                                                Config_StartMaxScore(0),
                                                Config_EndMaxScore(0),
                                                Config_InflectionPoint(PLAYTIME / 2), Config_Inclination(0),
                                                mEnemySpawn(nullptr),
                                                mPool(nullptr),
                                                mFireManager(nullptr),
                                                mCurrentFieldScore(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UWaveManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	loadWaveConfigForCurrentLevel();

	APawn* playerPawn = GetTargetPlayerPawn(TargetPlayer, this);
	check(playerPawn);
	const FTransform playerTx( playerPawn->GetActorRotation(), playerPawn->GetActorLocation() );
	
	check(EnemySpawnClass);
	mEnemySpawn = GetWorld()->SpawnActor<AEnemySpawnCelestial>(EnemySpawnClass, playerTx)->Initialize(playerPawn);
	
	if (!TryGetFirstComponentWithTag(GetOwner(), OBJECT_POOL_TAG, mPool))
	{
		checkf(false, TEXT("[WaveManager] 오브젝트 풀 컴포넌트 불러오기 실패."));
	}
	if (!TryGetFirstComponentWithTag(GetOwner(), FIRE_MANAGER_TAG, mFireManager))
	{
		checkf(false, TEXT("[WaveManager] 적 Fire Manager 컴포넌트 불러오기 실패."));
	}
	mFireManager->Initialize(mEnemySpawn);

	// PlayWaveMode1();
	PlayWaveMode2();
}

void UWaveManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(mWaveTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mDifficultyTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mListTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void UWaveManagerComponent::PlayWaveMode1()
{
	updateSpawnableEnemyListByGameTime();
	SpawnEnemyWave();
	
	GetWorld()->GetTimerManager().SetTimer(
		mWaveTimerHandle,
		this,
		&UWaveManagerComponent::SpawnEnemyWave,
		Config_EnemySpawnInterval,
		true
	);

	// GetWorld()->GetTimerManager().SetTimer(
	// 	mDifficultyTimerHandle,
	// 	this,
	// 	&UWaveManagerComponent::updateMaxFieldScoreByGameTime,
	// 	Config_DifficultyInterval,
	// 	true
	// );

	GetWorld()->GetTimerManager().SetTimer(
		mListTimerHandle,
		this,
		&UWaveManagerComponent::updateSpawnableEnemyListByGameTime,
		Config_DifficultyInterval,
		true
	);
}

void UWaveManagerComponent::PlayWaveMode2()
{
	updateSpawnableEnemyListByGameTime();

	for (USceneComponent* spawnPoint : mEnemySpawn->GetNthRowSpawnPoints(3))
	{
		spawnEnemyOrNull(mRuntimeSpawnableList[0], spawnPoint);
	}
}

void UWaveManagerComponent::SpawnEnemyWave()
{
	check(mEnemySpawn);
	
	if (mRuntimeSpawnableList.IsEmpty() || mCurrentFieldScore > CurrentMaxFieldScore)
		return;

	while (mCurrentFieldScore < CurrentMaxFieldScore)
	{
		TSubclassOf<AEnemyPawn> enemyClass = mRuntimeSpawnableList[FMath::RandRange(0, mRuntimeSpawnableList.Num() - 1)];
		
		EEnemyType enemyType = enemyClass.GetDefaultObject()->EnemyType;
		USceneComponent* spawnPoint = getRandomPointForTypeOrNull(enemyType);

		if (spawnPoint == nullptr)
			continue;
		
		AEnemyPawn* spawnedEnemy = spawnEnemyOrNull(enemyClass, spawnPoint);
		if (spawnedEnemy == nullptr)
			return;
	}
}

void UWaveManagerComponent::SpawnEnemiesAtRandomRow(const TSubclassOf<AEnemyPawn>& _enemyClass)
{
	check(mEnemySpawn);
	check(_enemyClass);

	TArray<USceneComponent*> spawnPoints = mEnemySpawn->GetRandomRowSpawnPoints();

	for (USceneComponent* spawnPoint : spawnPoints)
	{
		AEnemyPawn* spawnedEnemy = spawnEnemyOrNull(_enemyClass, spawnPoint);
		if (spawnedEnemy == nullptr)
			return;
	}
}

void UWaveManagerComponent::EnemyDied(AEnemyPawn* _deadEnemy)
{
	mCurrentFieldScore -= _deadEnemy->RuntimeSettings.FieldScore;
	mPool->Release(_deadEnemy);

	mFireManager->RemoveEnemy(_deadEnemy);
}

AEnemyPawn* UWaveManagerComponent::spawnEnemyOrNull(const TSubclassOf<AEnemyPawn>& _enemyClass, USceneComponent* _spawnPoint)
{
	const FTransform spawnTx( _spawnPoint->GetComponentRotation(), _spawnPoint->GetComponentLocation() );

	AEnemyPawn* spawnedEnemy = mPool->AcquireOrNull(_enemyClass, spawnTx);

	if (spawnedEnemy)
	{
		spawnedEnemy->ResetToDefaultSettings(Config_ScaleSettings);
		mCurrentFieldScore += spawnedEnemy->RuntimeSettings.FieldScore;
		mFireManager->AddEnemy(spawnedEnemy, _spawnPoint);
	}
	return spawnedEnemy;
}

USceneComponent* UWaveManagerComponent::getRandomPointForTypeOrNull(const EEnemyType& _type) const
{
	switch (_type)
	{
		case EEnemyType::Ranged: return mEnemySpawn->GetRandomActiveSpawnPointOrNull();
		default:				 return mEnemySpawn->GetRandomSpawnPoint();
	}
}

void UWaveManagerComponent::updateMaxFieldScoreByGameTime()
{
	const float elapsedTime = UGameplayStatics::GetTimeSeconds(this);

	if (Config_MaxFieldScoreCurve.GetRichCurveConst())
	{
		CurrentMaxFieldScore = Config_MaxFieldScoreCurve.GetRichCurveConst()->Eval(elapsedTime);
	}
	else
	{
		CurrentMaxFieldScore = CalulateDefaultSigmoid(Config_StartMaxScore, Config_EndMaxScore, Config_Inclination, Config_InflectionPoint, elapsedTime);
	}
}

void UWaveManagerComponent::updateSpawnableEnemyListByGameTime()
{
	const float elapsedTime = UGameplayStatics::GetTimeSeconds(this);

	for (const auto& [enemyClass, unlockTime, lockTime] : Config_SpawnInfos)
	{
		if (elapsedTime >= lockTime)
		{
			mRuntimeSpawnableList.RemoveSingle(enemyClass);
		}
		else if (elapsedTime >= unlockTime)
		{
			mRuntimeSpawnableList.AddUnique(enemyClass);
		}
	}
}

void UWaveManagerComponent::loadWaveConfigForCurrentLevel()
{
	int32 levelIndex = ParseLevelIndex(GetWorld());
	checkf(levelIndex > 0 && levelIndex <= WaveConfigDatas.Num(), TEXT("[WaveManager]: 레벨 인덱스 파싱 실패: %d"), levelIndex);

	UWaveConfigDataAsset* waveConfig = WaveConfigDatas[levelIndex - 1];

	Config_SpawnInfos			= waveConfig->SpawnInfos;
	Config_EnemySpawnInterval	= waveConfig->EnemySpawnInterval;
	Config_DifficultyInterval	= waveConfig->DifficultyInterval;
	Config_StartMaxScore		= waveConfig->StartMaxScore;
	Config_EndMaxScore			= waveConfig->EndMaxScore;
	Config_InflectionPoint		= waveConfig->InflectionPoint;
	Config_Inclination			= waveConfig->Inclination;
	Config_MaxFieldScoreCurve	= waveConfig->MaxFieldScoreCurve;
	Config_ScaleSettings		= waveConfig->ScaleSettings;
}

