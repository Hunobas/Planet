// RewardSelector.cpp
#include "RewardSelector.h"

#include "RewardManager.h"
#include "PlanetPawn.h"
#include "WeaponRewardData.h"
#include "PassiveItemRewardData.h"
#include "PlayerPowerUpRewardData.h"        // 없을 시 빌드에러.
#include "WeaponPawn.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"

void URewardSelector::Initialize(URewardManager* Manager, APlanetPawn* _owner)
{
    mManager = Manager;
    cOwner = _owner;

    check(mManager);
    mWeaponAppearanceRate = mManager->WeaponAppearanceRate;
    mPassiveItemAppearanceRate = mManager->PassiveItemAppearanceRate;
    
    updateAvailablePools();
}

TArray<TScriptInterface<IRewardData>> URewardSelector::SelectRewards(int32 RequestedCount)
{
    updateAvailablePools();

    TArray<TScriptInterface<IRewardData>> SelectedRewards;
    while (SelectedRewards.Num() < RequestedCount && hasAvailableRewards())
    {
        if (TScriptInterface<IRewardData> Reward = selectRandomRewardOrNull())
        {
            SelectedRewards.Add(Reward);
            removeFromAvailablePool(Reward);
        }
        else
        {
            check(false);
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

    check(cOwner->WeaponSlot);
    if (cOwner->WeaponSlot->RemainSlots > 0)
    {
        for (auto* Weapon : mManager->CachedWeaponInstances)
        {
            const auto CurrentWeapon = cOwner->WeaponSlot->GetWeaponByTypeOrNull(Weapon->WeaponType);
            Weapon->SetLevel(CurrentWeapon ? CurrentWeapon->CurrentLevel : 0);
            
            if (!Weapon->IsMaxLevel())
            {
                mAvailableWeapons.Add(Weapon);
            }
        }
    }

    check(cOwner->ItemSlot);
    if (cOwner->ItemSlot->RemainSlots > 0)
    {
        for (auto* Item : mManager->CachedPassiveItemInstances)
        {
            const auto CurrentItem = cOwner->ItemSlot->GetItemByTypeOrNull(Item->PassiveItemType);
            Item->SetLevel(/*CurrentItem ? CurrentItem->CurrentLevel :*/ 0);
            
            if (!Item->IsMaxLevel())
            {
                mAvailablePassiveItems.Add(Item);
            }
        }
    }

    mAvailablePowerUps.Append(mManager->CachedPowerUpInstances);
}

bool URewardSelector::hasAvailableRewards() const
{
    return mAvailableWeapons.Num() > 0 || mAvailablePassiveItems.Num() > 0 || mAvailablePowerUps.Num() > 0;
}

TScriptInterface<IRewardData> URewardSelector::selectRandomRewardOrNull()
{
    const bool bWeaponAvailable = mAvailableWeapons.Num() > 0;
    const bool bPassiveAvailable = mAvailablePassiveItems.Num() > 0;
    const bool bPowerUpAvailable = mAvailablePowerUps.Num() > 0;

    float totalProb = 0.f;
    if (bWeaponAvailable) totalProb += mWeaponAppearanceRate;
    if (bPassiveAvailable) totalProb += mPassiveItemAppearanceRate;
    if (bPowerUpAvailable) totalProb += (1.f - mWeaponAppearanceRate - mPassiveItemAppearanceRate);

    if (totalProb <= 0.f) return nullptr;

    float RandomValue = FMath::FRandRange(0.f, totalProb);

    float Accum = 0.0f;
    if (bWeaponAvailable)
    {
        Accum += mWeaponAppearanceRate;
        if (RandomValue < Accum)
            return selectRandomWeapon();
    }
    if (bPassiveAvailable)
    {
        Accum += mPassiveItemAppearanceRate;
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
    mAvailableWeapons.RemoveSingleSwap(Reward);
    mAvailablePassiveItems.RemoveSingleSwap(Reward);
    mAvailablePowerUps.RemoveSingleSwap(Reward);
}