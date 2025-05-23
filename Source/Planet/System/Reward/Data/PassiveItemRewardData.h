// PassiveItemRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardData.h"
#include "PassiveItemType.h"
#include "PassiveItemRewardData.generated.h"

UCLASS(Blueprintable)
class PLANET_API UPassiveItemRewardData : public UObject, public IRewardData
{
	GENERATED_BODY()

public:
	virtual int32 GetCurrentLevel() const override { return mCurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { mCurrentLevel = NewLevel; }
	virtual bool IsMaxLevel() const override { return mCurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EPassiveItemType PassiveItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float Rarity = 1.0f;

private:
	int32 mCurrentLevel = 1;
};