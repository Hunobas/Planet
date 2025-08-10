// WeekendShift.cpp
#include "WeekendShift.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "DayOfWeekComponent.h"
#include "WaveManagerComponent.h"

AWeekendShift::AWeekendShift(): cOwner(nullptr), mWaveManager(nullptr), mThisWeekPay(0)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeekendShift::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	check(cOwner->DayOfWeek);
	cOwner->DayOfWeek->OnDayChanged.AddUniqueDynamic(this, &AWeekendShift::adjustPremiumPay);

	mWaveManager = GetPlanetGameMode(this)->WaveManager;
}

void AWeekendShift::LevelUp(const int32& _newLevel)
{
	Super::LevelUp(_newLevel);

	switch (_newLevel)
	{
	case 2:
		PremiumPay = PremiumPay_LV2;
		break;
	case 3:
		PremiumPay = PremiumPay_LV3;
		break;
	case 4:
		PremiumPay = PremiumPay_LV4;
		break;
	case 5:
		PremiumPay = PremiumPay_LV5;
		break;
	default:
		checkNoEntry();
		break;
	}
}

void AWeekendShift::adjustPremiumPay(EPlanetDayOfWeek _newDay)
{
	if (_newDay == EPlanetDayOfWeek::Saturday)
	{
		mWaveManager->Config_ScaleSettings.XPDropScale += PremiumPay;
		mThisWeekPay = PremiumPay;
	}
	else if (_newDay == EPlanetDayOfWeek::Monday)
	{
		check(mWaveManager->CurrentLevelConfig);
		float originWeekPay = mWaveManager->Config_ScaleSettings.XPDropScale - mThisWeekPay;
		mWaveManager->Config_ScaleSettings.XPDropScale = FMath::Max(originWeekPay, mWaveManager->CurrentLevelConfig->ScaleSettings.XPDropScale);
		mThisWeekPay = 0.0f;
	}
}
