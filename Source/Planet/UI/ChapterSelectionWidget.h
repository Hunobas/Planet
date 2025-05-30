// ChapterSelectionWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterSelectionWidget.generated.h"

class UButton;

UCLASS()
class PLANET_API UChapterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UChapterSelectionWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnLevel01Clicked();
	UFUNCTION()
	void OnLevel02Clicked();
	UFUNCTION()
	void OnLevel03Clicked();

	UFUNCTION()
	void OnPowerButtonClicked();
	UFUNCTION()
	void OnSettingsButtonClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Level01Button;
	UPROPERTY(meta = (BindWidget)) 
	UButton* Level02Button;
	UPROPERTY(meta = (BindWidget))
	UButton* Level03Button;

	UPROPERTY(meta = (BindWidget))
	UButton* PowerButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

private:
	void bindButtons();
	void loadLevel(const FString& _levelPath) const;
	static void showNotImplementedMessage();
};
