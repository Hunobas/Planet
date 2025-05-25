// ObjectPoolManagerComponent.cpp
#include "ObjectPoolManagerComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UObjectPoolManagerComponent::UObjectPoolManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UObjectPoolManagerComponent::BeginPlay()
{
	Super::BeginPlay();
    
	for (const FPoolConfig& config : PoolConfigs)
	{
		if (config.ActorClass && config.InitialPoolSize > 0)
		{
			repopulatePool(config.ActorClass, config.InitialPoolSize);
		}
	}
}

void UObjectPoolManagerComponent::EndPlay(const EEndPlayReason::Type _endPlayReason)
{
	FScopeLock lock(&mPoolCriticalSection);
    
	for (AActor* actor : mAllPooledActors)
	{
		if (IsValid(actor))
		{
			actor->Destroy();
		}
	}
    
	mAllPooledActors.Empty();
    
	for (auto& pair : mPoolMap)
	{
		delete pair.Value;
	}
	mPoolMap.Empty();
    
	Super::EndPlay(_endPlayReason);
}

void UObjectPoolManagerComponent::Release(AActor* _actor)
{
    if (!IsValid(_actor))
        return;

    FScopeLock lock(&mPoolCriticalSection);
    
    TSubclassOf<AActor> actorClass = _actor->GetClass();
    FPoolData* poolData = getOrCreatePoolData(actorClass);
    
    if (!poolData)
        return;

    AActor* dequeuedActor = nullptr;
    TQueue<AActor*, EQueueMode::Mpsc> tempQueue;
    
    while (poolData->InUse.Dequeue(dequeuedActor))
    {
        if (dequeuedActor == _actor)
        {
            setActorActiveState(_actor, false);
            poolData->Available.Enqueue(_actor);
            break;
        }
        else
        {
            tempQueue.Enqueue(dequeuedActor);
        }
    }
    
    while (tempQueue.Dequeue(dequeuedActor))
    {
        poolData->InUse.Enqueue(dequeuedActor);
    }
}

void UObjectPoolManagerComponent::repopulatePool(const TSubclassOf<AActor>& _actorClass, const int32& _count)
{
    if (!_actorClass || _count <= 0)
        return;

    UWorld* world = GetWorld();
    if (!world)
        return;

    FScopeLock lock(&mPoolCriticalSection);
    
    FPoolData* poolData = getOrCreatePoolData(_actorClass);
    if (!poolData)
        return;

    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < _count; ++i)
    {
	    if (AActor* newActor = world->SpawnActor<AActor>(_actorClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams))
        {
            setActorActiveState(newActor, false);
            poolData->Available.Enqueue(newActor);
            mAllPooledActors.Add(newActor);
        }
    }
}

void UObjectPoolManagerComponent::setActorActiveState(AActor* _actor, bool _active)
{
	if (!IsValid(_actor))
		return;

	_actor->SetActorTickEnabled(_active);
	_actor->SetActorEnableCollision(_active);
	_actor->SetActorHiddenInGame(!_active);
}

UObjectPoolManagerComponent::FPoolData* UObjectPoolManagerComponent::getOrCreatePoolData(const TSubclassOf<AActor>& _actorClass)
{
	if (!_actorClass)
		return nullptr;

	if (FPoolData** foundPoolData = mPoolMap.Find(_actorClass))
	{
		return *foundPoolData;
	}

	FPoolData* newPoolData = new FPoolData(_actorClass);
	mPoolMap.Add(_actorClass, newPoolData);
	return newPoolData;
}

AActor* UObjectPoolManagerComponent::createNewActor(const TSubclassOf<AActor>& _actorClass, const FTransform& _spawnTransform)
{
	UWorld* world = GetWorld();
	if (!world || !_actorClass)
		return nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* newActor = world->SpawnActor<AActor>(_actorClass, _spawnTransform, spawnParams);
	if (newActor)
	{
		mAllPooledActors.Add(newActor);
	}

	return newActor;
}