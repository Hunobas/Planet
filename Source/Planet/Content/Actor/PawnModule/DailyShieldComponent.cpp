// DailyShieldComponent.cpp
#include "DailyShieldComponent.h"

#include "NiagaraComponent.h"

#include "../Planet.h"
#include "EnemyPawn.h"
#include "PlanetPawn.h"
#include "DayOfWeekComponent.h"
#include "HPComponent.h"
#include "EnemyHPWidget.h"

UDailyShieldComponent::UDailyShieldComponent()
    : CurrentShield(0.0f)
      , ShieldEffectTemplate(nullptr), cHPComponent(nullptr)
      , mEnemy(nullptr), mCurrentShieldEffect(nullptr)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDailyShieldComponent::BeginPlay()
{
    Super::BeginPlay();

    mEnemy = Cast<AEnemyPawn>(GetOwner());
    
    if (mEnemy)
    {
        TryGetFirstComponentWithTag(mEnemy, HP_TAG, cHPComponent);
        // TODO: 적의 일반 Shield도 고려해야 할까?
        mEnemy->OnTakeAnyDamage.RemoveAll(cHPComponent);
        mEnemy->OnTakeAnyDamage.AddUniqueDynamic(this, &UDailyShieldComponent::OnTakeAnyDamage);
    }
    check(cHPComponent);
}

void UDailyShieldComponent::AddShield(float _shieldAmount)
{
    if (_shieldAmount <= 0.0f)
        return;

    CurrentShield += _shieldAmount;
    updateUI();

    if (!IsValid(mCurrentShieldEffect))
    {
        check(mEnemy);
        check(ShieldEffectTemplate);
        mCurrentShieldEffect = SpawnSystemAttachedFacingForward(ShieldEffectTemplate, mEnemy->GetRootComponent());
    }
}

void UDailyShieldComponent::OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
    if (_damage <= 0.0f || !isWeaknessDay())
        return;

    if (CurrentShield <= 0.0f)
    {
        // TODO: 적의 일반 Shield도 고려해야 할까?
        cHPComponent->OnTakeAnyDamage(_damageCauser, _damage, _damageType, _instigatedBy, _damagedActor);
        destroyShieldEffect();
        return;
    }
    
    const float shieldDamage = FMath::Min(CurrentShield, _damage);
    CurrentShield = FMath::Max(0.0f, CurrentShield - shieldDamage);
    
    updateUI();
    
    const float remainingDamage = _damage - shieldDamage;
    if (cHPComponent && remainingDamage > 0.0f)
    {
        // TODO: 적의 일반 Shield도 고려해야 할까?
        cHPComponent->OnTakeAnyDamage(_damageCauser, remainingDamage, _damageType, _instigatedBy, _damagedActor);
        destroyShieldEffect();
    }
}

bool UDailyShieldComponent::isWeaknessDay() const
{
    check(mEnemy);
    if (mEnemy->TargetPawn)
    {
        if (APlanetPawn* attacker = Cast<APlanetPawn>(mEnemy->TargetPawn))
        {
            check(attacker->DayOfWeek);
            return WeaknessDay == attacker->DayOfWeek->CurrentDay;
        }
    }
    return false;
}

void UDailyShieldComponent::destroyShieldEffect()
{
    if (IsValid(mCurrentShieldEffect))
    {
        mCurrentShieldEffect->DestroyComponent();
        mCurrentShieldEffect = nullptr;
    }
}

void UDailyShieldComponent::updateUI() const
{
    if (!cHPComponent)
        return;
    
    if (mEnemy)
    {
        // TODO: UEnemyHPWidget에 관련된 무언가
    }
}
