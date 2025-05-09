// FiringComponent.cpp
#include "FiringComponent.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemyPawn.h"

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
		if (cEnemyOwner && bIsInJustAimWindow /* && TargetPlanet->JustAimManager->HasJustAimed()*/)
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
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		JustAimTemplate,
		mMuzzlePoint,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);

	cOwner->GetWorldTimerManager().SetTimer(mJustAimWindowTimerHandle, this, &UFiringComponent::startJustAimWindow, JustAimDelay, false);
	cOwner->GetWorldTimerManager().SetTimer(mFireTimerHandle, this, &UFiringComponent::Fire, FireDelay, false);
}

void UFiringComponent::Fire()
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleTemplate,
		mMuzzlePoint,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);

	check(cTargetPawn);
	
	const float damage = getOwnerDamage();
	
	FDamageEvent damageEvent;
	cTargetPawn->TakeDamage(damage, damageEvent, cOwner->GetController(), cOwner);
}

void UFiringComponent::setOwnerParams()
{
	cOwner = Cast<APawn>(GetOwner());
	cEnemyOwner = Cast<AEnemyPawn>(GetOwner());
	// cWeaponOwner = Cast<AWeaponPawn>(GetOwner());
	
	if (cOwner && MuzzlePointTag != NAME_None)
	{
		if (!TryGetFirstComponentWithTag(cOwner, MuzzlePointTag, mMuzzlePoint))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] '%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
				*cOwner->GetName(), *MuzzlePointTag.ToString());
		}
	}
}

void UFiringComponent::setTargetParams()
{
	if (TargetPlayer != EAutoReceiveInput::Disabled)
	{
		const int32 playerIndex = int32(TargetPlayer.GetValue()) - 1;
		cTargetPawn = UGameplayStatics::GetPlayerPawn(this, playerIndex);
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
		return CalculateDamage(cEnemyOwner->RuntimeSettings.Damage, 0.0f/*, WaveManager->AttackMultiplier*/);
	}
	// else if (cWeaponOwner)
	// {
	// return CalculateDamage(cWeaponOwner->Setting.Attack, cWeaponOwner->Setting.Attack, WaveManager->AttackMultiplier);
	// }
	return 0.0f;
}