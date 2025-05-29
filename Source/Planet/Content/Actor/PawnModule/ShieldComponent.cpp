// ShieldComponent.cpp
#include "ShieldComponent.h"

#include "NiagaraComponent.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemyPawn.h"
#include "HPComponent.h"
#include "PlanetHUD.h"

UShieldComponent::UShieldComponent()
    : CurrentShield(0.0f)
      , ShieldEffectTemplate(nullptr), cHPComponent(nullptr)
      , mPlanet(nullptr)
      , mEnemy(nullptr), mCurrentShieldEffect(nullptr), bIsDecaying(false)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UShieldComponent::BeginPlay()
{
    Super::BeginPlay();

    mPlanet = Cast<APlanetPawn>(GetOwner());
    mEnemy = Cast<AEnemyPawn>(GetOwner());

    if (mPlanet)
    {
        cHPComponent = mPlanet->HP;
        mPlanet->OnTakeAnyDamage.AddUniqueDynamic(this, &UShieldComponent::OnTakeAnyDamage);
    }
    else if (mEnemy)
    {
        TryGetFirstComponentWithTag(mEnemy, HP_TAG, cHPComponent);
        mEnemy->OnTakeAnyDamage.RemoveAll(cHPComponent);
        mEnemy->OnTakeAnyDamage.AddUniqueDynamic(this, &UShieldComponent::OnTakeAnyDamage);
    }
    check(cHPComponent);
}

void UShieldComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
    Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

    if (!cHPComponent)
        return;

    if (CurrentShield <= 0.0f)
    {
        if (IsValid(mCurrentShieldEffect))
        {
            OnShieldOff.Broadcast();
            mCurrentShieldEffect->DestroyComponent();
            mCurrentShieldEffect = nullptr;
        }
        return;
    }
    
    if (bIsDecaying)
    {
        updateDecay();
    }
}

void UShieldComponent::AddShield(float _shieldAmount)
{
    if (_shieldAmount <= 0.0f)
        return;

    CurrentShield += _shieldAmount;
    
    GetWorld()->GetTimerManager().ClearTimer(mDecayDelayHandle);
    bIsDecaying = false;
    
    startDecayTimer();
    updateUI();

    if (!IsValid(mCurrentShieldEffect))
    {
        check(ShieldEffectTemplate);
        mCurrentShieldEffect = SpawnSystemAttachedFacingForward(ShieldEffectTemplate, GetOwner()->GetRootComponent());
    }
}

void UShieldComponent::OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
    if (_damage <= 0.0f || CurrentShield <= 0.0f)
        return;
    
    const float shieldDamage = FMath::Min(CurrentShield, _damage);
    CurrentShield = FMath::Max(0.0f, CurrentShield - shieldDamage);
    
    updateUI();
    
    const float remainingDamage = _damage - shieldDamage;
    if (cHPComponent && remainingDamage > 0.0f)
    {
        cHPComponent->OnTakeAnyDamage(_damageCauser, remainingDamage, _damageType, _instigatedBy, _damagedActor);
    }
}

void UShieldComponent::startDecayTimer()
{
    if (CurrentShield <= 0.0f)
        return;
    
    GetWorld()->GetTimerManager().SetTimer(
        mDecayDelayHandle,
        [this]()
        {
            if (CurrentShield > 0.0f)
            {
                bIsDecaying = true;
            }
        },
        DecayDelay,
        false
    );
}

void UShieldComponent::updateDecay()
{
    if (CurrentShield <= 0.0f)
    {
        bIsDecaying = false;
        return;
    }
    
    const float decayAmount = calculateDecayAmount();
    CurrentShield = FMath::Max(0.0f, CurrentShield - decayAmount);

    updateUI();
}

float UShieldComponent::calculateDecayAmount() const
{
    if (!cHPComponent)
        return DecayMinAmount;
    
    const float shieldToHpRatio = CurrentShield / cHPComponent->CurrentHP;
    const float normalizedRatio = FMath::Clamp(shieldToHpRatio, DecayMinAmount, DecayMaxAmount);
    return EaseInCubic(normalizedRatio);
}

void UShieldComponent::updateUI() const
{
    if (!cHPComponent)
        return;
    
    if (mPlanet)
    {
        check(mPlanet->PlanetHUD);
        mPlanet->PlanetHUD->OnShieldChanged(CurrentShield, cHPComponent->CurrentHP, cHPComponent->MaxHP);
    }
    else if (mEnemy)
    {
        // TODO: UEnemyHPWidget에 관련된 무언가
        // 적 실드는 요일실드가 따로 있는데.. 굳이 필요할까..?
    }
}
