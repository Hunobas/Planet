// AAntiSpacecraftGun.cpp
#include "AntiSpacecraftGun.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "DefaultProjectile.h"

AAntiSpacecraftGun::AAntiSpacecraftGun()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AAntiSpacecraftGun::BeginPlay()
{
	Super::BeginPlay();

	if (!TryGetFirstComponentWithTag(this, MuzzleCenterTag, mMuzzleCenter))
	{
		checkf(false, TEXT("[AAntiSpacecraftGun] %s->'%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
			*this->GetName(), *MuzzleCenterTag.ToString());
	}
	if (!TryGetFirstComponentWithTag(this, MuzzleLeftTag, mMuzzleLeft))
	{
		checkf(false, TEXT("[AAntiSpacecraftGun] %s->'%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
			*this->GetName(), *MuzzleLeftTag.ToString());
	}
	if (!TryGetFirstComponentWithTag(this, MuzzleRightTag, mMuzzleRight))
	{
		checkf(false, TEXT("[AAntiSpacecraftGun] %s->'%s' 태그를 가진 컴포넌트를 찾지 못했습니다."), 
			*this->GetName(), *MuzzleRightTag.ToString());
	}

	mPool = GetObjectPoolManager(this);
}

void AAntiSpacecraftGun::LevelUp(const int32& _newLevel)
{
	Super::LevelUp(_newLevel);

	switch (_newLevel)
	{
	case 2:
		Damage = Damage_LV2;
		break;
	case 3:
		// bReleaseSideSpawnPoint = true;
		break;
	case 4:
		bReleaseSideSpawnPoint = true;
		break;
	case 5:
		// bReleaseUpdownSpawnPoint = true;
		break;
	case 6:
		// SetDamageScale(GetDamageScale() + DamageUp_LV6);
		break;
	case 7:
		// bReleasePositronRifle = true;
		// SetFireRate(LaserRate_LV7);
		break;
	default:
		break;
	}
}

void AAntiSpacecraftGun::Fire()
{
	Super::Fire();

	if (ADefaultProjectile* centerProjectile = spawnProjectileOrNull(mMuzzleCenter))
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			FireSound,
			mMuzzleCenter->GetComponentLocation()
		);
	}

	if (bReleaseSideSpawnPoint)
	{
		spawnProjectileOrNull(mMuzzleLeft);
		spawnProjectileOrNull(mMuzzleRight);
	}
}

ADefaultProjectile* AAntiSpacecraftGun::spawnProjectileOrNull(const USceneComponent* _muzzle)
{
	check(_muzzle);
	check(ProjectileClass);

	ADefaultProjectile* projectile = mPool->AcquireOrNull(ProjectileClass, _muzzle->GetComponentTransform());
	if (projectile)
	{
		projectile->SetOwner(this);
		projectile->Initialize(mPool);
	}
	
	return projectile;
}
