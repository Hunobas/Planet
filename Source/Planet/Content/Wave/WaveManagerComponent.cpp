// WaveManagerComponent.cpp
#include "WaveManagerComponent.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyFireManagerComponent.h"
#include "WaveConfigDataAsset.h"
#include "EnemyPawn.h"
#include "EnemySpawnCelestial.h"
#include "ObjectPoolManagerComponent.h"

UWaveManagerComponent::UWaveManagerComponent(): Config_EnemySpawnInterval(5.0f), Config_DifficultyInterval(5.0f),
                                                mEnemySpawn(nullptr),
                                                mPool(nullptr),
                                                mFireManager(nullptr), cTargetPlayer(nullptr),
                                                mCurrentLevelConfig(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UWaveManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (loadWaveConfigForCurrentLevel() == false)
		return;

	cTargetPlayer = GetTargetPlayerPawn(TargetPlayer, this);
	check(cTargetPlayer);
	const FTransform playerTx( cTargetPlayer->GetActorRotation(), cTargetPlayer->GetActorLocation() );
	
	check(EnemySpawnClass);
	mEnemySpawn = GetWorld()->SpawnActor<AEnemySpawnCelestial>(EnemySpawnClass, playerTx)->Initialize(cTargetPlayer);

	mPool = GetObjectPoolManager(this);
	if (!TryGetFirstComponentWithTag(GetOwner(), FIRE_MANAGER_TAG, mFireManager))
	{
		checkf(false, TEXT("[WaveManager] 적 Fire Manager 컴포넌트 불러오기 실패."));
	}
	mFireManager->Initialize(mEnemySpawn);

	CurrentFieldScore = 0;
	
	PlayWaveMode1();
	// PlayWaveMode2();
}

void UWaveManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(mWaveTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mDifficultyTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mEnemyScaleTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mListTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(mSpawnIntervalTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void UWaveManagerComponent::PlayWaveMode1()
{
	GetWorld()->GetTimerManager().SetTimer(
		mDifficultyTimerHandle,
		this,
		&UWaveManagerComponent::updateMaxFieldScoreByGameTime,
		Config_DifficultyInterval,
		true,
		0.0f
	);

	GetWorld()->GetTimerManager().SetTimer(
		mEnemyScaleTimerHandle,
		this,
		&UWaveManagerComponent::updateEnemyScaleByGameTime,
		Config_DifficultyInterval,
		true,
		0.0f
	);

	GetWorld()->GetTimerManager().SetTimer(
		mListTimerHandle,
		this,
		&UWaveManagerComponent::updateSpawnableEnemyListByGameTime,
		Config_DifficultyInterval,
		true,
		0.0f
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		mWaveTimerHandle,
		this,
		&UWaveManagerComponent::SpawnEnemyWave,
		Config_EnemySpawnInterval,
		true
	);
}

void UWaveManagerComponent::PlayWaveMode2()
{
	updateSpawnableEnemyListByGameTime();

	for (USceneComponent* spawnPoint : mEnemySpawn->GetNthRowSpawnPoints(2))
	{
		spawnEnemyOrNull(mRuntimeSpawnableList[0], spawnPoint);
	}
}

void UWaveManagerComponent::SpawnEnemyWave()
{
	if (mRuntimeSpawnableList.IsEmpty() || CurrentFieldScore > CurrentMaxFieldScore)
	{
		GetWorld()->GetTimerManager().ClearTimer(mSpawnIntervalTimerHandle);
		return;
	}
        
	if (GetWorld()->GetTimerManager().IsTimerActive(mSpawnIntervalTimerHandle))
		return;

	GetWorld()->GetTimerManager().SetTimer(
		mSpawnIntervalTimerHandle,
		this,
		&UWaveManagerComponent::spawnSingleEnemy,
		SpawnInterval,
		true,
		0.0f
	);
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
	CurrentFieldScore -= _deadEnemy->RuntimeSettings.FieldScore;
	mPool->Release(_deadEnemy);

	mFireManager->RemoveEnemy(_deadEnemy);
}

void UWaveManagerComponent::spawnSingleEnemy()
{
	if (mRuntimeSpawnableList.IsEmpty() || CurrentFieldScore > CurrentMaxFieldScore)
	{
		GetWorld()->GetTimerManager().ClearTimer(mSpawnIntervalTimerHandle);
		return;
	}
	
	TSubclassOf<AEnemyPawn> enemyClass = mRuntimeSpawnableList[FMath::RandRange(0, mRuntimeSpawnableList.Num() - 1)];
	EEnemyType enemyType = enemyClass.GetDefaultObject()->EnemyType;
	USceneComponent* spawnPoint = getRandomPointForTypeOrNull(enemyType);
    
	if (spawnPoint == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UWaveManagerComponent] 유효한 스폰 포인트 찾지 못함."));
		GetWorld()->GetTimerManager().ClearTimer(mSpawnIntervalTimerHandle);
	}
	if (spawnEnemyOrNull(enemyClass, spawnPoint) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UWaveManagerComponent] 오브젝트 풀에서 액터 찾지 못함: %s"), *enemyClass->GetName());
		GetWorld()->GetTimerManager().ClearTimer(mSpawnIntervalTimerHandle);
	}
}

