// DayOfWeekComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "DayOfWeekComponent.generated.h"

class APlanetPawn;

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

protected:
	virtual void BeginPlay() override;

public:
	void UpdateRotation(float _planetYaw);
    
	UFUNCTION(BlueprintCallable, Category = "Day of Week")
	float GetDailyProgress() const { return DailyAngle * DAY_TO_DEGREES_RATIO; }
	UFUNCTION(BlueprintCallable, Category = "Day of Week")
	float GetWeeklyProgress() const { return WeeklyAngle * WEEK_TO_DEGREES_RATIO; }

	UPROPERTY(BlueprintAssignable, Category = "Day of Week")
	FOnDayChanged OnDayChanged;

	UPROPERTY(EditAnywhere, Category = "Day of Week")
	float DayPassYawGap = 45.0f;
	UPROPERTY(EditAnywhere, Category = "Day of Week")
	float DayBackingYawGap = -90.0f;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week", meta=(ClampMin=0, ClampMax=360))
	float DailyAngle;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week", meta=(ClampMin=0, ClampMax=2520))
	float WeeklyAngle;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	EPlanetDayOfWeek CurrentDay;
	UPROPERTY(VisibleAnywhere, Category = "Day of Week")
	float MeridianYaw;

	float SpendAngle;

private:
	void updateCurrentDay();
	EPlanetDayOfWeek calculateDay() const;

	APlanetPawn* cOwner;
	
	float mWeeklyAngleStack;
	float mPreviousPlanetYaw;
};
