// ObjectPoolManagerComponent.cpp
#include "ObjectPoolManagerComponent.h"

UObjectPoolManagerComponent::UObjectPoolManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObjectPoolManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& [pooledClass, poolSize] : PoolConfigs)
	{
		if (pooledClass)
		{
			prepopulatePool(pooledClass.Get(), poolSize);
		}
	}
}

void UObjectPoolManagerComponent::EndPlay(const EEndPlayReason::Type _endPlayReason)
{
	for (auto& pair : mPools)
	{
		for (UObject* obj : pair.Value.Available)
		{
			if (AActor* A = Cast<AActor>(obj))
			{
				A->Destroy();
			}
		}
		for (UObject* obj : pair.Value.InUse)
		{
			if (AActor* A = Cast<AActor>(obj))
			{
				A->Destroy();
			}
		}
	}
	mPools.Empty();

	Super::EndPlay(_endPlayReason);
}


void UObjectPoolManagerComponent::Release(UObject* _obj)
{
	if (!_obj)
		return;

	const UClass* key = _obj->GetClass();
	FPool* pool = mPools.Find(key);
	if (!pool)
	{
		if (AActor* A = Cast<AActor>(_obj))
		{
			A->Destroy();
		}
		return;
	}

	if (AActor* A = Cast<AActor>(_obj))
	{
		A->SetActorHiddenInGame(true);
		A->SetActorEnableCollision(false);
	}

	pool->InUse.RemoveSingleSwap(_obj);
	pool->Available.Add(_obj);
}

void UObjectPoolManagerComponent::prepopulatePool(UClass* _inClass, int32 _count)
{
	UWorld* world = GetWorld();
	
	if (!_inClass || !world)
		return;
	
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FPool& pool = mPools.FindOrAdd(_inClass);

	for (int32 i = 0; i < _count; ++i)
	{
		UObject* obj = nullptr;

		if (_inClass->IsChildOf(AActor::StaticClass()))
		{
			obj = world->SpawnActor<AActor>(_inClass, FTransform::Identity, params);
			if (AActor* A = Cast<AActor>(obj))
			{
				A->SetActorHiddenInGame(true);
				A->SetActorEnableCollision(false);
			}
		}
		else
		{
			obj = NewObject<UObject>(this, _inClass);
		}

		check(obj);
		pool.Available.Add(obj);
	}
}

