// IUpdateStrategy.h
#pragma once

#include "CoreMinimal.h"

class IUpdateStrategy
{
public:
	virtual ~IUpdateStrategy() = default;
	virtual void Update(float _deltaTime) = 0;
	virtual void OnLookInput(const FVector2D& _inputValue) {}
};