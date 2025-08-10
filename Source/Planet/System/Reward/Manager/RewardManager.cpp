// RewardManager.cpp
#include "RewardManager.h"

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
}

void URewardManager::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	mRewardSelector = NewObject<URewardSelector>(this, TEXT("Reward Selector"));
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
	const IRewardData* RewardData = Reward.GetInterface();
	check(RewardData);
	FString FullIdentifier = RewardData->GetRewardIdentifier().ToString();
    
	int32 SplitIndex;
	if (FullIdentifier.FindChar(TEXT('_'), SplitIndex))
	{
		FString ApplicatorKey = FullIdentifier.Left(SplitIndex);
		
        checkf(mApplicators.Contains(ApplicatorKey), TEXT("Invalid applicator key: %s"), *FullIdentifier);
		mApplicators[ApplicatorKey]->Apply_Implementation(Reward, cOwner);
	}
	else
	{
		checkf(false, TEXT("Invalid reward identifier format: %s"), *FullIdentifier);
	}
}

void URewardManager::initializeApplicators()
{
	UWeaponRewardApplicator* WeaponApplicator = NewObject<UWeaponRewardApplicator>(this, TEXT("Weapon Applicator"));
	UPassiveItemRewardApplicator* PassiveItemApplicator = NewObject<UPassiveItemRewardApplicator>(this, TEXT("Passive Item Applicator"));
	UPlayerPowerUpRewardApplicator* PowerUpApplicator = NewObject<UPlayerPowerUpRewardApplicator>(this, TEXT("Power Up Applicator"));

	mApplicators.Add(WEAPON_REWARD_TAG, WeaponApplicator);
	mApplicators.Add(ITEM_REWARD_TAG, PassiveItemApplicator);
	mApplicators.Add(POWERUP_REWARD_TAG, PowerUpApplicator);

	WeaponApplicator->Rename(nullptr, this);
	PassiveItemApplicator->Rename(nullptr, this);
	PowerUpApplicator->Rename(nullptr, this);
}

void URewardManager::createAllRewardInstances()
{
	CachedWeaponInstances.Reset();
	CachedPassiveItemInstances.Reset();
	CachedPowerUpInstances.Reset();
	
	for (const TSubclassOf<UWeaponRewardData>& WeaponClass : AllWeaponRewards)
	{
		if (UWeaponRewardData* Weapon = NewObject<UWeaponRewardData>(this, WeaponClass))
		{
			CachedWeaponInstances.Add(Weapon);
		}
	}

	for (const TSubclassOf<UPassiveItemRewardData>& ItemClass : AllPassiveItemRewards)
	{
		if (UPassiveItemRewardData* Item = NewObject<UPassiveItemRewardData>(this, ItemClass))
		{
			CachedPassiveItemInstances.Add(Item);
		}
	}

	for (const TSubclassOf<UPlayerPowerUpRewardData>& PowerUpClass : AllPlayerPowerUpRewards)
	{
		if (UPlayerPowerUpRewardData* PowerUp = NewObject<UPlayerPowerUpRewardData>(this, PowerUpClass))
		{
			CachedPowerUpInstances.Add(PowerUp);
		}
	}
}
