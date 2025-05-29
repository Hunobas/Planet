// EquatorWave.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponPawn.h"
#include "EquatorWave.generated.h"

class APlanetPawn;
class ADefaultLaser;

UCLASS()
class PLANET_API AEquatorWave : public AWeaponPawn
{
	GENERATED_BODY()

public:
	AEquatorWave();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void LevelUp(const int32& _newLevel) override;
	virtual void StartAttack() override;
	virtual void StopAttack() override;

	UFUNCTION()
	void UpdateMuzzleOrbit(const float _weeklyAngle);

	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle1Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle2Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle3Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle4Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle5Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	FName LaserMuzzle6Tag;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	TSubclassOf<ADefaultLaser> LaserClass;
	UPROPERTY(EditAnywhere, Category = "Equator Wave")
	float DamageInterval		= 0.15f;
	UPROPERTY(VisibleAnywhere, Category = "Equator Wave")
	int32 LaserNum				= 6;
	UPROPERTY(VisibleAnywhere, Category = "Equator Wave")
	float PlanetRadius			= 100.0f;

	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV3			= 2.5f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float DamageInterval_LV5	= 0.08f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV6			= 3.8f;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* FireSound;

private:
	ADefaultLaser* spawnLaser(USceneComponent* _muzzle);
	void tickAllLasers(); 
	void initializeMuzzles();
	void bindControllerEvents();

	APlanetPawn* cOwner;
	TArray<USceneComponent*> mMuzzles;
	TArray<ADefaultLaser*> mLasers;
	
	int32 mMaxPierce = 1;
	static constexpr float ROTATION_MULTIPLIERS[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

	FTimerHandle mLaserTickHandle;
};
