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

	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="Fire Manager")
	void FireRandomFacingEnemy();
	
	void AddEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint);
	void RemoveEnemy(AEnemyPawn* _deadEnemy);
	void EnqueueFireComponent(UFiringComponent* _fireComponent);
	void DequeueFireComponent(const UFiringComponent* _firedComponent);

private:
	TArray<AEnemyPawn*> getPlayerFacingEnemies();
	
	AEnemySpawnCelestial* mEnemySpawn;
	TArray<AEnemyPawn*> mRangedEnemies;

	FCriticalSection mQueueCriticalSection;
	TQueue<UFiringComponent*> mFireComponentQueue;
	TSet<UFiringComponent*> mActiveFireComponents;
};