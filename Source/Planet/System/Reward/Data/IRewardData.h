// IRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRewardData.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class URewardData : public UInterface
{
	GENERATED_BODY()
};

class PLANET_API IRewardData
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Reward")
	virtual FName GetRewardIdentifier() const = 0;
    
	UFUNCTION(BlueprintCallable, Category="Reward")
	virtual UTexture2D* GetRewardIcon() const = 0;
	
	UFUNCTION(BlueprintCallable, Category="Reward")
	virtual int32 GetCurrentLevel() const = 0;

	UFUNCTION(BlueprintCallable, Category="Reward")
	virtual void SetLevel(int32 NewLevel) = 0;

	UFUNCTION(BlueprintCallable, Category="Reward")
	virtual bool IsMaxLevel() const = 0;
};
