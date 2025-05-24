// PlayerPowerUpRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "IRewardData.h"
#include "PlayerPowerUpType.h"
#include "PlayerPowerUpRewardData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PLANET_API UPlayerPowerUpRewardData : public UObject, public IRewardData
{
	GENERATED_BODY()

public:
	virtual FName GetRewardIdentifier() const override
	{
		FString FullIdentifier = POWERUP_REWARD_TAG + "_" + UEnum::GetValueAsString(PowerUpType);
		return FName(*FullIdentifier);
	}
	virtual UTexture2D* GetRewardIcon() const override { return PowerUpIcon; }
	virtual int32 GetCurrentLevel() const override { return CurrentLevel; }
	virtual void SetLevel(int32 NewLevel) override { CurrentLevel = FMath::Clamp(NewLevel, 1, MaxLevel); }
	virtual bool IsMaxLevel() const override { return CurrentLevel >= MaxLevel; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UTexture2D* PowerUpIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EPlayerPowerUpType PowerUpType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaxLevel = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 CurrentLevel = 0;
	
};