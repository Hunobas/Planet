// PlayerSetting.h
#pragma once

#include "CoreMinimal.h"
#include "PlayerSetting.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FPlayerSetting
{
	GENERATED_BODY()

	FPlayerSetting()
		: HP(100.0f)
		, Damage(10.0f)
		, Critical(5.0f)
		, CriticalDamage(150.0f)
		, Haste(100.0f)
		, XpGain(100.0f)
	{}

	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float HP;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float Damage;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float Critical;
    UPROPERTY(VisibleInstanceOnly, Category = "Combat")
    float CriticalDamage;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float Haste;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float XpGain;
};
