// ADefaultLaser.cpp
#include "DefaultLaser.h"

#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "WeaponPawn.h"
#include "EnemyPawn.h"

ADefaultLaser::ADefaultLaser()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("LaserRoot"));
    RootComponent = Root;

    LaserEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Effect"));
    LaserEffect->SetupAttachment(RootComponent);
}

void ADefaultLaser::Initialize(AWeaponPawn* _owner)
{
    cOwner = _owner;
    cOwnPlanet = Cast<APlanetPawn>(_owner->GetOwner());

    if (LaserType == ELaserType::Finite)
    {
        GetWorld()->GetTimerManager().SetTimer(
            mLifeSpanTimerHandle,
            this,
            &ADefaultLaser::destroyLaser,
            LifeSpan,
            false
        );
    }
    
    if (LaserEffect)
    {
        LaserEffect->Activate();
    }
}

void ADefaultLaser::FireTick()
{
    if (!cOwner || !cOwnPlanet || IsActorBeingDestroyed())
        return;

    FHitResult hit;
    const FVector startLocation = GetActorLocation();
    const FVector endLocation   = startLocation + (GetActorForwardVector() * TraceDistance);

    if (GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECC_LASER))
    {
        const float playerDamage = cOwnPlanet->RuntimeSettings.Damage;
        const float playerCritical = cOwnPlanet->RuntimeSettings.Critical;
        const float playerCriticalDamage = cOwnPlanet->RuntimeSettings.CriticalDamage;
        const float damage = CalculateCriticalDamage(playerDamage, cOwner->Damage, playerCritical, playerCriticalDamage);

        FDamageEvent damageEvent;
        hit.GetActor()->TakeDamage(damage, damageEvent, cOwner->GetController(), this);

        SpawnSystemAttachedFacingForward(LaserHitTemplate, hit.GetComponent());
    }

    updateLaserEndPosition(hit.GetActor());
}

void ADefaultLaser::updateLaserEndPosition(const AActor* _targetEnemy) const
{
    if (!LaserEffect)
        return;
    
    float distance = TraceDistance;

    if (_targetEnemy)
    {
        const FVector laserStart = GetActorLocation();
        distance = FVector::Dist(laserStart, _targetEnemy->GetActorLocation());
    }
    
    FVector endPosition = FVector::UnitX() * distance;
    LaserEffect->SetVariableVec3(FName("End"), endPosition);
}


void ADefaultLaser::destroyLaser()
{
    GetWorld()->GetTimerManager().ClearTimer(mLifeSpanTimerHandle);

    if (LaserEffect)
    {
        LaserEffect->Deactivate();
    }

    Destroy();
}
