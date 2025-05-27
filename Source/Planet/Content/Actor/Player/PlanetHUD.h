// PlanetHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlanetHUD.generated.h"

class UUserWidget;
class APlanetPawn;
class UHUDWidget;
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

    void OnXPGain(const float& _currentXP, const float& _xpToNextLevel) const;
    void OnLevelUp(const int32& _currentLevel) const;
    void OnHPChanged(const float& _currentHP, const float& _maxHP) const;
    void OnWeaponSlotChanged(const TArray<AWeaponPawn*>& _equippedWeapons) const;
    void OnItemSlotChanged(const TArray<APassiveItem*>& _equippedItems) const;

    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> HUDClass;
    
    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> RewardSelectionClass;

private:
    void beginLatePlay() const;
    
    APlanetPawn* cPlayerPawn;
    UHUDWidget* mHUDWidget;
};
