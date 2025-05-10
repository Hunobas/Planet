// WaveManagerSubsystem.h
#include "WaveManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyPawn.h"
#include "EnemySpawnCelestial.h"

void UWaveManagerSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{
	Super::Initialize(_collection);

	cPlayerPawn = GetTargetPlayerPawn(TargetPlayer, this);
}

void UWaveManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UWaveManagerSubsystem::OnWorldBeginPlay(UWorld& _world)
{
	Super::OnWorldBeginPlay(_world);
	
	check(cPlayerPawn);
	const FVector playerLocation	= cPlayerPawn->GetActorLocation();
	const FRotator playerRotation	= cPlayerPawn->GetActorRotation();
	
	check(EnemySpawnClass);
	cEnemySpawn = GetWorld()->SpawnActor<AEnemySpawnCelestial>(EnemySpawnClass, playerLocation, playerRotation)->Initialize(cPlayerPawn);

	mSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	PlayWaveMode1(_world);
}

void UWaveManagerSubsystem::PlayWaveMode1(const UWorld& _world)
{
	_world.GetTimerManager().SetTimer(
		mWaveTimerHandle,
		this,
		&UWaveManagerSubsystem::SpawnEnemyWave,
		EnemySpawnInterval,
		true
	);

	_world.GetTimerManager().SetTimer(
		mDifficultyTimerHandle,
		this,
		&UWaveManagerSubsystem::updateMaxFieldScoreByGameTime,
		DifficultyInterval,
		true
	);

	_world.GetTimerManager().SetTimer(
		mListTimerHandle,
		this,
		&UWaveManagerSubsystem::updateSpawnableEnemyListByGameTime,
		EnemySpawnInterval,
		true
	);
}

void UWaveManagerSubsystem::SpawnEnemyWave()
{
	check(cEnemySpawn);
	
	if (mRuntimeSpawnableList.IsEmpty() || mFieldScore > mMaxFieldScore)
		return;

	while (mFieldScore < mMaxFieldScore)
	{
		TSubclassOf<AEnemyPawn> enemyClass = mRuntimeSpawnableList[FMath::RandRange(0, mRuntimeSpawnableList.Num() - 1)];
		
		EEnemyType enemyType = enemyClass.GetDefaultObject()->EnemyType;
		USceneComponent* spawnPoint = getRandomPointForTypeOrNull(enemyType);

		if (spawnPoint == nullptr)
			continue;
		
		AEnemyPawn* spawnedEnemy = spawnEnemy(enemyClass, spawnPoint);

		if (enemyType == EEnemyType::Ranged)
		{
			spawnedEnemy->AttachToComponent(spawnPoint, FAttachmentTransformRules::KeepWorldTransform);
			cEnemySpawn->SetActiveSpawnPoint(spawnPoint, false);
		}
	}
}

void UWaveManagerSubsystem::SpawnEnemiesAtRandomRow(const TSubclassOf<AEnemyPawn>& _enemyClass)
{
	check(cEnemySpawn);
	check(_enemyClass);

	TArray<USceneComponent*> spawnPoints = cEnemySpawn->GetRandomRowSpawnPoints();

	for (USceneComponent* spawnPoint : spawnPoints)
	{
		spawnEnemy(_enemyClass, spawnPoint);
	}
}

void UWaveManagerSubsystem::EnemyDied(const AEnemyPawn* _deadEnemy)
{
	mFieldScore -= _deadEnemy->RuntimeSettings.FieldScore;
}

AEnemyPawn* UWaveManagerSubsystem::spawnEnemy(const TSubclassOf<AEnemyPawn>& _enemyClass, const USceneComponent* _spawnPoint)
{
	const FVector pointLocation		= _spawnPoint->GetComponentLocation();
	const FRotator pointRotation	= _spawnPoint->GetComponentRotation();

	AEnemyPawn* spawnedEnemy = GetWorld()->SpawnActor<AEnemyPawn>(_enemyClass, pointLocation, pointRotation, mSpawnParams);
	
	spawnedEnemy->ResetToDefaultSettings(ScaleSettings);
	mFieldScore += spawnedEnemy->RuntimeSettings.FieldScore;

	return spawnedEnemy;
}

USceneComponent* UWaveManagerSubsystem::getRandomPointForTypeOrNull(EEnemyType _type) const
{
	switch (_type)
	{
		case EEnemyType::Ranged: return cEnemySpawn->GetRandomActiveSpawnPointOrNull();
		default:                 return cEnemySpawn->GetRandomSpawnPoint();
	}
}

void UWaveManagerSubsystem::updateMaxFieldScoreByGameTime()
{
	const float elaspedTime = UGameplayStatics::GetTimeSeconds(this);
	mMaxFieldScore = StartMaxScore + (EndMaxScore - StartMaxScore) / (1 + FMath::Exp(Inclination * (InflectionPoint - elaspedTime)));
}

void UWaveManagerSubsystem::updateSpawnableEnemyListByGameTime()
{
	const float elapsedTime = UGameplayStatics::GetTimeSeconds(this);

	for (const FSpawnInfo& info : SpawnInfos)
	{
		if (elapsedTime > info.UnlockTime)
		{
			mRuntimeSpawnableList.AddUnique(info.EnemyClass);
		}
	}
}