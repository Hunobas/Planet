// EnemySetting.h
#pragma once

#include "CoreMinimal.h"
#include "EnemySetting.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FEnemySetting
{
	GENERATED_BODY()

	FEnemySetting()
		: HP(100.0f)
		, Damage(10.f)
		, Speed(100.f)
		, XPDrop(5.f)
		, FieldScore(1)
	{}

	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float HP;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float Damage;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float Speed;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float XPDrop;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	int32 FieldScore;
};
