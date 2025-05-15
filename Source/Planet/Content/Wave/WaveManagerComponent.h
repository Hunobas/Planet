// WaveManagerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "WaveConfigDataAsset.h"
#include "EnemyScaleSetting.h"
#include "WaveManagerComponent.generated.h"

class AEnemyPawn;
class AEnemySpawnCelestial;
class UWaveConfigDataAsset;
class UEnemyFireManagerComponent;
class UObjectPoolManagerComponent;
class USceneComponent;
enum class EEnemyType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UWaveManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWaveManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void PlayWaveMode1();
	void PlayWaveMode2();

	void SpawnEnemyWave();
	void SpawnEnemiesAtRandomRow(const TSubclassOf<AEnemyPawn>& _enemyClass);

	void EnemyDied(AEnemyPawn* _deadEnemy);

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	TEnumAsByte<EAutoReceiveInput::Type> TargetPlayer = SINGLEPLAYER_INDEX;
	
	UPROPERTY(EditAnywhere, Category = "Wave Manager", meta=(AllowAbstract="false"))
	TSubclassOf<AEnemySpawnCelestial> EnemySpawnClass;

	UPROPERTY(EditAnywhere, Category = "Wave Config")
	TArray<UWaveConfigDataAsset*> WaveConfigDatas;

	UPROPERTY(EditAnywhere, Category = "Wave Config")
	float CurrentMaxFieldScore = 10.0f;

	TArray<FSpawnInfo>	Config_SpawnInfos;
	float				Config_EnemySpawnInterval;
	float				Config_DifficultyInterval;
	float				Config_StartMaxScore;
	float				Config_EndMaxScore;
	float				Config_InflectionPoint;
	float				Config_Inclination;
	FRuntimeFloatCurve	Config_MaxFieldScoreCurve;
	FEnemyScaleSetting	Config_ScaleSettings;

private:
	AEnemyPawn* spawnEnemyOrNull(const TSubclassOf<AEnemyPawn>& _enemyClass, USceneComponent* _spawnPoint);
	USceneComponent* getRandomPointForTypeOrNull(const EEnemyType& _type) const;
	void updateMaxFieldScoreByGameTime();
	void updateSpawnableEnemyListByGameTime();
	void loadWaveConfigForCurrentLevel();
	
	AEnemySpawnCelestial* mEnemySpawn;
	UObjectPoolManagerComponent* mPool;
	UEnemyFireManagerComponent* mFireManager;
	TArray<TSubclassOf<AEnemyPawn>> mRuntimeSpawnableList;
	APawn* cTargetPlayer;

	FTimerHandle mWaveTimerHandle;
	FTimerHandle mDifficultyTimerHandle;
	FTimerHandle mListTimerHandle;

	float mCurrentFieldScore;
};
