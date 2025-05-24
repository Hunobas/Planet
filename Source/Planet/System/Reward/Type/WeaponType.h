// WeaponType.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None            = 0,
    ASGun           UMETA(DisplayName="다연장 로켓"),
    EquatorWave     UMETA(DisplayName="적도 웨이브"),
    WeatherStation  UMETA(DisplayName="기상 센터"),
};