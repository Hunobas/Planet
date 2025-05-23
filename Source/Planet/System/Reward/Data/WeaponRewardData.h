// WeaponRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardData.h"
#include "WeaponType.h"
#include "WeaponRewardData.generated.h"

UCLASS(Blueprintable)
class PLANET_API UWeaponRewardData : public UObject, public IRewardData
{
	GENERATED_BODY()

public:
	virtual int32 GetCurrentLevel() const override { return mCurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { mCurrentLevel = FMath::Clamp(NewLevel, 1, MaxLevel); }
	virtual bool IsMaxLevel() const override { return mCurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 7;

private:
	int32 mCurrentLevel = 1;
};