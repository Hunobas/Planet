// PlanetHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlanetHUD.generated.h"

enum class EPlanetDayOfWeek : uint8;
class UUserWidget;
class APlanetPawn;
class APlanetController;
class UHUDWidget;
class UCanvasPanel;
class AWeaponPawn;
class APassiveItem;

UCLASS()
class PLANET_API APlanetHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Reward")
    void ShowRewardSelection();

    void OnHPChanged(const float& _currentHP, const float& _maxHP) const;
    void OnShieldChanged(const float& _currentShield,const float _currentHP, const float& _maxHP) const;
    void OnXPGain(const float& _currentXP, const float& _xpToNextLevel) const;
    void OnLevelUp(const int32& _currentLevel) const;
    void OnWeaponSlotChanged(const TArray<AWeaponPawn*>& _equippedWeapons) const;
    void OnItemSlotChanged(const TArray<APassiveItem*>& _equippedItems) const;
    void OnDailyProgressChanged(const float _dailyProgress, const float _weeklyProgress) const;
    void OnCurrentDayChanged(EPlanetDayOfWeek _newDay) const;
    void ShowWinGame();
    void ShowLoseGame();

    void ShowDashLine(bool _bShow);
    void UpdateDashLineDirection() const;

    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> HUDClass;
    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> RewardSelectionClass;
    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> DashLineClass;

    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> LoseGameClass;
    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> WinGameClass;

    bool bDashLineVisible;

private:
    void beginLatePlay() const;
    
    APlanetPawn* cPlayerPawn;
    APlanetController* cPlayerController;
    UHUDWidget* mHUDWidget;
    UUserWidget* mDashLineWidget;
    UCanvasPanel* mDashLineRootWidget;
    UUserWidget* mWinGameWidget;
    UUserWidget* mLoseGameWidget;
};
