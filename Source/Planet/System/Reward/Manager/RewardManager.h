// RewardManager.h
#pragma once

#include "CoreMinimal.h"
#include "../Planet.h"
#include "RewardManager.generated.h"

class IRewardData;
class UWeaponRewardData;
class UPassiveItemRewardData;
class UPlayerPowerUpRewardData;
class URewardSelector;
class IRewardApplicator;
class UDataTable;
class APlanetPawn;
class URewardSelectionWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API URewardManager : public UActorComponent
{
	GENERATED_BODY()

public:
	URewardManager();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Reward")
	TArray<TScriptInterface<IRewardData>> GetAvailableRewards(const int32& RewardCount);

	UFUNCTION(BlueprintCallable, Category = "Reward")
	void ApplyReward(const TScriptInterface<IRewardData>& Reward);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardApplied, const TScriptInterface<IRewardData>&, AppliedReward);
	FOnRewardApplied OnRewardApplied;
	
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TSubclassOf<UWeaponRewardData>> AllWeaponRewards;
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TSubclassOf<UPassiveItemRewardData>> AllPassiveItemRewards;
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TSubclassOf<UPlayerPowerUpRewardData>> AllPlayerPowerUpRewards;
	
	UPROPERTY(EditAnywhere, Category = "Reward")
	float WeaponAppearanceRate		= WEAPON_APPEAR_RATE;
	UPROPERTY(EditAnywhere, Category = "Reward")
	float PassiveItemAppearanceRate	= ITEM_APPEAR_RATE;

	TArray<UWeaponRewardData*> CachedWeaponInstances;
	TArray<UPassiveItemRewardData*> CachedPassiveItemInstances;
	TArray<UPlayerPowerUpRewardData*> CachedPowerUpInstances;
	
private:
	void initializeApplicators();
	void createAllRewardInstances();

	APlanetPawn* cOwner;
	URewardSelector* mRewardSelector;
	TMap<FString, TScriptInterface<IRewardApplicator>> mApplicators;
	
	FString mCurrentLanguage;
};
