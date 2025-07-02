// HUDWidget.cpp
#include "HUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameFramework/PlayerController.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "WeaponPawn.h"
#include "PassiveItem.h"
#include "DayOfWeekComponent.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	cPlayerPawn = Cast<APlanetPawn>(GetOwningPlayerPawn());
	checkf(cPlayerPawn, TEXT("[HUDWidget] PlayerPawn을 찾을 수 없습니다."));

	GetWorld()->GetTimerManager().SetTimer(
		mGameTimeUpdateTimer,
		this,
		&UHUDWidget::updateGameTimeDisplay,
		1.0f,
		true,
		0.0f
	);
}

void UHUDWidget::NativeTick(const FGeometry& _myGeometry, float _deltaTime)
{
	Super::NativeTick(_myGeometry, _deltaTime);

	if(cPlayerPawn)
	{
		UpdateRotationBarAlpha(_deltaTime);
	}
}

void UHUDWidget::UpdateHP(const float _currentHP, const float _maxHP) const
{
	if (!HPProgressBar || !CurrentHPOverMaxHp)
		return;
    
	const float progressPercent = _currentHP / FMath::Max(_maxHP, 1.0f);
	HPProgressBar->SetPercent(Saturate(progressPercent));

	const FString hpText = FString::Printf(TEXT("%.0f/%.0f"), _currentHP, _maxHP);
	CurrentHPOverMaxHp->SetText(FText::FromString(hpText));
}

void UHUDWidget::UpdateShield(const float _currentShield, const float _currentHP, const float _maxHP) const
{
	if (!ShieldProgressBar || !CurrentHPOverMaxHp)
		return;
    
	const float progressPercent = _currentShield / FMath::Max(_maxHP, 1.0f);
	ShieldProgressBar->SetPercent(Saturate(progressPercent));

	const FString hpText = FString::Printf(TEXT("%.0f/%.0f"), _currentHP + _currentShield, _maxHP);
	CurrentHPOverMaxHp->SetText(FText::FromString(hpText));
}

void UHUDWidget::UpdateXPProgress(const float _currentXP, const float _xpToNextLevel) const
{
	if (!XPProgressBar)
		return;
    
	const float progressPercent = _currentXP / FMath::Max(_xpToNextLevel, 1.0f);
	XPProgressBar->SetPercent(Saturate(progressPercent));
}

void UHUDWidget::UpdateCurrentLevel(const int32 _level) const
{
	if (!CurrentLevel)
		return;
    
	const FText levelText = FText::FromString(FString::FromInt(_level));
	CurrentLevel->SetText(levelText);
}

void UHUDWidget::UpdateAllWeaponIcons(const TArray<AWeaponPawn*>& _equippedWeapons) const
{
	for (int32 i = 0; i < _equippedWeapons.Num(); i++)
	{
		if (_equippedWeapons[i])
		{
			SetWeaponIconBySlot(i, _equippedWeapons[i]->WeaponIcon);
		}
		else
		{
			SetWeaponIconBySlot(i, nullptr);
		}
	}
}

