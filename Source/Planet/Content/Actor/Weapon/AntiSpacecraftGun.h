// AAntiSpacecraftGun.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponPawn.h"
#include "AntiSpacecraftGun.generated.h"

enum class EPlanetDayOfWeek : uint8;
class USoundBase;
class ADefaultProjectile;
class APlanetPawn;
class UObjectPoolManagerComponent;

UCLASS()
class PLANET_API AAntiSpacecraftGun : public AWeaponPawn
{
	GENERATED_BODY()

public:
	AAntiSpacecraftGun();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void LevelUp(const int32& _newLevel) override;
	virtual void Fire() override;

	virtual void StartAttack() override;
	virtual void StopAttack() override;

	void UpdateRotation();

	UPROPERTY(EditAnywhere, Category = "AS Gun")
	FName MuzzleCenterTag;
	UPROPERTY(EditAnywhere, Category = "AS Gun")
	FName MuzzleLeftTag;
	UPROPERTY(EditAnywhere, Category = "AS Gun")
	FName MuzzleRightTag;
	UPROPERTY(EditAnywhere, Category = "AS Gun")
	TSubclassOf<ADefaultProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "AS Gun")
	int32 BurstFireMaxCount		= 4;
	UPROPERTY(EditAnywhere, Category = "AS Gun")
	float ReloadDelay			= 0.5f;
	UPROPERTY(VisibleAnywhere, Category = "AS Gun", meta=(ClampMin=0, ClampMax=360))
	float DailyAngle;
	UPROPERTY(VisibleAnywhere, Category = "AS Gun", meta=(ClampMin=0, ClampMax=2520))
	float WeeklyAngle;

	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV2			= 40.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireRate_LV3			= 0.2f;
	UPROPERTY(EditAnywhere, Category = "LV")
	int32 BurstFireMaxCount_LV3	= 5;
	UPROPERTY(EditAnywhere, Category = "LV")
	bool bReleaseSideSpawnPoint	= false;
	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV5			= 70.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	int32 MaxPierce_LV6			= 3;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireRate_LV7			= 0.1f;
	UPROPERTY(EditAnywhere, Category = "LV")
	int32 BurstFireMaxCount_LV7	= 6;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* FireSound;

private:
	void burstFire();
	void onReloadComplete();
	ADefaultProjectile* spawnProjectileOrNull(const USceneComponent* _muzzle);
	void updateCurrentDay();
	EPlanetDayOfWeek calculateDay() const;

	UPROPERTY()
	APlanetPawn* cOwner;
	UPROPERTY()
	UObjectPoolManagerComponent* mPool;
	
	USceneComponent* mMuzzleCenter;
	USceneComponent* mMuzzleLeft;
	USceneComponent* mMuzzleRight;
	
	FTimerHandle mBurstFireTimerHandle;
	FTimerHandle mReloadTimerHandle;
	float mFireRate;
	int32 mBurstFireCount = 0;
	int32 mMaxPierce = 1;
	bool bIsReloadDelayElasped;

	float mDayPassYawGap = 45.0f;
	float mWeeklyAngleStack;
	float mReloadYaw;
	EPlanetDayOfWeek mCurrentDay;
	bool bIsDayPassed;
};

