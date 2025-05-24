// IRewardApplicator.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRewardApplicator.generated.h"

class IRewardData;
class APlanetPawn;

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
	void Apply(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer);

	virtual void Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer) = 0;
};
