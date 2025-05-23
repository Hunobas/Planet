// PlayerPowerUpRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardData.h"
#include "PlayerPowerUpType.h"
#include "PlayerPowerUpRewardData.generated.h"

UCLASS(Blueprintable)
class PLANET_API UPlayerPowerUpRewardData : public UObject, public IRewardData
{
	GENERATED_BODY()

public:
	virtual int32 GetCurrentLevel() const override { return mCurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { mCurrentLevel = NewLevel; }
	virtual bool IsMaxLevel() const override { return mCurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EPlayerPowerUpType PowerUpType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 100;

private:
	int32 mCurrentLevel = 1;
};