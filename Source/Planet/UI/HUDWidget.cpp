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

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	cPlayerPawn = Cast<APlanetPawn>(GetOwningPlayerPawn());
	checkf(cPlayerPawn, TEXT("[HUDWidget] PlayerPawn을 찾을 수 없습니다."));
}

void UHUDWidget::UpdateXPProgress(const float& _currentXP, const float& _xpToNextLevel) const
{
	if (!XPProgressBar)
		return;
    
	const float progressPercent = _xpToNextLevel > 0.0f ? _currentXP / _xpToNextLevel : 0.0f;
	XPProgressBar->SetPercent(FMath::Clamp(progressPercent, 0.0f, 1.0f));
}

void UHUDWidget::UpdateCurrentLevel(const int32& _level) const
{
	if (!CurrentLevel)
		return;
    
	const FText levelText = FText::FromString(FString::FromInt(_level));
	CurrentLevel->SetText(levelText);
}

void UHUDWidget::UpdateHP(const float& _currentHP, const float& _maxHP) const
{
	if (!HPProgressBar || !CurrentHPOverMaxHp)
		return;
    
	const float progressPercent = _maxHP > 0.0f ? _currentHP / _maxHP : 0.0f;
	HPProgressBar->SetPercent(FMath::Clamp(progressPercent, 0.0f, 1.0f));

	const FString hpText = FString::Printf(TEXT("%.0f/%.0f"), _currentHP, _maxHP);
	CurrentHPOverMaxHp->SetText(FText::FromString(hpText));
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

void UHUDWidget::SetWeaponIconBySlot(const int32& _slotIndex, UTexture2D* _weaponTexture) const
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

void UHUDWidget::SetItemIconBySlot(const int32& _slotIndex, UTexture2D* _itemTexture) const
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

UImage* UHUDWidget::getWeaponImageBySlot(int32 _slotIndex) const
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

UImage* UHUDWidget::getItemImageBySlot(int32 _slotIndex) const
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
