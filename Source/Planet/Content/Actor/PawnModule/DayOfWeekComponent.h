// DayOfWeekComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "DayOfWeekComponent.generated.h"

UENUM(BlueprintType)
enum class EPlanetDayOfWeek : uint8
{
	Monday = 0      UMETA(DisplayName = "월"),
	Tuesday = 1     UMETA(DisplayName = "화"),
	Wednesday = 2   UMETA(DisplayName = "수"),
	Thursday = 3    UMETA(DisplayName = "목"),
	Friday = 4      UMETA(DisplayName = "금"),
	Saturday = 5    UMETA(DisplayName = "토"),
	Sunday = 6      UMETA(DisplayName = "일")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChanged, EPlanetDayOfWeek, NewDay);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UDayOfWeekComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDayOfWeekComponent();

	void UpdateRotation(float _yawDelta);
    
	UFUNCTION(BlueprintCallable, Category = "Day of Week")
	float GetDailyProgress() const { return DailyAngle / DEGREES_PER_DAY; }
	UFUNCTION(BlueprintCallable, Category = "Day of Week")
	float GetWeeklyProgress() const { return WeeklyAngle / DEGREES_PER_WEEK; }

	UPROPERTY(BlueprintAssignable, Category = "Day of Week")
	FOnDayChanged OnDayChanged;

	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	float DailyAngle;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	float WeeklyAngle;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	EPlanetDayOfWeek CurrentDay;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	float MeridianYaw;
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugMode = false;

private:
	void updateCurrentDay();
	void broadcastDayChange(EPlanetDayOfWeek _newDay);
	EPlanetDayOfWeek calculateDay() const;
};
