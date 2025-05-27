// PlanetHUD.cpp
#include "PlanetHUD.h"

#include "Blueprint/UserWidget.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "LevelComponent.h"
#include "HPComponent.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"
#include "IRewardData.h"
#include "RewardManager.h"
#include "HUDWidget.h"
#include "RewardSelectionWidget.h"

void APlanetHUD::BeginPlay()
{
	Super::BeginPlay();
	
	cPlayerPawn = Cast<APlanetPawn>(GetOwningPawn());
	check(cPlayerPawn);

	mHUDWidget = CreateWidget<UHUDWidget>(GetOwningPlayerController(), HUDClass);
	check(mHUDWidget);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlanetHUD::beginLatePlay);
}

void APlanetHUD::beginLatePlay() const
{
	check(cPlayerPawn->Level);
	OnXPGain(cPlayerPawn->Level->CurrentXP, cPlayerPawn->Level->XPToNextLevel);
	OnLevelUp(cPlayerPawn->Level->CurrentLevel);

	check(cPlayerPawn->HP);
	OnHPChanged(cPlayerPawn->HP->CurrentHP, cPlayerPawn->HP->MaxHP);

	check(cPlayerPawn->WeaponSlot)
	OnWeaponSlotChanged(cPlayerPawn->WeaponSlot->EquippedWeapons);

	check(cPlayerPawn->ItemSlot)
	OnItemSlotChanged(cPlayerPawn->ItemSlot->EquippedItems);

	mHUDWidget->AddToViewport(LOW_ORDER);
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

void APlanetHUD::OnXPGain(const float& _currentXP, const float& _xpToNextLevel) const
{
	mHUDWidget->UpdateXPProgress(_currentXP, _xpToNextLevel);
}

void APlanetHUD::OnLevelUp(const int32& _currentLevel) const
{
	mHUDWidget->UpdateCurrentLevel(_currentLevel);
}

void APlanetHUD::OnHPChanged(const float& _currentHP, const float& _maxHP) const
{
	mHUDWidget->UpdateHP(_currentHP, _maxHP);
}

void APlanetHUD::OnWeaponSlotChanged(const TArray<AWeaponPawn*>& _equippedWeapons) const
{
	mHUDWidget->UpdateAllWeaponIcons(_equippedWeapons);
}

void APlanetHUD::OnItemSlotChanged(const TArray<APassiveItem*>& _equippedItems) const
{
	mHUDWidget->UpdateAllItemIcons(_equippedItems);
}
