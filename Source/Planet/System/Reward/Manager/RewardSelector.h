// RewardSelector.h
#pragma once

#include "CoreMinimal.h"
#include "../Planet.h"
#include "RewardSelector.generated.h"

class URewardManager;
class IRewardData;

UCLASS()
class PLANET_API URewardSelector : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(URewardManager* Manager);

	TArray<TScriptInterface<IRewardData>> SelectRewards(int32 RequestedCount);

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	float WeaponAppearanceRate = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	float PassiveItemAppearanceRate = 0.35f;

private:
	void updateAvailablePools();
	bool hasAvailableRewards() const;
	TScriptInterface<IRewardData> selectRandomRewardOrNull();
	TScriptInterface<IRewardData> selectRandomWeapon();
	TScriptInterface<IRewardData> selectRandomPassiveItem();
	TScriptInterface<IRewardData> selectRandomPowerUp();
	void removeFromAvailablePool(const TScriptInterface<IRewardData>& Reward);

	URewardManager* mManager = nullptr;

	TArray<TScriptInterface<IRewardData>> mAvailableWeapons;
	TArray<TScriptInterface<IRewardData>> mAvailablePassiveItems;
	TArray<TScriptInterface<IRewardData>> mAvailablePowerUps;

	int32 RemainWeaponSlots = WEAPON_MAX_SLOT;
	int32 RemainPassiveItemSlots = ITEM_MAX_SLOT;
};
