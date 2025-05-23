// AAntiSpacecraftGun.cpp
#include "AntiSpacecraftGun.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "ObjectPoolManagerComponent.h"
#include "DefaultProjectile.h"

AAntiSpacecraftGun::AAntiSpacecraftGun(): FireSound(nullptr), cOwner(nullptr), mPool(nullptr), mMuzzleCenter(nullptr),
                                          mMuzzleLeft(nullptr),
                                          mMuzzleRight(nullptr)
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

	cOwner = Cast<APlanetPawn>(GetOwner());
	mPool = GetObjectPoolManager(this);

	StartAttack();
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
		BaseFireRate = FireRate_LV3;
		FireInterval = FireInterval_LV3;
		StopAttack();
		StartAttack();
		break;
	case 4:
		bReleaseSideSpawnPoint = true;
		break;
	case 5:
		Damage = Damage_LV5;
		break;
	case 6:
		mMaxPierce = MaxPierce_LV6;
		break;
	case 7:
		BaseFireRate = FireRate_LV7;
		FireInterval = FireInterval_LV7;
		StopAttack();
		StartAttack();
		break;
	default:
		break;
	}
}

void AAntiSpacecraftGun::Fire()
{
	Super::Fire();

	GetWorldTimerManager().SetTimer(
		mBurstFireTimerHandle,
		this,
		&AAntiSpacecraftGun::burstFire,
		FireInterval,
		true,
		0.0f
	);
}

void AAntiSpacecraftGun::StartAttack()
{
	const float playerHaste = cOwner->RuntimeSettings.Haste;
	const float fireRate = CalculateFireRate(BaseFireRate, playerHaste);

	GetWorldTimerManager().SetTimer(
		mFireTimerHandle,
		this,
		&AAntiSpacecraftGun::Fire,
		fireRate,
		true,
		0.0f
	);
}

void AAntiSpacecraftGun::StopAttack()
{
	GetWorldTimerManager().ClearTimer(mFireTimerHandle);
	GetWorldTimerManager().ClearTimer(mBurstFireTimerHandle);
}

void AAntiSpacecraftGun::burstFire()
{
	if (mBurstFireCount++ >= 4)
	{
		GetWorldTimerManager().ClearTimer(mBurstFireTimerHandle);
		mBurstFireCount = 0;
		return;
	}

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
	check(mPool);
	check(ProjectileClass);

	ADefaultProjectile* projectile = mPool->AcquireOrNull(ProjectileClass, _muzzle->GetComponentTransform());
	if (projectile)
	{
		projectile->SetOwner(this);
		projectile->Initialize(mPool);
		projectile->MaxPierce = mMaxPierce;
	}
	
	return projectile;
}
