// EnemyScaleSetting.h
#pragma once

#include "CoreMinimal.h"
#include "EnemyScaleSetting.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FEnemyScaleSetting
{
	GENERATED_BODY()

	FEnemyScaleSetting()
		: HPScale(1.0f)
		, DamageScale(1)
		, SpeedScale(1.0f)
		, XPDropScale(1.0f)
	{}

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float HPScale;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float DamageScale;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float SpeedScale;
	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	float XPDropScale;
};
