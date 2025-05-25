// PlanetHUD.cpp
#include "PlanetHUD.h"

#include "Blueprint/UserWidget.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "IRewardData.h"
#include "RewardManager.h"
#include "RewardSelectionWidget.h"

void APlanetHUD::BeginPlay()
{
	Super::BeginPlay();
	
	cPlayerPawn = Cast<APlanetPawn>(GetOwningPawn());
	check(cPlayerPawn);
}

void APlanetHUD::ShowRewardSelection()
{
	const TArray<TScriptInterface<IRewardData>> Rewards = cPlayerPawn->RewardManager->GetAvailableRewards(REWARD_COUNT);
	check(Rewards.Num() == REWARD_COUNT);
	
	URewardSelectionWidget* RewardWidget = CreateWidget<URewardSelectionWidget>(GetOwningPlayerController(), RewardSelectionClass);
	check(RewardWidget);
	
	RewardWidget->AddToViewport(HIGH_ORDER);
	RewardWidget->DisplayRewards(Rewards);
}
