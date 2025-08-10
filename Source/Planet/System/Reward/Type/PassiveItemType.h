// PassiveItemType.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPassiveItemType : uint8
{
	// TODO: 추가 예정
	None            = 0,
	PushPullLegsSplit	UMETA(DisplayName="주간 삼분할"),
	RhythmicalCharge	UMETA(DisplayName="리드미컬 차징"),
	WeekendShift		UMETA(DisplayName="주출 수당")
};