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

void ADefaultProjectile::Initialize(UObjectPoolManagerComponent* _pool)
{
	cOwner = Cast<AWeaponPawn>(GetOwner());
	mPool = _pool;

	check(ProjectileMovement);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;

	check(CollisionBox);
	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADefaultProjectile::OnOverlapBegin);

	GetWorld()->GetTimerManager().SetTimer(
		mLifeSpanTimerHandle,
		this,
		&ADefaultProjectile::returnToPool,
		LifeSpan,
		false
	);
}

void ADefaultProjectile::OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (mHitActors.Contains(_otherActor))
		return;
	
	APlanetPawn* playerPawn = Cast<APlanetPawn>(cOwner->GetOwner());
	check(playerPawn);
	check(cOwner);

	if (Cast<AEnemyPawn>(_otherActor) != nullptr)
	{
		mHitActors.Add(_otherActor);
		
		const float playerDamage			= playerPawn->RuntimeSettings.Damage;
		const float playerCritical			= playerPawn->RuntimeSettings.Critical;
		const float playerCriticalDamage	= playerPawn->RuntimeSettings.CriticalDamage;
		
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

void ADefaultProjectile::returnToPool()
{
	GetWorld()->GetTimerManager().ClearTimer(mLifeSpanTimerHandle);
	mCurrentPierce = 0;
	mHitActors.Empty();
	mPool->Release(this);
}