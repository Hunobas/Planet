// AAntiSpacecraftGun.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponPawn.h"
#include "AntiSpacecraftGun.generated.h"

class USoundBase;
class ADefaultProjectile;
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

	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	float Damage_LV2			= 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	bool bReleaseSideSpawnPoint = false;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	float DamageUp_LV6			= 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	float LaserDamage_LV7		= 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	float LaserRate_LV7			= 4.f;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	float MaxRange_LV7			= 8000.f;
	UPROPERTY(EditDefaultsOnly, Category = "레벨 업")
	int32 MaxHitInOneCycle_LV7	= 2;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleCenterTag;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleLeftTag;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleRightTag;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ADefaultProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	USoundBase* FireSound;

private:
	ADefaultProjectile* spawnProjectileOrNull(const USceneComponent* _muzzle);

	UObjectPoolManagerComponent* mPool;
	USceneComponent* mMuzzleCenter;
	USceneComponent* mMuzzleLeft;
	USceneComponent* mMuzzleRight;
};

