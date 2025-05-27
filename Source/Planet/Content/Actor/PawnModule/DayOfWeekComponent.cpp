// DayOfWeekComponent.cpp
#include "DayOfWeekComponent.h"

#include "../Planet.h"

UDayOfWeekComponent::UDayOfWeekComponent() : DailyAngle(0.0f), WeeklyAngle(0.0f), MeridianYaw(90.0f),
    CurrentDay(EPlanetDayOfWeek::Monday)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDayOfWeekComponent::UpdateRotation(float _yawDelta)
{
    if (FMath::Abs(_yawDelta) < EPSILON)
        return;
    
    if (_yawDelta > 0.0f)
    {
        DailyAngle += _yawDelta;
        
        if (DailyAngle >= DEGREES_PER_DAY)
        {
            const float overflow = DailyAngle - DEGREES_PER_DAY;
            DailyAngle = overflow;
        }
        
        WeeklyAngle += _yawDelta;
    }
    else
    {
        const float newDailyAngle = DailyAngle + _yawDelta;
        
        if (newDailyAngle >= 0.0f)
        {
            DailyAngle = newDailyAngle;
            WeeklyAngle += _yawDelta;
        }
        else
        {
            DailyAngle = 0.0f;
            MeridianYaw = NormalizeAngle(MeridianYaw + newDailyAngle);
        }
    }
    WeeklyAngle = FMath::Fmod(WeeklyAngle, DEGREES_PER_WEEK);
    
    updateCurrentDay();

#ifdef DEBUG
    if (bDebugMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("YawDelta: %.2f, DailyAngle: %.2f, WeeklyAngle: %.2f, Day: %d"), 
            _yawDelta, DailyAngle, WeeklyAngle, static_cast<int32>(CurrentDay));
    }
#endif
}

void UDayOfWeekComponent::updateCurrentDay()
{
    const EPlanetDayOfWeek newDay = calculateDay();
    
    if (newDay != CurrentDay)
    {
        broadcastDayChange(newDay);
        CurrentDay = newDay;
    }
}

EPlanetDayOfWeek UDayOfWeekComponent::calculateDay() const
{
    const int32 dayIndex = static_cast<int32>(WeeklyAngle / DEGREES_PER_DAY) % DAY_PER_WEEK;
    return static_cast<EPlanetDayOfWeek>(dayIndex);
}

void UDayOfWeekComponent::broadcastDayChange(EPlanetDayOfWeek _newDay)
{
    OnDayChanged.Broadcast(_newDay);

#ifdef DEBUG
    if (bDebugMode)
    {
        const TCHAR* dayNames[] = {
            TEXT("월요일"), TEXT("화요일"), TEXT("수요일"), TEXT("목요일"),
            TEXT("금요일"), TEXT("토요일"), TEXT("일요일")
        };
        
        UE_LOG(LogTemp, Warning, TEXT("요일 변경: %s -> %s"), 
            dayNames[static_cast<int32>(CurrentDay)], 
            dayNames[static_cast<int32>(_newDay)]);
    }
#endif
}