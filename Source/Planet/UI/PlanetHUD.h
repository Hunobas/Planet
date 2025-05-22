// PlanetHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlanetHUD.generated.h"

class UCursorWidget;

UCLASS()
class PLANET_API APlanetHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "UI Component")
    TSubclassOf<UCursorWidget> CursorWidgetClass;

private:
    UCursorWidget* mCursorWidget;
};
