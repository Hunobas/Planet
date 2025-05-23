// RewardManager.cpp
#include "RewardManager.h"

#include "PlanetPawn.h"
#include "RewardSelector.h"
#include "WeaponRewardApplicator.h"
#include "PassiveItemRewardApplicator.h"
#include "PassiveItemSlotComponent.h"
#include "PlayerPowerUpRewardApplicator.h"
#include "WeaponSlotComponent.h"

URewardManager::URewardManager() : cOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	RewardSelector = CreateDefaultSubobject<URewardSelector>(TEXT("Reward Selector"));
}

void URewardManager::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	RewardSelector->Initialize(this);
}

TArray<TScriptInterface<IRewardData>> URewardManager::GetAvailableRewards(int32 RewardCount)
{
	return RewardSelector->SelectRewards(RewardCount);
}

void URewardManager::ApplyReward(const TScriptInterface<IRewardData>& Reward)
{
	if (const IRewardData* RewardData = Reward.GetInterface())
	{
		FString ApplicatorKey = RewardData->GetApplicatorType();
		if (mApplicators.Contains(ApplicatorKey))
		{
			mApplicators[ApplicatorKey]->Apply(Reward);
			OnRewardApplied.Broadcast(Reward);
		}
	}
}

int32 URewardManager::GetRemainWeaponSlots() const
{
	return cOwner->WeaponSlot->RemainSlots;
}

int32 URewardManager::GetRemainPassiveItemSlots() const
{
	return cOwner->ItemSlot->RemainSlots;
}

FText URewardManager::getLocalizedText(const FString& TextKey, const FString& Category) const
{
	// static const FString ContextString(TEXT("RewardText"));
	// if (RewardTextDataTable)
	// {
	// 	FName RowName = FName(*FString::Printf(TEXT("%s_%s"), *Category, *TextKey));
	// 	return RewardTextDataTable->FindRow<FRewardTextData>(RowName, ContextString)->DisplayName;
	// }
	return FText::GetEmpty();
}

void URewardManager::loadWeaponConfigs()
{
	// static const FString Context(TEXT("Weapon Config"));
	// if (WeaponConfigDataTable)
	// {
	// 	TArray<FWeaponConfigData*> Rows;
	// 	WeaponConfigDataTable->GetAllRows<FWeaponConfigData>(Context, Rows);
 //        
	// 	for (auto& Row : Rows)
	// 	{
	// 		UWeaponRewardData* NewData = NewObject<UWeaponRewardData>(this);
	// 		NewData->InitializeFromConfig(Row);
	// 		AllRewards.Add(NewData->GetIdentifier(), NewData);
	// 	}
	// }
}
