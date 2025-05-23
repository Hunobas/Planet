// PlayerPowerUpType.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerPowerUpType : uint8
{
	None            = 0,
	HP				UMETA(DisplayName="체력"),
	Damage			UMETA(DisplayName="데미지"),
	Critical		UMETA(DisplayName="치명타 확률"),
	CriticalDamage	UMETA(DisplayName="치명타 데미지"),
	Haste			UMETA(DisplayName="공격 속도"),
	XpGain			UMETA(DisplayName="XP 획득량"),
	XpSpeedScale	UMETA(DisplayName="XP 이동속도"),
};