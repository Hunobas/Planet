// ADefaultProjectile.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultProjectile.generated.h"

class UObjectPoolManagerComponent;
class UBoxComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class AWeaponPawn;

UCLASS()
class PLANET_API ADefaultProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ADefaultProjectile();
	void Initialize(UObjectPoolManagerComponent* _pool);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* _hitComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit);

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpan	= 4.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Speed		= 5000.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxPierce	= 1;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* HitTemplate;

private:
	void returnToPool();
	
	AWeaponPawn* cOwner;
	UObjectPoolManagerComponent* mPool;
	TSet<AActor*> mHitActors;

	FTimerHandle mLifeSpanTimerHandle;
	int32 mCurrentPierce = 0;
};