AEnemyPawn* UWaveManagerComponent::spawnEnemyOrNull(const TSubclassOf<AEnemyPawn>& _enemyClass, USceneComponent* _spawnPoint)
{
	const FTransform spawnTx( _spawnPoint->GetComponentRotation(), _spawnPoint->GetComponentLocation() );

	AEnemyPawn* spawnedEnemy = mPool->AcquireOrNull(_enemyClass, spawnTx);

	if (spawnedEnemy)
	{
		spawnedEnemy->SetOwner(GetOwner());
		spawnedEnemy->Initialize(this, cTargetPlayer);
		CurrentFieldScore += spawnedEnemy->RuntimeSettings.FieldScore;
		mFireManager->AddEnemy(spawnedEnemy, _spawnPoint);
	}
	return spawnedEnemy;
}

USceneComponent* UWaveManagerComponent::getRandomPointForTypeOrNull(const EEnemyType& _type) const
{
	switch (_type)
	{
		case EEnemyType::Ranged: return mEnemySpawn->GetRandomRangedSpawnPointOrNull();
		default:				 return mEnemySpawn->GetRandomMeleeSpawnPoint();
	}
}

void UWaveManagerComponent::updateMaxFieldScoreByGameTime()
{
	const float elapsedTime = UGameplayStatics::GetTimeSeconds(this);

	if (Config_MaxFieldScoreCurve.GetRichCurveConst())
	{
		CurrentMaxFieldScore = Config_MaxFieldScoreCurve.GetRichCurveConst()->Eval(elapsedTime);
	}
}

void UWaveManagerComponent::updateEnemyScaleByGameTime()
{
	check(mCurrentLevelConfig);
	const float elapsedTime = UGameplayStatics::GetTimeSeconds(this);
    
	if (mCurrentLevelConfig->HPScaleCurve.GetRichCurveConst())
	{
		Config_ScaleSettings.HPScale = mCurrentLevelConfig->HPScaleCurve.GetRichCurveConst()->Eval(elapsedTime);
	}
	if (mCurrentLevelConfig->DamageScaleCurve.GetRichCurveConst())
	{
		Config_ScaleSettings.DamageScale = mCurrentLevelConfig->DamageScaleCurve.GetRichCurveConst()->Eval(elapsedTime);
	}
	if (mCurrentLevelConfig->SpeedScaleCurve.GetRichCurveConst())
	{
		Config_ScaleSettings.SpeedScale = mCurrentLevelConfig->SpeedScaleCurve.GetRichCurveConst()->Eval(elapsedTime);
	}
	if (mCurrentLevelConfig->XPDropScaleCurve.GetRichCurveConst())
	{
		Config_ScaleSettings.XPDropScale = mCurrentLevelConfig->XPDropScaleCurve.GetRichCurveConst()->Eval(elapsedTime);
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

bool UWaveManagerComponent::loadWaveConfigForCurrentLevel()
{
	int32 levelIndex = ParseLevelIndex(GetWorld());

	if (levelIndex <= 0 || levelIndex > WaveConfigDatas.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("[WaveManager]: 레벨 인덱스 파싱 실패: %d"), levelIndex);
		return false;
	}

	mCurrentLevelConfig = WaveConfigDatas[levelIndex - 1];

	Config_SpawnInfos			= mCurrentLevelConfig->SpawnInfos;
	Config_EnemySpawnInterval	= mCurrentLevelConfig->EnemySpawnInterval;
	Config_DifficultyInterval	= mCurrentLevelConfig->DifficultyInterval;
	Config_MaxFieldScoreCurve	= mCurrentLevelConfig->MaxFieldScoreCurve;
	Config_ScaleSettings		= mCurrentLevelConfig->ScaleSettings;

	return true;
}

