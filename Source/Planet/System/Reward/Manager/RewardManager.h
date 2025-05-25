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

	UPROPERTY()
	TArray<UWeaponRewardData*> CachedWeaponInstances;
	UPROPERTY()
	TArray<UPassiveItemRewardData*> CachedPassiveItemInstances;
	UPROPERTY()
	TArray<UPlayerPowerUpRewardData*> CachedPowerUpInstances;
	
private:
	void initializeApplicators();
	void createAllRewardInstances();

	APlanetPawn* cOwner;
	
	UPROPERTY()
	URewardSelector* mRewardSelector;
	UPROPERTY()
	TMap<FString, TScriptInterface<IRewardApplicator>> mApplicators;
};
