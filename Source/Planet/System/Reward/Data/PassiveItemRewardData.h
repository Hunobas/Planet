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
	virtual FName GetRewardIdentifier() const override
	{
		FString FullIdentifier = PASSIVEITEM_REWARD_TAG + "_" + UEnum::GetValueAsString(PassiveItemType);
		return FName(*FullIdentifier);
	}
	virtual UTexture2D* GetRewardIcon() const override { return PassiveItemIcon; }
	virtual int32 GetCurrentLevel() const override { return CurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { CurrentLevel = FMath::Clamp(NewLevel, 1, MaxLevel); }
	virtual bool IsMaxLevel() const override { return CurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UTexture2D* PassiveItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EPassiveItemType PassiveItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 CurrentLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float Rarity = 1.0f;
	
};