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

	setActorActiveState(_actor, false);
	
	if (!poolData->Available.Enqueue(_actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ObjectPool] %s 클래스 풀 가득 참 - 액터 파괴됨."), 
			   *actorClass->GetName());
		_actor->Destroy();
		mAllPooledActors.Remove(_actor);
	}
}

void UObjectPoolManagerComponent::repopulatePool(const TSubclassOf<AActor>& _actorClass, const int32 _count)
{
    if (!_actorClass || _count <= 0)
        return;

    UWorld* world = GetWorld();
    if (!world)
        return;

    FScopeLock lock(&mPoolCriticalSection);
    
	FPoolData* poolData = getOrCreatePoolData(_actorClass, _count);
	if (!poolData)
		return;

    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	int32 successfullyCreated = 0;
	for (int32 i = 0; i < _count; ++i)
	{
		if (AActor* newActor = world->SpawnActor<AActor>(_actorClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams))
		{
			setActorActiveState(newActor, false);
            
			if (poolData->Available.Enqueue(newActor))
			{
				mAllPooledActors.Add(newActor);
				successfullyCreated++;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[ObjectPool] Queue full during prepopulation for %s at %d/%d"), 
					   *_actorClass->GetName(), i, _count);
				newActor->Destroy();
				break;
			}
		}
	}
    
	UE_LOG(LogTemp, Log, TEXT("[ObjectPool] Successfully created %d/%d actors for %s (Queue capacity: %d)"), 
		   successfullyCreated, _count, *_actorClass->GetName(), poolData->PoolCapacity);
}

void UObjectPoolManagerComponent::setActorActiveState(AActor* _actor, bool _active)
{
	if (!IsValid(_actor))
		return;
	
	_actor->SetActorTickEnabled(_active);
	_actor->SetActorEnableCollision(_active);
	_actor->SetActorHiddenInGame(!_active);
}

UObjectPoolManagerComponent::FPoolData* UObjectPoolManagerComponent::getOrCreatePoolData(const TSubclassOf<AActor>& _actorClass, int32 _suggestedCapacity)
{
	if (!_actorClass)
		return nullptr;

	if (FPoolData** foundPoolData = mPoolMap.Find(_actorClass))
	{
		return *foundPoolData;
	}

	int32 finalCapacity = FMath::Max(_suggestedCapacity * 2, 256);
	FPoolData* newPoolData = new FPoolData(_actorClass, finalCapacity);
	mPoolMap.Add(_actorClass, newPoolData);
    
	UE_LOG(LogTemp, Log, TEXT("[ObjectPool] Created new pool for %s with capacity %d"), 
		   *_actorClass->GetName(), finalCapacity);
    
	return newPoolData;
}

AActor* UObjectPoolManagerComponent::createNewActor(const TSubclassOf<AActor>& _actorClass, const FTransform& _spawnTransform)
{
	UWorld* world = GetWorld();
	if (!world || !_actorClass)
		return nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

#ifdef DEBUG
	UE_LOG(LogTemp, Error, TEXT("[UObjectPoolManagerComponent] @@@@@@@@@벌써 불리면 안되는데@@@@@@@ Create New Actor: %s"), *_actorClass->GetName());
#endif

	AActor* newActor = world->SpawnActor<AActor>(_actorClass, _spawnTransform, spawnParams);
	if (newActor)
	{
		mAllPooledActors.Add(newActor);
	}

	return newActor;
}