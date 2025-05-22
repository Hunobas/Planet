// WeaponType.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None            = 0,
    MainWeapon      UMETA(DisplayName="메인 무기"),
    EquatorWave     UMETA(DisplayName="적도 웨아브"),
    WeatherStation  UMETA(DisplayName="기상 센터"),
};