// WaveManagerSubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Planet.h"
#include "EnemyScaleSetting.h"
#include "WaveManagerSubsystem.generated.h"

class AEnemyPawn;
class AEnemySpawnCelestial;
class USceneComponent;
enum class EEnemyType : uint8;

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	TSubclassOf<AEnemyPawn> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	int32 UnlockTime = 0;
};

UCLASS()
class PLANET_API UWaveManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& _collection) override;
	virtual void Deinitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& _world) override;

	void PlayWaveMode1(const UWorld& _world);
	void SpawnEnemyWave();
	void SpawnEnemiesAtRandomRow(const TSubclassOf<AEnemyPawn>& _enemyClass);

	void EnemyDied(const AEnemyPawn* _deadEnemy);

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	TEnumAsByte<EAutoReceiveInput::Type> TargetPlayer = SINGLEPLAYER_INDEX;
	
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	TSubclassOf<AEnemySpawnCelestial> EnemySpawnClass;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	TArray<FSpawnInfo> SpawnInfos;

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float EnemySpawnInterval	= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float DifficultyInterval	= 60.0f;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float StartMaxScore			= 4.0f;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float EndMaxScore			= 600.0f;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float InflectionPoint		= PLAYTIME / 2;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float Inclination			= 0.008f;

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	FEnemyScaleSetting ScaleSettings;

private:
	AEnemyPawn* spawnEnemy(const TSubclassOf<AEnemyPawn>& _enemyClass, const USceneComponent* _spawnPoint);
	USceneComponent* getRandomPointForTypeOrNull(EEnemyType _type) const;
	void updateMaxFieldScoreByGameTime();
	void updateSpawnableEnemyListByGameTime();
	
	APawn* cPlayerPawn;
	AEnemySpawnCelestial* cEnemySpawn;
	TArray<TSubclassOf<AEnemyPawn>> mRuntimeSpawnableList;

	FTimerHandle mWaveTimerHandle;
	FTimerHandle mDifficultyTimerHandle;
	FTimerHandle mListTimerHandle;

	FActorSpawnParameters mSpawnParams;
	float mMaxFieldScore;
	float mFieldScore;
	int32 spawnableIndex = 0;
};