// RewardManager.cpp
#include "RewardManager.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "RewardSelector.h"
#include "WeaponRewardData.h"
#include "PassiveItemRewardData.h"
#include "PlayerPowerUpRewardData.h"
#include "WeaponRewardApplicator.h"
#include "PassiveItemRewardApplicator.h"
#include "PlayerPowerUpRewardApplicator.h"

URewardManager::URewardManager() : cOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	mRewardSelector = CreateDefaultSubobject<URewardSelector>(TEXT("Reward Selector"));
}

void URewardManager::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	initializeApplicators();
	createAllRewardInstances();
	mRewardSelector->Initialize(this, cOwner);
}

TArray<TScriptInterface<IRewardData>> URewardManager::GetAvailableRewards(const int32& RewardCount)
{
	return mRewardSelector->SelectRewards(RewardCount);
}

void URewardManager::ApplyReward(const TScriptInterface<IRewardData>& Reward)
{
	if (const IRewardData* RewardData = Reward.GetInterface())
	{
		FString FullIdentifier = RewardData->GetRewardIdentifier().ToString();
        
		int32 SplitIndex;
		if (FullIdentifier.FindChar(TEXT('_'), SplitIndex))
		{
			FString ApplicatorKey = FullIdentifier.Left(SplitIndex);
			
            checkf(mApplicators.Contains(ApplicatorKey), TEXT("Invalid applicator key: %s"), *FullIdentifier);
			mApplicators[ApplicatorKey]->Apply(Reward, cOwner);
			
			OnRewardApplied.Broadcast(Reward);
		}
		else
		{
			checkf(false, TEXT("Invalid reward identifier format: %s"), *FullIdentifier);
		}
	}
}

void URewardManager::initializeApplicators()
{
	UWeaponRewardApplicator* WeaponApplicator = NewObject<UWeaponRewardApplicator>(this, TEXT("Weapon Applicator"));
	UPassiveItemRewardApplicator* PassiveItemApplicator = NewObject<UPassiveItemRewardApplicator>(this, TEXT("Passive Item Applicator"));
	UPlayerPowerUpRewardApplicator* PowerUpApplicator = NewObject<UPlayerPowerUpRewardApplicator>(this, TEXT("Power Up Applicator"));

	mApplicators.Add(WEAPON_REWARD_TAG, WeaponApplicator);
	mApplicators.Add(PASSIVEITEM_REWARD_TAG, PassiveItemApplicator);
	mApplicators.Add(POWERUP_REWARD_TAG, PowerUpApplicator);

	WeaponApplicator->Rename(nullptr, this);
	PassiveItemApplicator->Rename(nullptr, this);
	PowerUpApplicator->Rename(nullptr, this);
}

void URewardManager::createAllRewardInstances()
{
	for (const auto& WeaponClass : AllWeaponRewards)
	{
		if (auto* Weapon = NewObject<UWeaponRewardData>(this, WeaponClass))
		{
			CachedWeaponInstances.Add(Weapon);
		}
	}

	for (const auto& ItemClass : AllPassiveItemRewards)
	{
		if (auto* Item = NewObject<UPassiveItemRewardData>(this, ItemClass))
		{
			CachedPassiveItemInstances.Add(Item);
		}
	}

	for (const auto& PowerUpClass : AllPlayerPowerUpRewards)
	{
		if (auto* PowerUp = NewObject<UPlayerPowerUpRewardData>(this, PowerUpClass))
		{
			CachedPowerUpInstances.Add(PowerUp);
		}
	}
}
