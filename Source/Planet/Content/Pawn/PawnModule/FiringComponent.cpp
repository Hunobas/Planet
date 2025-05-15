// FiringComponent.cpp
#include "FiringComponent.h"

#include "Engine/DamageEvents.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemyPawn.h"
#include "WaveManagerComponent.h"
#include "JustAimManagerComponent.h"

UFiringComponent::UFiringComponent(): TargetPlayer(EAutoReceiveInput::Disabled), JustAimDefaultNS(nullptr), MuzzleTemplate(nullptr),
                                      MuzzlePoint(nullptr),
                                      TargetPawn(nullptr),
                                      mOwner(nullptr), mEnemyOwner(nullptr),
                                      cWeaponOwner(nullptr),
                                      mTargetPlanet(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFiringComponent::BeginPlay()
{
	Super::BeginPlay();

	setOwnerParams();
	setTargetParams();

#ifdef DEBUG
	FString newName = FString::Printf(TEXT("FireComponent_%d"), GetUniqueID());
	this->Rename(*newName);
#endif
}

void UFiringComponent::StartFireSequence(const TFunction<void(const UFiringComponent*)>& _callback)
{
	check(mOwner);
    
	SpawnSystemAttachedFacingForward(JustAimDefaultNS, MuzzlePoint);

	mOwner->GetWorldTimerManager().SetTimer(mJustAimWindowTimerHandle, this, &UFiringComponent::startJustAimWindow, JustAimDelay, false);
	mOwner->GetWorldTimerManager().SetTimer(mFireTimerHandle, this, &UFiringComponent::Fire, FireDelay, false);

	mDequeueCallback = _callback;
}

void UFiringComponent::Fire()
{
	check(mOwner);
	check(TargetPawn);
	
	SpawnSystemAttachedFacingForward(MuzzleTemplate, MuzzlePoint);
	// TODO: mMuzzlePoint로부터 cTargetPawn을 찾을 때까지 레이캐스팅 한 뒤 피격 이펙트 소환
	
	const float damage = getOwnerDamage();
	
	FDamageEvent damageEvent;
	TargetPawn->TakeDamage(damage, damageEvent, mOwner->GetController(), mOwner);

	if (mTargetPlanet)
	{
		mTargetPlanet->JustAimManager->FailJustAim();
	}

	invokeDequeueCallback();
}

void UFiringComponent::HandleJustAim()
{
	if (!mEnemyOwner)
		return;
	
	check(mTargetPlanet);

	if (bIsInJustAimWindow && mTargetPlanet->JustAimManager->HasJustAimed(MuzzlePoint))
	{
		FDamageEvent damageEvent;
		mEnemyOwner->TakeDamage(JustAimDamage, damageEvent, mTargetPlanet->GetController(), mEnemyOwner);
		mTargetPlanet->JustAimManager->SucceedJustAim(MuzzlePoint);

		invokeDequeueCallback();
    
		mEnemyOwner->GetWorldTimerManager().ClearTimer(mFireTimerHandle);
		bIsInJustAimWindow = false;
	}
}

void UFiringComponent::setOwnerParams()
{
	mOwner = Cast<APawn>(GetOwner());
	mEnemyOwner = Cast<AEnemyPawn>(GetOwner());
	// cWeaponOwner = Cast<AWeaponPawn>(GetOwner());

	check(MuzzlePointTag != NAME_None);
	if (mOwner)
	{
		if (!TryGetFirstComponentWithTag(mOwner, MuzzlePointTag, MuzzlePoint))
		{
			UE_LOG(LogTemp, Error, TEXT("[UFiringComponent] %s->'%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
				*mOwner->GetName(), *MuzzlePointTag.ToString());
		}
		check(MuzzlePoint != nullptr);
	}
}

void UFiringComponent::setTargetParams()
{
	if (TargetPlayer != EAutoReceiveInput::Disabled)
	{
		TargetPawn = GetTargetPlayerPawn(TargetPlayer, this);
		mTargetPlanet = Cast<APlanetPawn>(TargetPawn);
	}
	else if (cWeaponOwner)
	{
		// TODO: 자신의 사정거리 반경 내에 cTargetPawn을 찾는 TryFindTarget 구현
	}
}

void UFiringComponent::startJustAimWindow()
{
	bIsInJustAimWindow = true;
	mOwner->GetWorldTimerManager().SetTimer(mJustAimWindowTimerHandle, this, &UFiringComponent::endJustAimWindow, FireDelay - JustAimDelay - 0.01f, false);
}

void UFiringComponent::endJustAimWindow()
{
	bIsInJustAimWindow = false;
}

void UFiringComponent::invokeDequeueCallback()
{
	if (!mDequeueCallback)
		return;
		
	mDequeueCallback(this);
	mDequeueCallback = nullptr;
}

float UFiringComponent::getOwnerDamage() const
{
	if (mEnemyOwner)
	{
		return CalculateDamage(mEnemyOwner->RuntimeSettings.Damage, 0.0f, mEnemyOwner->ActiveBuffs.DamageScale);
	}
	// else if (cWeaponOwner)
	// {
	// return CalculateDamage(cWeaponOwner->Setting.Attack, cWeaponOwner->Setting.Attack, 1.0f);
	// }
	return 0.0f;
}