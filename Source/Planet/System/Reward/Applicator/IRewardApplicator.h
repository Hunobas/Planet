// IRewardApplicator.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRewardApplicator.generated.h"

class IRewardData;

UINTERFACE(MinimalAPI, BlueprintType)
class URewardApplicator : public UInterface
{
	GENERATED_BODY()
};

class PLANET_API IRewardApplicator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Reward")
	void Apply(const TScriptInterface<IRewardData>& RewardData);
	
	virtual void Apply_Implementation(const TScriptInterface<IRewardData>& RewardData) = 0;
};
