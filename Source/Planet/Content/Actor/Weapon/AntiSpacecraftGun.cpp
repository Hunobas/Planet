// AAntiSpacecraftGun.cpp
#include "AntiSpacecraftGun.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlanetController.h"
#include "DayOfWeekComponent.h"
#include "ObjectPoolManagerComponent.h"
#include "DefaultProjectile.h"

AAntiSpacecraftGun::AAntiSpacecraftGun(): DailyAngle(0.0f), WeeklyAngle(0.0f), ReloadSound(nullptr), FireSound(nullptr),
                                          cOwner(nullptr),
                                          mPool(nullptr),
                                          mMuzzleCenter(nullptr),
                                          mMuzzleLeft(nullptr),
                                          mMuzzleRight(nullptr), mFireRate(BaseFireRate), bIsReloadDelayElasped(false),
                                          mWeeklyAngleStack(0.0f),
                                          mReloadYaw(180.0f),
                                          mCurrentDay(EPlanetDayOfWeek::Monday),
                                          bIsDayPassed(false)
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

	if (APlanetController* PlanetController = Cast<APlanetController>(cOwner->GetController()))
	{
		PlanetController->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
		{
			UpdateRotation();
		});
	}

	StopAttack();
	StartAttack();
}

void AAntiSpacecraftGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReloadDelayElasped && bIsDayPassed)
	{
		check(ReloadSound);
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ReloadSound,
			mMuzzleCenter->GetComponentLocation()
		);
		
		Fire();
		bIsReloadDelayElasped = false;
		bIsDayPassed = false;
	}
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
		BurstFireMaxCount = BurstFireMaxCount_LV3;
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
		BurstFireMaxCount = BurstFireMaxCount_LV7;
		break;
	default:
		checkNoEntry();
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
		mFireRate,
		true,
		0.0f
	);
}

void AAntiSpacecraftGun::StartAttack()
{
	check(cOwner);
	const float playerHaste = cOwner->RuntimeSettings.Haste;
	
	mFireRate = CalculateFireRate(BaseFireRate, playerHaste);
	mReloadYaw = cOwner->PreviousYaw;
	
	Fire();
}

void AAntiSpacecraftGun::StopAttack()
{
	GetWorldTimerManager().ClearTimer(mBurstFireTimerHandle);
	GetWorldTimerManager().ClearTimer(mReloadTimerHandle);
}

void AAntiSpacecraftGun::UpdateRotation()
{
	if (!IsValid(this) || !IsValid(cOwner))
		return;
	
	const float planetYaw = cOwner->PreviousYaw;
	const float spendAngle = ToFixedAngle(planetYaw - mReloadYaw);      // 0~360
	const float presentYaw = NormalizeAngle(planetYaw - mReloadYaw);    // -180~180

	if (DailyAngle >= NOON_ANGLE && presentYaw > 5 && presentYaw <= mDayPassYawGap)
	{
		mWeeklyAngleStack = FMath::Modulo(mWeeklyAngleStack + DEGREES_PER_DAY, DEGREES_PER_WEEK);
		DailyAngle = EPSILON;
	}
	else
	{
		DailyAngle = FMath::Modulo(MIDNIGHT_ANGLE + spendAngle, DEGREES_PER_DAY);
	}
	WeeklyAngle = FMath::Modulo(mWeeklyAngleStack + DailyAngle, DEGREES_PER_WEEK);

	updateCurrentDay();
}

void AAntiSpacecraftGun::burstFire()
{
	if (!IsValid(this) || !IsValid(mPool) || !IsValid(cOwner))
	{
		GetWorldTimerManager().ClearTimer(mBurstFireTimerHandle);
		return;
	}
	
	if (mBurstFireCount++ >= BurstFireMaxCount)
	{
		GetWorldTimerManager().ClearTimer(mBurstFireTimerHandle);
		mBurstFireCount = 0;

		mReloadYaw = cOwner->PreviousYaw;
		GetWorldTimerManager().SetTimer(
			mReloadTimerHandle,
			this,
			&AAntiSpacecraftGun::onReloadComplete,
			ReloadDelay,
			false
		);
		
		return;
	}

	if (ADefaultProjectile* centerProjectile = spawnProjectileOrNull(mMuzzleCenter))
	{
		check(FireSound);
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

void AAntiSpacecraftGun::onReloadComplete()
{
	bIsReloadDelayElasped = true;
}

ADefaultProjectile* AAntiSpacecraftGun::spawnProjectileOrNull(const USceneComponent* _muzzle)
{
	if (!IsValid(_muzzle) || !IsValid(mPool) || !ProjectileClass)
		return nullptr;
    
	ADefaultProjectile* projectile = mPool->AcquireOrNull<ADefaultProjectile>(ProjectileClass, _muzzle->GetComponentTransform());
	if (IsValid(projectile))
	{
		projectile->Initialize(this, mPool);
		projectile->MaxPierce = mMaxPierce;
	}
	
	return projectile;
}

void AAntiSpacecraftGun::updateCurrentDay()
{
	const EPlanetDayOfWeek newDay = calculateDay();
    
	if (newDay != mCurrentDay)
	{
		mCurrentDay = newDay;
		bIsDayPassed = true;
	}
}

EPlanetDayOfWeek AAntiSpacecraftGun::calculateDay() const
{
	const int32 dayIndex = static_cast<int32>(WeeklyAngle / DEGREES_PER_DAY) % DAY_PER_WEEK;
	return static_cast<EPlanetDayOfWeek>(dayIndex);
}