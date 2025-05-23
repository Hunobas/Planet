// PlayerPowerUpRewardApplicator.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardApplicator.h"
#include "PlayerPowerUpRewardApplicator.generated.h"

UCLASS()
class PLANET_API UPlayerPowerUpRewardApplicator  : public UObject, public IRewardApplicator
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(const TScriptInterface<IRewardData>& RewardData) override;
};
