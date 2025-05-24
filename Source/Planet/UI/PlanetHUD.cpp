// PlanetHUD.cpp
#include "PlanetHUD.h"

#include "Blueprint/UserWidget.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "CursorWidget.h"
#include "IRewardData.h"
#include "RewardManager.h"
#include "RewardSelectionWidget.h"

void APlanetHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CursorWidgetClass)
	{
		mCursorWidget = CreateWidget<UCursorWidget>(GetWorld(), CursorWidgetClass);
		mCursorWidget->AddToViewport(HIGH_ORDER * HIGH_ORDER);
	}

	cPlayerPawn = Cast<APlanetPawn>(GetTargetPlayerPawn(TargetPlayer, this));
	check(cPlayerPawn);
}

void APlanetHUD::ShowRewardSelection()
{
	const TArray<TScriptInterface<IRewardData>> Rewards = cPlayerPawn->RewardManager->GetAvailableRewards(REWARD_COUNT);
	check(Rewards.Num() == REWARD_COUNT);
	
	URewardSelectionWidget* RewardWidget = CreateWidget<URewardSelectionWidget>(GetOwningPlayerController(), RewardSelectionClass);
	check(RewardWidget);
	
	RewardWidget->AddToViewport();
	RewardWidget->DisplayRewards(Rewards);
}
