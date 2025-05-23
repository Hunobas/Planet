// RewardSelector.cpp
#include "RewardSelector.h"

#include "RewardManager.h"
#include "WeaponRewardData.h"
#include "PassiveItemRewardData.h"

void URewardSelector::Initialize(URewardManager* Manager)
{
    mManager = Manager;
    updateAvailablePools();
}

TArray<TScriptInterface<IRewardData>> URewardSelector::SelectRewards(int32 RequestedCount)
{
    updateAvailablePools();

    TArray<TScriptInterface<IRewardData>> SelectedRewards;
    while (SelectedRewards.Num() < RequestedCount && hasAvailableRewards())
    {
        if (auto Reward = selectRandomRewardOrNull())
        {
            SelectedRewards.Add(Reward);
            removeFromAvailablePool(Reward);
        }
        else
        {
            break;
        }
    }
    return SelectedRewards;
}

void URewardSelector::updateAvailablePools()
{
    mAvailableWeapons.Empty();
    mAvailablePassiveItems.Empty();
    mAvailablePowerUps.Empty();

    for (auto& Reward : mManager->AllWeaponRewards)
    {
        UWeaponRewardData* WeaponReward = Cast<UWeaponRewardData>(Reward.GetObject());
        if (WeaponReward && !WeaponReward->IsMaxLevel())
            mAvailableWeapons.Add(WeaponReward);
    }
    RemainWeaponSlots = mManager->GetRemainWeaponSlots();

    for (auto& Reward : mManager->AllPassiveItemRewards)
    {
        UPassiveItemRewardData* ItemReward = Cast<UPassiveItemRewardData>(Reward.GetObject());
        if (ItemReward && !ItemReward->IsMaxLevel())
            mAvailablePassiveItems.Add(ItemReward);
    }
    RemainPassiveItemSlots = mManager->GetRemainPassiveItemSlots();

    for (auto& Reward : mManager->AllPlayerPowerUpRewards)
    {
        mAvailablePowerUps.Add(Reward);
    }
}

bool URewardSelector::hasAvailableRewards() const
{
    return mAvailableWeapons.Num() > 0 || mAvailablePassiveItems.Num() > 0 || mAvailablePowerUps.Num() > 0;
}

TScriptInterface<IRewardData> URewardSelector::selectRandomRewardOrNull()
{
    const bool bWeaponAvailable = RemainWeaponSlots > 0 && mAvailableWeapons.Num() > 0;
    const bool bPassiveAvailable = RemainPassiveItemSlots > 0 && mAvailablePassiveItems.Num() > 0;
    const bool bPowerUpAvailable = mAvailablePowerUps.Num() > 0;

    float totalProb = 0.f;
    if (bWeaponAvailable) totalProb += WeaponAppearanceRate;
    if (bPassiveAvailable) totalProb += PassiveItemAppearanceRate;
    if (bPowerUpAvailable) totalProb += (1.f - WeaponAppearanceRate - PassiveItemAppearanceRate);

    if (totalProb <= 0.f) return nullptr;

    float RandomValue = FMath::FRandRange(0.f, totalProb);

    float Accum = 0.0f;
    if (bWeaponAvailable)
    {
        Accum += WeaponAppearanceRate;
        if (RandomValue < Accum)
            return selectRandomWeapon();
    }
    if (bPassiveAvailable)
    {
        Accum += PassiveItemAppearanceRate;
        if (RandomValue < Accum)
            return selectRandomPassiveItem();
    }
    if (bPowerUpAvailable)
    {
        return selectRandomPowerUp();
    }
    return nullptr;
}

TScriptInterface<IRewardData> URewardSelector::selectRandomWeapon()
{
    int32 Index = FMath::RandRange(0, mAvailableWeapons.Num() - 1);
    return mAvailableWeapons[Index];
}

TScriptInterface<IRewardData> URewardSelector::selectRandomPassiveItem()
{
    float TotalWeight = 0.f;
    TArray<float> Weights;
    
    for (auto& Reward : mAvailablePassiveItems)
    {
        UPassiveItemRewardData* ItemData = Cast<UPassiveItemRewardData>(Reward.GetObject());
        float Rarity = ItemData ? ItemData->Rarity : 0.0f;
        Weights.Add(Rarity);
        TotalWeight += Rarity;
    }
    
    if (TotalWeight <= 0.f)
        return nullptr;

    float Rand = FMath::FRandRange(0.f, TotalWeight);
    float Accum = 0.f;
    for (int32 i = 0; i < mAvailablePassiveItems.Num(); ++i)
    {
        Accum += Weights[i];
        if (Rand < Accum)
            return mAvailablePassiveItems[i];
    }
    return nullptr;
}

TScriptInterface<IRewardData> URewardSelector::selectRandomPowerUp()
{
    int32 Index = FMath::RandRange(0, mAvailablePowerUps.Num() - 1);
    return mAvailablePowerUps[Index];
}

void URewardSelector::removeFromAvailablePool(const TScriptInterface<IRewardData>& Reward)
{
    if (!Reward->IsMaxLevel())
        return;
    
    mAvailableWeapons.Remove(Reward);
    mAvailablePassiveItems.Remove(Reward);
    mAvailablePowerUps.Remove(Reward);
}