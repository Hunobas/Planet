// PlayerDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerDataAsset.generated.h"

UCLASS(Blueprintable)
class PLANET_API UPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float HPBase				= 100.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageBase			= 10.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CriticalBase			= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CriticalDamageBase	= 150.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float HasteBase				= 100.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float XPGainBase			= 100.0f;
};
