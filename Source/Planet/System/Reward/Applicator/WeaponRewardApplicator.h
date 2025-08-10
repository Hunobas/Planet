// WeaponRewardApplicator.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardApplicator.h"
#include "WeaponRewardApplicator.generated.h"

UCLASS()
class PLANET_API UWeaponRewardApplicator : public UObject, public IRewardApplicator
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer) override;
};
