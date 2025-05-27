// LevelManager.cpp
#include "LevelManager.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlanetHUD.h"

ULevelManager::ULevelManager(): cOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULevelManager::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	calculateNextLevelXP();
}

void ULevelManager::GainXP(float XP)
{
	CurrentXP += CalculateXPGain(XP, cOwner->RuntimeSettings.XpGain);
	while (CurrentXP >= XPToNextLevel)
	{
		processLevelUp();
	}

	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->OnXPGain(CurrentXP, XPToNextLevel);
}

void ULevelManager::calculateNextLevelXP()
{
	XPToNextLevel = CalculateXPToNextLevel(CurrentLevel);
}

void ULevelManager::processLevelUp()
{
	CurrentXP -= XPToNextLevel;
	CurrentLevel++;
	calculateNextLevelXP();

	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->ShowRewardSelection();
	cOwner->PlanetHUD->OnLevelUp(CurrentLevel);
}
