// RewardManager.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IRewardData.h"
#include "RewardManager.generated.h"

class URewardSelector;
class IRewardApplicator;
class UDataTable;
class APlanetPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardApplied, const TScriptInterface<IRewardData>&, AppliedReward);

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
	TArray<TScriptInterface<IRewardData>> GetAvailableRewards(int32 RewardCount = 3);

	UFUNCTION(BlueprintCallable, Category = "Reward")
	void ApplyReward(const TScriptInterface<IRewardData>& Reward);

	int32 GetRemainWeaponSlots() const;
	int32 GetRemainPassiveItemSlots() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRewardApplied OnRewardApplied;
	
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	UDataTable* RewardTextDataTable;
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	UDataTable* WeaponConfigDataTable;
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	UDataTable* PassiveItemConfigDataTable;

	UPROPERTY(EditAnywhere, Category = "Reward")
	URewardSelector* RewardSelector;
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TScriptInterface<IRewardData>> AllWeaponRewards;
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TScriptInterface<IRewardData>> AllPassiveItemRewards;
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TScriptInterface<IRewardData>> AllPlayerPowerUpRewards;

private:
	void initializeApplicators();
	FText getLocalizedText(const FString& TextKey, const FString& Category) const;
	void loadWeaponConfigs();
	void loadPassiveItemConfigs();
	void loadPowerUpConfigs();

	APlanetPawn* cOwner;
	
	TMap<FString, TScriptInterface<IRewardApplicator>> mApplicators;
};
