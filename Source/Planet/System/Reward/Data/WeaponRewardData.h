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
	virtual FName GetRewardIdentifier() const override
	{
		FString FullIdentifier = WEAPON_REWARD_TAG + "_" + UEnum::GetValueAsString(WeaponType);
		return FName(*FullIdentifier);
	}
	virtual UTexture2D* GetRewardIcon() const override { return WeaponIcon; }
	virtual int32 GetCurrentLevel() const override { return CurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { CurrentLevel = FMath::Clamp(NewLevel, 1, MaxLevel); }
	virtual bool IsMaxLevel() const override { return CurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UTexture2D* WeaponIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 7;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 CurrentLevel = 1;

};