// HUDWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class APlanetPawn;
class AWeaponPawn;

UCLASS()
class PLANET_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateXPProgress(const float& _currentXP, const float& _xpToNextLevel) const;
	void UpdateCurrentLevel(const int32& _level) const;
	void UpdateHP(const float& _currentHP, const float& _maxHP) const;
	void UpdateAllWeaponIcons(const TArray<AWeaponPawn*>& _equippedWeapons) const;
	void SetWeaponIconBySlot(const int32& _slotIndex, UTexture2D* _weaponTexture) const;
	void UpdateAllItemIcons(const TArray<UObject*>& _equippedItems) const;
	void SetItemIconBySlot(const int32& _slotIndex, UTexture2D* _itemTexture) const;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* HPProgressBar;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* CurrentHPOverMaxHp;
	
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* XPProgressBar;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* CurrentLevel;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon1;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon2;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon3;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon4;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon5;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Weapon6;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item1;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item2;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item3;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item4;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item5;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UImage* Item6;

private:
	UImage* getWeaponImageBySlot(int32 _slotIndex) const;
	UImage* getItemImageBySlot(int32 _slotIndex) const;
	
	APlanetPawn* cPlayerPawn;
};