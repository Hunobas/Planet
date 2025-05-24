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
	mCurrentXP += CalculateXPGain(XP, cOwner->RuntimeSettings.XpGain);
	while (mCurrentXP >= mXPToNextLevel)
	{
		processLevelUp();
	}
}

void ULevelManager::calculateNextLevelXP()
{
	mXPToNextLevel = CalculateXPToNextLevel(mCurrentLevel);
}

void ULevelManager::processLevelUp()
{
	mCurrentXP -= mXPToNextLevel;
	mCurrentLevel++;
	calculateNextLevelXP();

	cOwner->PlanetHUD->ShowRewardSelection();
}
