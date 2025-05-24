// PassiveItemRewardApplicator.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardApplicator.h"
#include "PassiveItemRewardApplicator.generated.h"

UCLASS()
class PLANET_API UPassiveItemRewardApplicator  : public UObject, public IRewardApplicator
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer) override;
};
