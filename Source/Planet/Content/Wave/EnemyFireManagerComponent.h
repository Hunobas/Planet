// EnemyFireManagerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
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
	void FireOnBeat();
	UFUNCTION(BlueprintCallable, Category="Fire Manager")
	void FireOnBeatAt(const int32 _index);
	
	void RegisterRangedEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint);
	void UnregisterRangedEnemy(AEnemyPawn* _deadEnemy);
	
	void EnqueueFireComponent(UFiringComponent* _fireComponent);
	void DequeueFireComponent(const UFiringComponent* _firedComponent);

private:
	UFiringComponent* getNextPointOrNull();
	UFiringComponent* getPointAtIndexOrNull(const int32 _index) const;
	static bool isEnemyValidForFiring(const AEnemyPawn* _enemy);

	UPROPERTY()
	AEnemySpawnCelestial* mEnemySpawn;
	UPROPERTY()
	TArray<AEnemyPawn*> mRangedEnemies;

	FCriticalSection mQueueCriticalSection;
	TQueue<UFiringComponent*> mFireComponentQueue;
	int32 mCurrentFireIndex = 0;
};