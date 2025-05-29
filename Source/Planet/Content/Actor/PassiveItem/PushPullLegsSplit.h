// PushPullLegsSplit.h
#pragma once

#include "CoreMinimal.h"
#include "PassiveItem.h"
#include "PushPullLegsSplit.generated.h"

enum class EPlanetDayOfWeek : uint8;
class APlanetPawn;

UCLASS()
class PLANET_API APushPullLegsSplit : public APassiveItem
{
	GENERATED_BODY()

public:
	APushPullLegsSplit();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void LevelUp(const int32& _newLevel) override;

	UPROPERTY(EditAnywhere, Category = "Push Pull Legs Split")
	float EachShield		= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Push Pull Legs Split")
	float MaxWeeklyShield	= 15.0f;
	
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV2	= 7.5f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV3	= 10.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV4	= 15.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV5	= 20.0f;

private:
	UFUNCTION()
	void addShieldToPlayer(EPlanetDayOfWeek _newDay);
	
	APlanetPawn* cOwner;
	float mWeeklyShield;
	bool bIsWeeklyRoutineEnd;
};
