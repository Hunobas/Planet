// PlanetHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Planet.h"
#include "PlanetHUD.generated.h"

class UCursorWidget;
class APlanetPawn;

UCLASS()
class PLANET_API APlanetHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Reward")
    void ShowRewardSelection();

    UPROPERTY(EditAnywhere, Category = "HUD")
    TEnumAsByte<EAutoReceiveInput::Type> TargetPlayer = SINGLEPLAYER_INDEX;

    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UCursorWidget> CursorWidgetClass;
    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UUserWidget> RewardSelectionClass;

private:
    APlanetPawn* cPlayerPawn;
    
    UCursorWidget* mCursorWidget;
};
