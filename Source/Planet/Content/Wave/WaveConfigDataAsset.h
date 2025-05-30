// WaveConfigDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "PlanetConst.h"
#include "Engine/DataAsset.h"
#include "EnemyScaleSetting.h"
#include "WaveConfigDataAsset.generated.h"

class AEnemyPawn;

USTRUCT()
struct FSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Wave Config", meta=(AllowAbstract="false"))
	TSubclassOf<AEnemyPawn> EnemyClass;
	UPROPERTY(EditAnywhere, Category = "Wave Config", meta=(ClampMin="0"))
	int32 UnlockTime = 0;
	UPROPERTY(EditAnywhere, Category = "Wave Config", meta=(ClampMin="0"))
	int32 LockTime = PlanetConst::PLAYTIME;
};

UCLASS(Blueprintable)
class PLANET_API UWaveConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Wave Config")
	TArray<FSpawnInfo> SpawnInfos;

	UPROPERTY(EditAnywhere, Category = "Wave Config")
	float EnemySpawnInterval = 5.f;
	UPROPERTY(EditAnywhere, Category = "Wave Config")
	float DifficultyInterval = 60.f;

	UPROPERTY(EditAnywhere, Category = "Wave Config")
	FRuntimeFloatCurve MaxFieldScoreCurve;

	UPROPERTY(EditAnywhere, Category = "WaveConfig")
	FEnemyScaleSetting ScaleSettings;

	UPROPERTY(EditAnywhere, Category = "Enemy Scale")
	FRuntimeFloatCurve HPScaleCurve;
	UPROPERTY(EditAnywhere, Category = "Enemy Scale")
	FRuntimeFloatCurve DamageScaleCurve;
	UPROPERTY(EditAnywhere, Category = "Enemy Scale")
	FRuntimeFloatCurve SpeedScaleCurve;
	UPROPERTY(EditAnywhere, Category = "Enemy Scale")
	FRuntimeFloatCurve XPDropScaleCurve;
};
