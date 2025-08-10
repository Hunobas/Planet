// PushPullLegsSplit.cpp
#include "PushPullLegsSplit.h"

#include "DayOfWeekComponent.h"
#include "PlanetPawn.h"
#include "ShieldComponent.h"

APushPullLegsSplit::APushPullLegsSplit(): cOwner(nullptr), mWeeklyShield(0), bIsWeeklyRoutineEnd(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void APushPullLegsSplit::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	check(cOwner->DayOfWeek);
	cOwner->DayOfWeek->OnDayChanged.AddUniqueDynamic(this, &APushPullLegsSplit::addShieldToPlayer);

	check(cOwner->Shield);
	cOwner->Shield->OnShieldOff.AddLambda([this]()
	{
		bIsWeeklyRoutineEnd = false;
	});
}

void APushPullLegsSplit::LevelUp(const int32& _newLevel)
{
	Super::LevelUp(_newLevel);

	switch (_newLevel)
	{
	case 2:
		EachShield = EachShield_LV2;
		MaxWeeklyShield = EachShield_LV2 * 3;
		break;
	case 3:
		EachShield = EachShield_LV3;
		MaxWeeklyShield = EachShield_LV3 * 3;
		break;
	case 4:
		EachShield = EachShield_LV4;
		MaxWeeklyShield = EachShield_LV4 * 3;
		break;
	case 5:
		EachShield = EachShield_LV5;
		MaxWeeklyShield = EachShield_LV5 * 3;
		break;
	default:
		checkNoEntry();
		break;
	}
}

void APushPullLegsSplit::addShieldToPlayer(EPlanetDayOfWeek _newDay)
{
	if (bIsWeeklyRoutineEnd)
		return;
	
	if (_newDay != EPlanetDayOfWeek::Monday && _newDay != EPlanetDayOfWeek::Wednesday && _newDay != EPlanetDayOfWeek::Friday)
		return;

	check(cOwner->Shield);
	cOwner->Shield->AddShield(EachShield);
	mWeeklyShield += EachShield;
	
	if (mWeeklyShield >= MaxWeeklyShield)
	{
		mWeeklyShield = 0.0f;
		bIsWeeklyRoutineEnd = true;
	}
}