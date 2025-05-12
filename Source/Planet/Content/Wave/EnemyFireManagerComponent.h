// EnemyFireManagerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFireManagerComponent.generated.h"

class AEnemyPawn;
class AEnemySpawnCelestial;
class UFiringComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UEnemyFireManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFireManagerComponent();
	UEnemyFireManagerComponent* Initialize(AEnemySpawnCelestial* _enemySpawn);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Fire Manager")
	UFiringComponent* FireRandomFacingEnemy();
	
	void AddEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint);
	void RemoveEnemy(AEnemyPawn* _deadEnemy);

private:
	TArray<AEnemyPawn*> getPlayerFacingEnemies();
	
	AEnemySpawnCelestial* mEnemySpawn;
	TArray<AEnemyPawn*> mRangedEnemies;
};
