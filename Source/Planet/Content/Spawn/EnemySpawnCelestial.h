// AEnemySpawnCelestial.h
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
	AEnemySpawnCelestial* Initialize(APawn* InTargetPawn);
	
	virtual void Tick(float DeltaTime) override;

	USceneComponent* GetRandomActiveSpawnPointOrNull();
	TArray<USceneComponent*> GetRandomRowSpawnPoints() const;
	
	void SetActiveSpawnPoint(USceneComponent* AimPoint, bool Active);
	void SetActiveAllSpawnPoints(bool Active);

	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<USceneComponent*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float SphereRadius = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 PointsPerRow = 24;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	int32 NumRows = 5;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float PitchInterval = 15.0f;
	UPROPERTY(VisibleAnywhere, Category = "Enemy Spawn")
	float YawInterval = 360.0f / PointsPerRow;
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
	float HalfFOV = 60.0f;

private:
	APawn* cTargetPawn;
	UCameraComponent* cTargetCamera;
	TArray<bool> mIsPointsActive;

	void updateSpawnPointActivation();

};
