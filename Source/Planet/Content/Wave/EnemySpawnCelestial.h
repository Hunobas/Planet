// EnemySpawnCelestial.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnCelestial.generated.h"

class USceneComponent;
class UCameraComponent;

UCLASS()
class PLANET_API AEnemySpawnCelestial : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnCelestial();
	
	// UWaveManagerComponent::BeginPlay 에서 호출
	AEnemySpawnCelestial* Initialize(APawn* _targetPawn);

	USceneComponent* GetRandomMeleeSpawnPoint();
	USceneComponent* GetRandomRangedSpawnPointOrNull();
	TArray<USceneComponent*> GetNthRowSpawnPoints(const int32& n) const;
	TArray<USceneComponent*> GetRandomRowSpawnPoints() const;
	
	void SetOccupiedSpawnPoint(USceneComponent* _spawnPoint, bool _active);

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<USceneComponent*> MeleeSpawnPoints;
	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<USceneComponent*> RangedSpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float MeleeSpawnRadius	= 5000.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float RangedSpawnRadius	= 1500.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 PointsPerCircle	= 24;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 NumCircles		= 3;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float RadiusInterval	= 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Enemy Spawn")
	float YawInterval		= 360.0f / PointsPerCircle;
	
private:
	APawn* cPlayerPawn;
	
	TArray<bool> mIsPointsOccupied;

	void composeSpawnPointScenes();
};
