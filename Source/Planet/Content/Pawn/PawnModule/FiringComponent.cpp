// FiringComponent.cpp
#include "FiringComponent.h"

#include "Engine/DamageEvents.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemyPawn.h"
#include "WaveManagerComponent.h"

UFiringComponent::UFiringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFiringComponent::BeginPlay()
{
	Super::BeginPlay();

	setOwnerParams();
	setTargetParams();
}

void UFiringComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	if (APlanetPawn* TargetPlanet = Cast<APlanetPawn>(cTargetPawn))
	{
		if (cEnemyOwner && bIsInJustAimWindow && false /* && TargetPlanet->JustAimManager->HasJustAimed()*/)
		{
			FDamageEvent damageEvent;
			cEnemyOwner->TakeDamage(JustAimDamage, damageEvent, TargetPlanet->GetController(), cEnemyOwner);
			// TargetPlanet->JustAimManager->SucceedJustAim(cOwner);
        
			cEnemyOwner->GetWorldTimerManager().ClearTimer(mFireTimerHandle);
			bIsInJustAimWindow = false;
		}
	}
}

void UFiringComponent::StartFireSequence()
{
	check(cOwner);
	
	SpawnSystemAttachedFacingForward(JustAimTemplate, mMuzzlePoint);

	cOwner->GetWorldTimerManager().SetTimer(mJustAimWindowTimerHandle, this, &UFiringComponent::startJustAimWindow, JustAimDelay, false);
	cOwner->GetWorldTimerManager().SetTimer(mFireTimerHandle, this, &UFiringComponent::Fire, FireDelay, false);
}

void UFiringComponent::Fire() const
{
	check(cOwner);
	check(cTargetPawn);
	
	SpawnSystemAttachedFacingForward(MuzzleTemplate, mMuzzlePoint);
	// TODO: mMuzzlePoint로부터 cTargetPawn을 찾을 때까지 레이캐스팅 한 뒤 피격 이펙트 소환
	
	const float damage = getOwnerDamage();
	
	FDamageEvent damageEvent;
	cTargetPawn->TakeDamage(damage, damageEvent, cOwner->GetController(), cOwner);
}

void UFiringComponent::setOwnerParams()
{
	cOwner = Cast<APawn>(GetOwner());
	cEnemyOwner = Cast<AEnemyPawn>(GetOwner());
	// cWeaponOwner = Cast<AWeaponPawn>(GetOwner());

	check(MuzzlePointTag != NAME_None);
	if (cOwner && MuzzlePointTag != NAME_None)
	{
		if (!TryGetFirstComponentWithTag(cOwner, MuzzlePointTag, mMuzzlePoint))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] '%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
				*cOwner->GetName(), *MuzzlePointTag.ToString());
		}
		check(mMuzzlePoint != nullptr);
	}
}

void UFiringComponent::setTargetParams()
{
	if (TargetPlayer != EAutoReceiveInput::Disabled)
	{
		cTargetPawn = GetTargetPlayerPawn(TargetPlayer, this);
	}
	else if (cWeaponOwner)
	{
		// TODO: 자신의 사정거리 반경 내에 cTargetPawn을 찾는 TryFindTarget 구현
	}
}

void UFiringComponent::startJustAimWindow()
{
	bIsInJustAimWindow = true;
	cOwner->GetWorldTimerManager().SetTimer(mJustAimWindowTimerHandle, this, &UFiringComponent::endJustAimWindow, FireDelay - JustAimDelay - 0.01f, false);
}

void UFiringComponent::endJustAimWindow()
{
	bIsInJustAimWindow = false;
}

float UFiringComponent::getOwnerDamage() const
{
	if (cEnemyOwner)
	{
		return CalculateDamage(cEnemyOwner->RuntimeSettings.Damage, 0.0f, cEnemyOwner->ActiveBuffs.DamageScale);
	}
	// else if (cWeaponOwner)
	// {
	// return CalculateDamage(cWeaponOwner->Setting.Attack, cWeaponOwner->Setting.Attack, 1.0f);
	// }
	return 0.0f;
}