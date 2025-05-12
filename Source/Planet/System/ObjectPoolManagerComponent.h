// ObjectPoolManagerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolManagerComponent.generated.h"

struct FPool
{
	TArray<UObject*> Available;
	TArray<UObject*> InUse;
};

USTRUCT(BlueprintType)
struct FPoolConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Object Pool", meta=(AllowAbstract="false"))
	TSubclassOf<UObject> PooledClass;
	UPROPERTY(EditAnywhere, Category = "Object Pool", meta=(ClampMin="0"))
	int32 PoolSize = 10;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UObjectPoolManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type _endPlayReason) override;

public:	
	/** 지정한 클래스의 객체를 하나 가져옵니다. 
	 *  Actor라면 SpawnActor가 아닌 미리 스폰된 액터를 재활용합니다. */
	template<typename T>
	T* AcquireOrNull(const TSubclassOf<T>& _inClass, const FTransform& _spawnTransform = FTransform::Identity)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "Acquire<T>는 UObject 계열만 지원합니다.");
		const UClass* key = _inClass.Get();
		FPool* pool = mPools.Find(key);
		checkf(pool, TEXT("[ObjectPool] 등록되지 않은 클래스: %s"), *key->GetName());

		if (pool->Available.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ObjectPool] %s 풀 비어있음"), *key->GetName());
			return nullptr;
		}

		UObject* raw = pool->Available.Pop();
		T* obj = Cast<T>(raw);
		checkf(obj, TEXT("[ObjectPool] 다운캐스트 실패: %s"), *key->GetName());

		if (AActor* A = Cast<AActor>(obj))
		{
			A->SetActorTransform(_spawnTransform);
			A->SetActorHiddenInGame(false);
			A->SetActorEnableCollision(true);
		}

		pool->InUse.Add(obj);
		return obj;
	}

	/** 이전에 AcquireOrNull으로 꺼낸 객체를 다시 풀로 반환합니다. */
	void Release(UObject* _obj);

	UPROPERTY(EditAnywhere, Category="Object Pool")
	TArray<FPoolConfig> PoolConfigs;

private:
	void prepopulatePool(UClass* _inClass, int32 _count);
    
	TMap<UClass*, FPool> mPools;
};
