// RewardSelector.h
#pragma once

#include "CoreMinimal.h"
#include "RewardSelector.generated.h"

class URewardManager;
class APlanetPawn;
class IRewardData;

UCLASS()
class PLANET_API URewardSelector : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(URewardManager* Manager, APlanetPawn* _owner);

	TArray<TScriptInterface<IRewardData>> SelectRewards(int32 RequestedCount);

private:
	void updateAvailablePools();
	bool hasAvailableRewards() const;
	TScriptInterface<IRewardData> selectRandomRewardOrNull();
	TScriptInterface<IRewardData> selectRandomWeapon();
	TScriptInterface<IRewardData> selectRandomPassiveItem();
	TScriptInterface<IRewardData> selectRandomPowerUp();
	void removeFromAvailablePool(const TScriptInterface<IRewardData>& Reward);

	APlanetPawn* cOwner = nullptr;
	URewardManager* mManager = nullptr;
	
	TArray<TScriptInterface<IRewardData>> mAvailableWeapons;
	TArray<TScriptInterface<IRewardData>> mAvailablePassiveItems;
	TArray<TScriptInterface<IRewardData>> mAvailablePowerUps;
	
	float mWeaponAppearanceRate;
	float mPassiveItemAppearanceRate;
};
