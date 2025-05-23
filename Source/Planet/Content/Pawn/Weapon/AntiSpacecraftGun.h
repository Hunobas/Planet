// AAntiSpacecraftGun.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponPawn.h"
#include "AntiSpacecraftGun.generated.h"

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
	virtual void LevelUp(const int32& _newLevel) override;
	virtual void Fire() override;

	void StartAttack();
	void StopAttack();

	UPROPERTY(EditAnywhere, Category = "ASGun")
	FName MuzzleCenterTag;
	UPROPERTY(EditAnywhere, Category = "ASGun")
	FName MuzzleLeftTag;
	UPROPERTY(EditAnywhere, Category = "ASGun")
	FName MuzzleRightTag;
	UPROPERTY(EditAnywhere, Category = "ASGun")
	TSubclassOf<ADefaultProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "ASGun")
	float FireInterval			= 0.3f;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV2			= 40.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireRate_LV3			= 1.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireInterval_LV3		= 0.2f;
	UPROPERTY(EditAnywhere, Category = "LV")
	bool bReleaseSideSpawnPoint	= false;
	UPROPERTY(EditAnywhere, Category = "LV")
	float Damage_LV5			= 70.0f;
	UPROPERTY(EditAnywhere, Category = "LV")
	int32 MaxPierce_LV6			= 3;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireRate_LV7			= 0.4f;
	UPROPERTY(EditAnywhere, Category = "LV")
	float FireInterval_LV7		= 0.1f;

private:
	void burstFire();
	ADefaultProjectile* spawnProjectileOrNull(const USceneComponent* _muzzle);

	APlanetPawn* cOwner;
	UObjectPoolManagerComponent* mPool;
	USceneComponent* mMuzzleCenter;
	USceneComponent* mMuzzleLeft;
	USceneComponent* mMuzzleRight;
	
	FTimerHandle mFireTimerHandle;
	FTimerHandle mBurstFireTimerHandle;
	int32 mBurstFireCount = 0;
	int32 mMaxPierce = 1;
};

