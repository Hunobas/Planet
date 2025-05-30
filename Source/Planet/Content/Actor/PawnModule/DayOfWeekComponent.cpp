// DayOfWeekComponent.cpp
#include "DayOfWeekComponent.h"

#include "PlanetPawn.h"
#include "PlanetHUD.h"

UDayOfWeekComponent::UDayOfWeekComponent() : DailyAngle(0.0f), WeeklyAngle(0.0f), CurrentDay(EPlanetDayOfWeek::Monday),
                                             MeridianYaw(180.0f), cOwner(nullptr), mWeeklyAngleStack(0.0f),
                                             mPreviousPlanetYaw(0.0f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDayOfWeekComponent::BeginPlay()
{
    Super::BeginPlay();

    cOwner = Cast<APlanetPawn>(GetOwner());
    check(cOwner);
}

void UDayOfWeekComponent::UpdateRotation(float _planetYaw)
{
    if (FMath::Abs(_planetYaw - mPreviousPlanetYaw) < EPSILON)
        return;
    mPreviousPlanetYaw = _planetYaw;
    
    const float spendAngle = ToFixedAngle(_planetYaw - MeridianYaw);      // 0~360
    const float presentYaw = NormalizeAngle(_planetYaw - MeridianYaw);    // -180~180
    
    if (DailyAngle >= NOON_ANGLE)
    {
        if (presentYaw >= 0 && presentYaw <= DayPassYawGap)
        {
            // 요일 진행
            mWeeklyAngleStack = FMath::Modulo(mWeeklyAngleStack + DEGREES_PER_DAY, DEGREES_PER_WEEK);
        }
        DailyAngle = FMath::Modulo(MIDNIGHT_ANGLE + spendAngle, DEGREES_PER_DAY);
    }
    else
    {
        if (presentYaw >= DayBackingYawGap && presentYaw <= 0)
        {
            // 역행
            MeridianYaw = _planetYaw;
            DailyAngle  = MIDNIGHT_ANGLE;
        }
        else
        {
            DailyAngle = FMath::Modulo(MIDNIGHT_ANGLE + spendAngle, DEGREES_PER_DAY);
        }
    }

    WeeklyAngle = FMath::Modulo(mWeeklyAngleStack + DailyAngle, DEGREES_PER_WEEK);
    updateCurrentDay();

    check(cOwner->PlanetHUD);
    cOwner->PlanetHUD->OnDailyProgressChanged(GetDailyProgress(), GetWeeklyProgress());
}

void UDayOfWeekComponent::updateCurrentDay()
{
    const EPlanetDayOfWeek newDay = calculateDay();
    
    if (newDay != CurrentDay)
    {
        OnDayChanged.Broadcast(newDay);
        CurrentDay = newDay;

        check(cOwner->PlanetHUD);
        cOwner->PlanetHUD->OnCurrentDayChanged(newDay);
    }
}

EPlanetDayOfWeek UDayOfWeekComponent::calculateDay() const
{
    const int32 dayIndex = static_cast<int32>(WeeklyAngle / DEGREES_PER_DAY) % DAY_PER_WEEK;
    return static_cast<EPlanetDayOfWeek>(dayIndex);
}