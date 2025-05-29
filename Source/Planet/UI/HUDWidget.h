// HUDWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

enum class EPlanetDayOfWeek : uint8;
class UProgressBar;
class UTextBlock;
class UImage;
class APlanetPawn;
class AWeaponPawn;
class APassiveItem;

UCLASS()
class PLANET_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHP(const float _currentHP, const float _maxHP) const;
	void UpdateShield(const float _currentShield, const float _currentHP, const float _maxHP) const;
	void UpdateXPProgress(const float _currentXP, const float _xpToNextLevel) const;
	void UpdateCurrentLevel(const int32 _level) const;
	void UpdateAllWeaponIcons(const TArray<AWeaponPawn*>& _equippedWeapons) const;
	void SetWeaponIconBySlot(const int32 _slotIndex, UTexture2D* _weaponTexture) const;
	void UpdateAllItemIcons(const TArray<APassiveItem*>& _equippedItems) const;
	void SetItemIconBySlot(const int32 _slotIndex, UTexture2D* _itemTexture) const;
	void UpdateDailyProgress(const float _dailyProgress, const float _weeklyProgress) const;
	void UpdateCurrentDayTextBlock(EPlanetDayOfWeek _currentDay) const;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* HPProgressBar;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* CurrentHPOverMaxHp;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* ShieldProgressBar;
	
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

	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* DailyProgressBar;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UProgressBar* WeeklyProgressBar;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* MON;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* TUE;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* WED;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* THU;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* FRI;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* SAT;
	UPROPERTY(EditAnywhere, Category = "UI", meta = (BindWidget))
	UTextBlock* SUN;

private:
	UImage* getWeaponImageBySlot(const int32 _slotIndex) const;
	UImage* getItemImageBySlot(const int32 _slotIndex) const;
	UTextBlock* getDayTextByDayOfWeek(const EPlanetDayOfWeek& _dayOfWeek) const;
	
	APlanetPawn* cPlayerPawn;
};