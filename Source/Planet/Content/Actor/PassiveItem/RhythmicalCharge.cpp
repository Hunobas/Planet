// RhythmicalCharge.cpp
#include "RhythmicalCharge.h"

#include "PlanetPawn.h"
#include "JustAimManagerComponent.h"
#include "ShieldComponent.h"
#include "HPComponent.h"

ARhythmicalCharge::ARhythmicalCharge(): cOwner(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARhythmicalCharge::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	check(cOwner->JustAimManager);
	cOwner->JustAimManager->OnSuccessJustAim.AddLambda([this](USceneComponent* _firePoint)
	{
		addShieldAndHealToPlayer();
	});
}

void ARhythmicalCharge::LevelUp(const int32& _newLevel)
{
	Super::LevelUp(_newLevel);

	switch (_newLevel)
	{
	case 2:
		EachShield = EachShield_LV2;
		break;
	case 3:
		EachShield = EachShield_LV3;
		break;
	case 4:
		EachShield = EachShield_LV4;
		break;
	case 5:
		EachShield = EachShield_LV5;
		EachHeal = EachHeal_LV5;
		break;
	default:
		checkNoEntry();
		break;
	}
}

void ARhythmicalCharge::addShieldAndHealToPlayer() const
{
	check(cOwner->Shield);
	check(cOwner->HP);
	
	cOwner->Shield->AddShield(EachShield);
	cOwner->HP->Heal(EachHeal);
}
