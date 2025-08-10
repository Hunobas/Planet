#include "MainMenuGameMode.h"

#include "ChapterSelectionWidget.h"
#include "Blueprint/UserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = APlayerController::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeUI();
}

void AMainMenuGameMode::InitializeUI() const
{
	if (ChapterWidgetClass)
	{
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				// TODO: 처음 뜨는 위젯 UChapterSelectionWidget에서 MainMenu 위젯으로 바꿔야 함.
				UChapterSelectionWidget* Widget = CreateWidget<UChapterSelectionWidget>(PC, ChapterWidgetClass);
				if (Widget)
				{
					Widget->AddToViewport();
                    
					PC->SetInputMode(FInputModeUIOnly());
					PC->SetShowMouseCursor(true);
				}
			}
		}
	}
}
