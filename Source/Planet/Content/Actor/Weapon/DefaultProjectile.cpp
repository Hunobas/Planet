// ADefaultProjectile.cpp
#include "DefaultProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "PlanetPawn.h"
#include "WeaponPawn.h"
#include "EnemyPawn.h"

ADefaultProjectile::ADefaultProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
}

void ADefaultProjectile::Initialize(AWeaponPawn* _owner, UObjectPoolManagerComponent* _pool)
{
	cOwner = _owner;
	cOwnPlanet = Cast<APlanetPawn>(_owner->GetOwner());
	mPool = _pool;
	
	reset();
    
	if (IsValid(this) && IsValid(CollisionBox) && !IsActorBeingDestroyed())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADefaultProjectile::OnOverlapBegin);
	}
    
	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
	}
    
	GetWorldTimerManager().SetTimer(
		mLifeSpanTimerHandle,
		this,
		&ADefaultProjectile::returnToPool,
		LifeSpan,
		false
	);
}

void ADefaultProjectile::OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (mHitActors.Contains(_otherActor) || !cOwner || !cOwnPlanet)
		return;

	if (Cast<AEnemyPawn>(_otherActor) != nullptr)
	{
		mHitActors.Add(_otherActor);
		
		const float playerDamage			= cOwnPlanet->RuntimeSettings.Damage;
		const float playerCritical			= cOwnPlanet->RuntimeSettings.Critical;
		const float playerCriticalDamage	= cOwnPlanet->RuntimeSettings.CriticalDamage;
		
		float damage = CalculateCriticalDamage(playerDamage, cOwner->Damage, playerCritical, playerCriticalDamage);

		FDamageEvent damageEvent;
		_otherActor->TakeDamage(damage, damageEvent, cOwner->GetController(), this);

		SpawnSystemFacingForward(HitTemplate, this);

		if (++mCurrentPierce >= MaxPierce) 
		{
			returnToPool();
		}
	}
}

void ADefaultProjectile::reset()
{
	if (!IsValidLowLevel() || !CollisionBox || !CollisionBox->IsValidLowLevel())
		return;
	
	GetWorldTimerManager().ClearTimer(mLifeSpanTimerHandle);
	
	check(CollisionBox);
	CollisionBox->OnComponentBeginOverlap.RemoveAll(this);
    
	mCurrentPierce = 0;
	mHitActors.Reset();
    
	check(ProjectileMovement);
	ProjectileMovement->Velocity = FVector::ZeroVector;
}

void ADefaultProjectile::returnToPool()
{
	if (!IsValid(this) || IsActorTickEnabled())
		return;
	
	reset();

	check(mPool);
	mPool->Release(this);
}
