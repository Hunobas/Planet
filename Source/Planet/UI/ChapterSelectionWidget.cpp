// ChapterSelectionWidget.cpp
#include "ChapterSelectionWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UChapterSelectionWidget::UChapterSelectionWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), Level01Button(nullptr), Level02Button(nullptr), Level03Button(nullptr),
      PowerButton(nullptr),
      SettingsButton(nullptr)
{
}

void UChapterSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();
    bindButtons();
}

void UChapterSelectionWidget::OnLevel01Clicked()
{
    loadLevel(TEXT("/Game/Levels/Level_1"));
}

void UChapterSelectionWidget::OnLevel02Clicked()
{
    showNotImplementedMessage();
}

void UChapterSelectionWidget::OnLevel03Clicked()
{
    showNotImplementedMessage();
}

void UChapterSelectionWidget::OnPowerButtonClicked()
{
    if (const UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }
    }
}

void UChapterSelectionWidget::OnSettingsButtonClicked()
{
    showNotImplementedMessage();
}

void UChapterSelectionWidget::bindButtons()
{
    if (Level01Button)
        Level01Button->OnClicked.AddUniqueDynamic(this, &UChapterSelectionWidget::OnLevel01Clicked);
    if (Level02Button)
        Level02Button->OnClicked.AddUniqueDynamic(this, &UChapterSelectionWidget::OnLevel02Clicked);
    if (Level03Button)
        Level03Button->OnClicked.AddUniqueDynamic(this, &UChapterSelectionWidget::OnLevel03Clicked);
    if (PowerButton)
        PowerButton->OnClicked.AddUniqueDynamic(this, &UChapterSelectionWidget::OnPowerButtonClicked);
    if (SettingsButton)
        SettingsButton->OnClicked.AddUniqueDynamic(this, &UChapterSelectionWidget::OnSettingsButtonClicked);
}

void UChapterSelectionWidget::loadLevel(const FString& _levelPath) const
{
    if (const UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, FName(*_levelPath));
    }
}

void UChapterSelectionWidget::showNotImplementedMessage()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("아직 구현되지 않은 기능입니다."));
    }
}
