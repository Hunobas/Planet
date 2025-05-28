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

    switch (LaserType)
    {
    case ELaserType::Infinite:
        Fire();
        break;

    case ELaserType::Finite:
        Fire();

        GetWorld()->GetTimerManager().SetTimer(
            mLifeSpanTimerHandle,
            this,
            &ADefaultLaser::destroyLaser,
            LifeSpan,
            false
        );
        break;
    }

    if (LaserEffect)
    {
        LaserEffect->Activate();
    }
}

void ADefaultLaser::Fire()
{
    GetWorld()->GetTimerManager().ClearTimer(mDamageTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        mDamageTimerHandle,
        this,
        &ADefaultLaser::performLaserTrace,
        DamageInterval,
        true,
        0.0f
    );
}

void ADefaultLaser::performLaserTrace()
{
    if (!cOwner || !cOwnPlanet)
        return;

    TArray<FHitResult> hitResults;
    const FVector startLocation = GetActorLocation();
    const FVector endLocation   = startLocation + (GetActorForwardVector() * TraceDistance);

    UKismetSystemLibrary::LineTraceMulti(
        GetWorld(),
        startLocation,
        endLocation,
        UEngineTypes::ConvertToTraceType(ECC_LASER),
        false,
        TArray<AActor*>(),
        EDrawDebugTrace::None,
        hitResults,
        true
    );

    TArray<AActor*> targetEnemies;
    TSet<AActor*> processedActors;

    for (const FHitResult& hit : hitResults)
    {
        if (targetEnemies.Num() >= MaxPierce)
            break;

        AActor* hitActor = hit.GetActor();
        if (hitActor && Cast<AEnemyPawn>(hitActor) && !processedActors.Contains(hitActor))
        {
            processedActors.Add(hitActor);
            targetEnemies.Add(hitActor);
        }
    }

    updateLaserEndPosition(targetEnemies);

    for (AActor* enemy : targetEnemies)
    {
        SpawnSystemFacingForward(LaserHitTemplate, enemy);
        
        const float playerDamage = cOwnPlanet->RuntimeSettings.Damage;
        const float playerCritical = cOwnPlanet->RuntimeSettings.Critical;
        const float playerCriticalDamage = cOwnPlanet->RuntimeSettings.CriticalDamage;
        const float damage = CalculateCriticalDamage(playerDamage, cOwner->Damage, playerCritical, playerCriticalDamage);

        FDamageEvent damageEvent;
        enemy->TakeDamage(damage, damageEvent, cOwner->GetController(), this);
    }
}

void ADefaultLaser::updateLaserEndPosition(const TArray<AActor*>& _targetEnemies) const
{
    if (!LaserEffect)
        return;

    FVector endPosition = FVector::UnitX() * TraceDistance;

    if (_targetEnemies.Num() > 0)
    {
        const FVector laserStart = GetActorLocation();
        float maxDistance = 0.0f;

        for (AActor* enemy : _targetEnemies)
        {
            const float distance = FVector::Dist(laserStart, enemy->GetActorLocation());
            if (distance > maxDistance)
            {
                maxDistance = distance;
            }
        }

        endPosition = FVector::UnitX() * maxDistance;
    }

    LaserEffect->SetVariableVec3(FName("End"), endPosition);
}

void ADefaultLaser::destroyLaser()
{
    GetWorld()->GetTimerManager().ClearTimer(mLifeSpanTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(mDamageTimerHandle);

    if (LaserEffect)
    {
        LaserEffect->Deactivate();
    }

    Destroy();
}
