// MainMenuGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class PLANET_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UChapterSelectionWidget> ChapterWidgetClass;

private:
    void InitializeUI() const;
};