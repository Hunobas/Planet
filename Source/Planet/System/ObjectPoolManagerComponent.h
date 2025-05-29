// ObjectPoolManagerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "HAL/CriticalSection.h"
#include "ObjectPoolManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FPoolConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool Config")
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool Config", meta = (ClampMin = "1"))
    int32 InitialPoolSize = 10;

    FPoolConfig()
    {
        ActorClass = nullptr;
        InitialPoolSize = 10;
    }

    FPoolConfig(const TSubclassOf<AActor>& _actorClass, int32 _initialPoolSize)
        : ActorClass(_actorClass), InitialPoolSize(_initialPoolSize)
    {
    }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UObjectPoolManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UObjectPoolManagerComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type _endPlayReason) override;

public:
    template<typename T>
    T* AcquireOrNull(const TSubclassOf<T>& _actorClass, const FTransform& _spawnTransform)
    {
        if (!_actorClass)
            return nullptr;

        FScopeLock lock(&mPoolCriticalSection);
    
        FPoolData* poolData = getOrCreatePoolData(_actorClass);
        if (!poolData)
            return nullptr;

        AActor* actor = nullptr;
    
        if (!poolData->Available.Dequeue(actor))
        {
            actor = createNewActor(_actorClass, _spawnTransform);
            if (!actor)
                return nullptr;
        }

        if (IsValid(actor) && !actor->IsActorBeingDestroyed())
        {
            actor->SetActorTransform(_spawnTransform);
            setActorActiveState(actor, true);
        }
        return Cast<T>(actor);
    }

    void Release(AActor* _actor);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool", meta = (ExposeOnSpawn = "true"))
    TArray<FPoolConfig> PoolConfigs;

private:
    struct FPoolData
    {
        TQueue<AActor*, EQueueMode::Mpsc> Available;
        // TQueue<AActor*, EQueueMode::Mpsc> InUse;
        TSubclassOf<AActor> ActorClass;
        
        FPoolData() : ActorClass(nullptr) {}
        FPoolData(const TSubclassOf<AActor>& _actorClass) : ActorClass(_actorClass) {}
    };

    TMap<TSubclassOf<AActor>, FPoolData*> mPoolMap;

    UPROPERTY()
    TArray<AActor*> mAllPooledActors;

    mutable FCriticalSection mPoolCriticalSection;

    
    void repopulatePool(const TSubclassOf<AActor>& _actorClass, const int32& _count);
    static void setActorActiveState(AActor* _actor, bool _active);
    FPoolData* getOrCreatePoolData(const TSubclassOf<AActor>& _actorClass);
    AActor* createNewActor(const TSubclassOf<AActor>& _actorClass, const FTransform& _spawnTransform);
};
