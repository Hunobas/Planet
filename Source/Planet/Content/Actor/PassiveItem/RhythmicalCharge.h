// RhythmicalCharge.h
#pragma once

#include "CoreMinimal.h"
#include "PassiveItem.h"
#include "RhythmicalCharge.generated.h"

class APlanetPawn;

UCLASS()
class PLANET_API ARhythmicalCharge : public APassiveItem
{
	GENERATED_BODY()

public:
	ARhythmicalCharge();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void LevelUp(const int32& _newLevel) override;

	UPROPERTY(EditAnywhere, Category = "Rhythmical Charge")
	float EachShield		= 4.0f;
	UPROPERTY(EditAnywhere, Category = "Rhythmical Charge")
	float EachHeal			= 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV2	= 7.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV3	= 10.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV4	= 12.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachShield_LV5	= 14.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float EachHeal_LV5		= 3.0f;

private:
	void addShieldAndHealToPlayer() const;
	
	APlanetPawn* cOwner;
};
