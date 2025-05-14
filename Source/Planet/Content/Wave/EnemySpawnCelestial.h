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
	
	// Pawn::BeginPlay 에서 호출
	AEnemySpawnCelestial* Initialize(APawn* _targetPawn);

	USceneComponent* GetRandomSpawnPoint();
	USceneComponent* GetRandomActiveSpawnPointOrNull();
	TArray<USceneComponent*> GetRandomRowSpawnPoints() const;
	TArray<USceneComponent*> GetNthRowSpawnPoints(const int& n) const;
	
	void SetOccupiedSpawnPoint(USceneComponent* _spawnPoint, bool _active);
	void SetActiveAllSpawnPoints(bool _active);

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<USceneComponent*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float EnemySpawnRadius	= 2000.f;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 PointsPerRow	= 24;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 NumRows		= 5;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float PitchInterval	= 15.0f;
	UPROPERTY(VisibleAnywhere, Category = "Enemy Spawn")
	float YawInterval	= 360.0f / PointsPerRow;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float HalfFOV		= 60.0f;

	UCameraComponent* PlayerCamera;
	
private:
	APawn* cPlayerPawn;
	
	TArray<bool> mIsPointsActive;
	TArray<bool> mIsPointsOccupied;

	void composeSpawnPointScenes();
	void updatePlayerFacingSpawnPoint();
};
