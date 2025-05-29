// LevelComponent.cpp
#include "LevelComponent.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlanetHUD.h"

ULevelComponent::ULevelComponent(): cOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULevelComponent::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	calculateNextLevelXP();
}

void ULevelComponent::GainXP(float XP)
{
	UE_LOG(LogTemp, Warning, TEXT("XP: %f"), XP);
	
	CurrentXP += CalculateXPGain(XP, cOwner->RuntimeSettings.XpGain);
	while (CurrentXP >= XPToNextLevel)
	{
		processLevelUp();
	}

	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->OnXPGain(CurrentXP, XPToNextLevel);
}

void ULevelComponent::calculateNextLevelXP()
{
	XPToNextLevel = CalculateXPToNextLevel(CurrentLevel);
}

void ULevelComponent::processLevelUp()
{
	CurrentXP -= XPToNextLevel;
	CurrentLevel++;
	calculateNextLevelXP();

	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->ShowRewardSelection();
	cOwner->PlanetHUD->OnLevelUp(CurrentLevel);
}
