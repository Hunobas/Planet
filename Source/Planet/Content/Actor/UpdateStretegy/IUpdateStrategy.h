// IUpdateStrategy.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUpdateType : uint8
{
	Continuous	UMETA(DisplayName="시간 연속적"),
	InputDriven	UMETA(DisplayName="플레이어 입력")
};

class IUpdateStrategy
{
public:
	virtual ~IUpdateStrategy() = default;
	virtual void Update(float _deltaTime) = 0;
	virtual void OnLookInput(const FVector2D& _inputValue) {}
};