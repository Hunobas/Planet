// RewardSelectionWidget.cpp
#include "RewardSelectionWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "../Planet.h"
#include "RewardManager.h"
#include "IRewardData.h"
#include "RewardTextDataTable.h"
#include "PlanetPawn.h"

void URewardSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
	if (APlanetPawn* Owner = Cast<APlanetPawn>(GetOwningPlayerPawn()))
	{
		cRewardManager = Owner->FindComponentByClass<URewardManager>();
	}
    
	if (RewardButton1) 
		RewardButton1->OnClicked.AddUniqueDynamic(this, &URewardSelectionWidget::OnRewardButton1Clicked);
	if (RewardButton2) 
		RewardButton2->OnClicked.AddUniqueDynamic(this, &URewardSelectionWidget::OnRewardButton2Clicked);
	if (RewardButton3) 
		RewardButton3->OnClicked.AddUniqueDynamic(this, &URewardSelectionWidget::OnRewardButton3Clicked);
        
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void URewardSelectionWidget::DisplayRewards(const TArray<TScriptInterface<IRewardData>>& Rewards)
{
	mCurrentRewards = Rewards;
    
	TArray<UButton*> Buttons = { RewardButton1, RewardButton2, RewardButton3 };
    
	for (int32 i = 0; i < FMath::Min(Buttons.Num(), Rewards.Num()); ++i)
	{
		if (Rewards.IsValidIndex(i))
		{
			SetupRewardButton(Buttons[i], Rewards[i], i);
		}
	}
    
	for (int32 i = Rewards.Num(); i < Buttons.Num(); ++i)
	{
		if (Buttons[i])
		{
			Buttons[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URewardSelectionWidget::SetupRewardButton(UButton* Button, const TScriptInterface<IRewardData>& Reward, int32 ButtonIndex) const
{
    if (!Button || !Reward.GetInterface())
    	return;
    
    const IRewardData* RewardData = Reward.GetInterface();
    FName RewardIdentifier = RewardData->GetRewardIdentifier();
    
    const FRewardTextData* TextData = GetLocalizedTextData(RewardIdentifier);
    if (!TextData)
    	return;
    
    FString IconWidgetName = FString::Printf(TEXT("%s%d"), *REWARD_ICON , ButtonIndex + 1);
    FString NameWidgetName = FString::Printf(TEXT("%s%d"), *REWARD_NAME,  ButtonIndex + 1);
    FString DescWidgetName = FString::Printf(TEXT("%s%d"), *REWARD_DESC, ButtonIndex + 1);
    
    UImage* IconImage = Cast<UImage>(GetWidgetFromName(*IconWidgetName));
    UTextBlock* NameText = Cast<UTextBlock>(GetWidgetFromName(*NameWidgetName));
    UTextBlock* DescText = Cast<UTextBlock>(GetWidgetFromName(*DescWidgetName));
    
    if (IconImage && RewardData->GetRewardIcon())
    {
        IconImage->SetBrushFromTexture(RewardData->GetRewardIcon());
    }
    
    if (NameText)
    {
        NameText->SetText(TextData->DisplayName);
    }
    
    if (DescText)
    {
        FText NextLevelDesc = GetNextLevelDescription(TextData, RewardData->GetCurrentLevel());
        DescText->SetText(NextLevelDesc);
    }
    
    Button->SetVisibility(ESlateVisibility::Visible);
}

FRewardTextData* URewardSelectionWidget::GetLocalizedTextData(FName RewardIdentifier) const
{
    UDataTable* const* DataTablePtr = LocalizedDataTables.Find(CurrentLanguage);
    if (!DataTablePtr || !*DataTablePtr)
    	return nullptr;
    
    return (*DataTablePtr)->FindRow<FRewardTextData>(RewardIdentifier, TEXT("RewardSelection"));
}

FText URewardSelectionWidget::GetNextLevelDescription(const FRewardTextData* TextData, int32 NextLevel) const
{
    if (!TextData)
    	return FText::GetEmpty();
    
    TArray<FText> LevelDescs = TextData->LevelDescriptions;
    
    if (LevelDescs.IsValidIndex(NextLevel))
    {
        return LevelDescs[NextLevel];
    }
    
    return TextData->Description;
}

void URewardSelectionWidget::OnRewardButton1Clicked()
{
    HandleRewardSelection(0);
}

void URewardSelectionWidget::OnRewardButton2Clicked()
{
    HandleRewardSelection(1);
}

void URewardSelectionWidget::OnRewardButton3Clicked()
{
    HandleRewardSelection(2);
}

void URewardSelectionWidget::HandleRewardSelection(int32 ButtonIndex)
{
    if (!mCurrentRewards.IsValidIndex(ButtonIndex) || !cRewardManager)
    	return;
    
    const TScriptInterface<IRewardData>& SelectedReward = mCurrentRewards[ButtonIndex];
    cRewardManager->ApplyReward(SelectedReward);
    
    CloseRewardSelection();
}

void URewardSelectionWidget::CloseRewardSelection()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    RemoveFromParent();
}