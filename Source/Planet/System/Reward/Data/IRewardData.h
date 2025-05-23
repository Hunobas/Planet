// IRewardData.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRewardData.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class URewardData : public UInterface
{
	GENERATED_BODY()
};

class PLANET_API IRewardData
{
	GENERATED_BODY()

public:
	virtual int32 GetRewardId() const PURE_VIRTUAL(IRewardData::GetRewardId, return 0;);
	virtual FString GetDisplayNameKey() const PURE_VIRTUAL(IRewardData::GetDisplayNameKey, return FString(););
	virtual FString GetDescriptionKey() const PURE_VIRTUAL(IRewardData::GetDescriptionKey, return FString(););
	virtual UTexture2D* GetIcon() const PURE_VIRTUAL(IRewardData::GetIcon, return nullptr;);
	virtual bool IsMaxLevel() const PURE_VIRTUAL(IRewardData::IsMaxLevel, return false;);
};
