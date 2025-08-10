// WeekendShift.h
#pragma once

#include "CoreMinimal.h"
#include "PassiveItem.h"
#include "WeekendShift.generated.h"

enum class EPlanetDayOfWeek : uint8;
class APlanetPawn;
class UWaveManagerComponent;

UCLASS()
class PLANET_API AWeekendShift : public APassiveItem
{
	GENERATED_BODY()

public:
	AWeekendShift();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void LevelUp(const int32& _newLevel) override;

	UPROPERTY(EditAnywhere, Category = "Weekend Shift")
	float PremiumPay		= 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "LV")
	float PremiumPay_LV2	= 0.4f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float PremiumPay_LV3	= 0.6f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float PremiumPay_LV4	= 0.8f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float PremiumPay_LV5	= 1.0f;

private:
	UFUNCTION()
	void adjustPremiumPay(EPlanetDayOfWeek _newDay);
	
	APlanetPawn* cOwner;
	UWaveManagerComponent* mWaveManager;

	float mThisWeekPay;
};