void UHUDWidget::SetWeaponIconBySlot(const int32 _slotIndex, UTexture2D* _weaponTexture) const
{
	UImage* weaponImage = getWeaponImageBySlot(_slotIndex);
	if (!weaponImage)
		return;
    
	if (_weaponTexture)
	{
		weaponImage->SetBrushFromTexture(_weaponTexture);
		weaponImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		weaponImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::UpdateAllItemIcons(const TArray<APassiveItem*>& _equippedItems) const
{
	for (int32 i = 0; i < _equippedItems.Num(); i++)
	{
		if (_equippedItems[i])
		{
			SetItemIconBySlot(i, _equippedItems[i]->ItemIcon);
		}
		else
		{
			SetItemIconBySlot(i, nullptr);
		}
	}
}

void UHUDWidget::SetItemIconBySlot(const int32 _slotIndex, UTexture2D* _itemTexture) const
{
	UImage* itemImage = getItemImageBySlot(_slotIndex);
	if (!itemImage)
		return;
    
	if (_itemTexture)
	{
		itemImage->SetBrushFromTexture(_itemTexture);
		itemImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		itemImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::UpdateDailyProgress(const float _dailyProgress, const float _weeklyProgress) const
{
	if (!DailyProgressBar || !WeeklyProgressBar)
		return;

	const float saturatedDailyProgress = Saturate(_dailyProgress);
	DailyProgressBar->SetPercent(saturatedDailyProgress);
	RotationSlowBar->SetPercent(saturatedDailyProgress);
	RotationFastBar->SetPercent(saturatedDailyProgress);
	WeeklyProgressBar->SetPercent(Saturate(_weeklyProgress));
}

void UHUDWidget::UpdateRotationBarAlpha(float _deltaTime) const
{
	if(!RotationSlowBar || !RotationFastBar || !cPlayerPawn)
		return;

	check(cPlayerPawn->DayOfWeek);
	const float spendAngle = cPlayerPawn->DayOfWeek->SpendAngle;

	if(spendAngle > -EPSILON && spendAngle < EPSILON)
	{
		RotationFastBar->SetRenderOpacity(0.0f);
		RotationSlowBar->SetRenderOpacity(0.0f);
	}
	else if(spendAngle >= EPSILON)
	{
		RotationFastBar->SetRenderOpacity(0.8f);
	}
	else
	{
		RotationSlowBar->SetRenderOpacity(0.8f);
	}
}

void UHUDWidget::UpdateCurrentDayTextBlock(EPlanetDayOfWeek _currentDay) const
{
	if (MON) MON->SetVisibility(ESlateVisibility::Hidden);
	if (TUE) TUE->SetVisibility(ESlateVisibility::Hidden);
	if (WED) WED->SetVisibility(ESlateVisibility::Hidden);
	if (THU) THU->SetVisibility(ESlateVisibility::Hidden);
	if (FRI) FRI->SetVisibility(ESlateVisibility::Hidden);
	if (SAT) SAT->SetVisibility(ESlateVisibility::Hidden);
	if (SUN) SUN->SetVisibility(ESlateVisibility::Hidden);

	if (UTextBlock* currentDayText = getDayTextByDayOfWeek(_currentDay))
	{
		currentDayText->SetVisibility(ESlateVisibility::Visible);
	}
}

UImage* UHUDWidget::getWeaponImageBySlot(const int32 _slotIndex) const
{
	check(_slotIndex < WEAPON_MAX_SLOT)
	
	switch (_slotIndex)
	{
		case 0: return Weapon1;
		case 1: return Weapon2;
		case 2: return Weapon3;
		case 3: return Weapon4;
		case 4: return Weapon5;
		case 5: return Weapon6;
		default:
			checkNoEntry();
			return nullptr;
	}
}

UImage* UHUDWidget::getItemImageBySlot(const int32 _slotIndex) const
{
	check(_slotIndex < ITEM_MAX_SLOT)
	
	switch (_slotIndex)
	{
		case 0: return Item1;
		case 1: return Item2;
		case 2: return Item3;
		case 3: return Item4;
		case 4: return Item5;
		case 5: return Item6;
		default:
			checkNoEntry();
			return nullptr;
	}
}

UTextBlock* UHUDWidget::getDayTextByDayOfWeek(const EPlanetDayOfWeek& _dayOfWeek) const
{
	switch (_dayOfWeek)
	{
		case EPlanetDayOfWeek::Monday: return MON;
		case EPlanetDayOfWeek::Tuesday: return TUE;
		case EPlanetDayOfWeek::Wednesday: return WED;
		case EPlanetDayOfWeek::Thursday: return THU;
		case EPlanetDayOfWeek::Friday: return FRI;
		case EPlanetDayOfWeek::Saturday: return SAT;
		case EPlanetDayOfWeek::Sunday: return SUN;
		default:
		checkNoEntry();
		return nullptr;
	}
}

void UHUDWidget::updateGameTimeDisplay() const
{
	if (GameTime)
	{
		float totalSeconds = UGameplayStatics::GetTimeSeconds(GetWorld());
		
		int32 minutes = FMath::FloorToInt(totalSeconds * TIMER_DURATION_RATIO);
		int32 seconds = FMath::FloorToInt(FMath::Fmod(totalSeconds, TIMER_DURATION));
		FString timeString = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
		
		GameTime->SetText(FText::FromString(timeString));
	}
}