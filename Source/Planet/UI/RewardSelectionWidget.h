// RewardSelectionWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardSelectionWidget.generated.h"

struct FRewardTextData;
class IRewardData;
class URewardManager;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PLANET_API URewardSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void DisplayRewards(const TArray<TScriptInterface<IRewardData>>& Rewards);

	UFUNCTION()
	void OnRewardButton1Clicked();
	UFUNCTION()
	void OnRewardButton2Clicked();
	UFUNCTION()
	void OnRewardButton3Clicked();

	UPROPERTY(EditAnywhere, Category = "Localization")
	FString CurrentLanguage = TEXT("KOR");
	UPROPERTY(EditAnywhere, Category = "Localization")
	TMap<FString, UDataTable*> LocalizedDataTables;

	UPROPERTY(meta = (BindWidget))
	UButton* RewardButton1;
	UPROPERTY(meta = (BindWidget))
	UButton* RewardButton2;
	UPROPERTY(meta = (BindWidget))
	UButton* RewardButton3;

private:
	void SetupRewardButton(UButton* Button, const TScriptInterface<IRewardData>& Reward, int32 ButtonIndex) const;
	void HandleRewardSelection(int32 ButtonIndex);
	void CloseRewardSelection();
    
	FRewardTextData* GetLocalizedTextData(FName RewardIdentifier) const;
	FText GetNextLevelDescription(const FRewardTextData* TextData, int32 CurrentLevel) const;

	URewardManager* cRewardManager;
	TArray<TScriptInterface<IRewardData>> mCurrentRewards;
};
