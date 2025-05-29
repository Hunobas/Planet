// PlanetHUD.cpp
#include "PlanetHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "LevelComponent.h"
#include "HPComponent.h"
#include "ShieldComponent.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"
#include "DayOfWeekComponent.h"
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

	mDashLineWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), DashLineClass);
	check(mDashLineWidget);
	mDashLineRootWidget = Cast<UCanvasPanel>(mDashLineWidget->GetRootWidget());

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlanetHUD::beginLatePlay);
}

void APlanetHUD::beginLatePlay() const
{
	check(cPlayerPawn->Level);
	OnXPGain(cPlayerPawn->Level->CurrentXP, cPlayerPawn->Level->XPToNextLevel);
	OnLevelUp(cPlayerPawn->Level->CurrentLevel);

	check(cPlayerPawn->HP);
	OnHPChanged(cPlayerPawn->HP->CurrentHP, cPlayerPawn->HP->MaxHP);

	check(cPlayerPawn->Shield);
	OnShieldChanged(cPlayerPawn->Shield->CurrentShield, cPlayerPawn->HP->CurrentHP, cPlayerPawn->HP->MaxHP);

	check(cPlayerPawn->WeaponSlot)
	OnWeaponSlotChanged(cPlayerPawn->WeaponSlot->EquippedWeapons);

	check(cPlayerPawn->ItemSlot)
	OnItemSlotChanged(cPlayerPawn->ItemSlot->EquippedItems);

	check(cPlayerPawn->DayOfWeek);
	OnCurrentDayChanged(cPlayerPawn->DayOfWeek->CurrentDay);

	mDashLineWidget->AddToViewport(LOW_ORDER);
	mHUDWidget->AddToViewport(DEFAULT_ORDER);
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

void APlanetHUD::OnHPChanged(const float& _currentHP, const float& _maxHP) const
{
	mHUDWidget->UpdateHP(_currentHP, _maxHP);
}

void APlanetHUD::OnShieldChanged(const float& _currentShield, const float _currentHP, const float& _maxHP) const
{
	mHUDWidget->UpdateShield(_currentShield, _currentHP, _maxHP);
}

void APlanetHUD::OnXPGain(const float& _currentXP, const float& _xpToNextLevel) const
{
	mHUDWidget->UpdateXPProgress(_currentXP, _xpToNextLevel);
}

void APlanetHUD::OnLevelUp(const int32& _currentLevel) const
{
	mHUDWidget->UpdateCurrentLevel(_currentLevel);
}

void APlanetHUD::OnWeaponSlotChanged(const TArray<AWeaponPawn*>& _equippedWeapons) const
{
	mHUDWidget->UpdateAllWeaponIcons(_equippedWeapons);
}

void APlanetHUD::OnItemSlotChanged(const TArray<APassiveItem*>& _equippedItems) const
{
	mHUDWidget->UpdateAllItemIcons(_equippedItems);
}

void APlanetHUD::OnDailyProgressChanged(const float _dailyProgress, const float _weeklyProgress) const
{
	mHUDWidget->UpdateDailyProgress(_dailyProgress, _weeklyProgress);
}

void APlanetHUD::OnCurrentDayChanged(EPlanetDayOfWeek _newDay) const
{
	mHUDWidget->UpdateCurrentDayTextBlock(_newDay);
}

void APlanetHUD::ShowDashLine(bool _bShow)
{
	check(mDashLineWidget);

	bDashLineVisible = _bShow;

	if (_bShow)
	{
		mDashLineWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		mDashLineWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlanetHUD::UpdateDashLineDirection(const FVector& _direction) const
{
	check(mDashLineRootWidget);

	const float rotationAngle = FMath::RadiansToDegrees(FMath::Atan2(_direction.Y, _direction.X));
	
	FWidgetTransform transform = mDashLineRootWidget->GetRenderTransform();
	transform.Angle = rotationAngle;
	mDashLineRootWidget->SetRenderTransform(transform);
}