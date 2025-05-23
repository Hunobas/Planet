// EnemyDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"

UCLASS(Blueprintable)
class PLANET_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float HPBase		 = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageBase	 = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpeedBase		 = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float XPDropBase	 = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FieldScoreBase = 1.0f;
};
