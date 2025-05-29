// EquatorWave.cpp
#include "EquatorWave.h"

#include "DayOfWeekComponent.h"
#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlanetController.h"
#include "DefaultLaser.h"

AEquatorWave::AEquatorWave(): LaserNum(6), FireSound(nullptr), cOwner(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	mLasers.Reserve(LaserNum);
	mMuzzles.SetNum(LaserNum);
}

void AEquatorWave::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
	check(cOwner);

	initializeMuzzles();
	bindControllerEvents();

    check(mMuzzles[0]);
    check(mMuzzles[1]);
    ADefaultLaser* laser1 = spawnLaser(mMuzzles[0]);
    ADefaultLaser* laser2 = spawnLaser(mMuzzles[1]);
    mLasers.Add(laser1);
    mLasers.Add(laser2);

	StopAttack();
	StartAttack();
}

void AEquatorWave::LevelUp(const int32& _newLevel)
{
	Super::LevelUp(_newLevel);

	switch (_newLevel)
	{
	case 2:
		{
			check(mMuzzles[2]);
			ADefaultLaser* laser3 = spawnLaser(mMuzzles[2]);
			mLasers.Add(laser3);
			break;
		}
	case 3:
		Damage = Damage_LV3;
		break;
	case 4:
		{
			check(mMuzzles[3]);
			ADefaultLaser* laser4 = spawnLaser(mMuzzles[3]);
			mLasers.Add(laser4);
			break;
		}
	case 5:
		DamageInterval = DamageInterval_LV5;
		break;
	case 6:
		Damage = Damage_LV6;
		break;
	case 7:
		{
			check(mMuzzles[4]);
			check(mMuzzles[5]);
			ADefaultLaser* laser5 = spawnLaser(mMuzzles[4]);
			ADefaultLaser* laser6 = spawnLaser(mMuzzles[5]);
			mLasers.Add(laser5);
			mLasers.Add(laser6);
			break;
		}
	default:
		checkNoEntry();
		break;
	}
}

void AEquatorWave::StartAttack()
{
	check(mMuzzles[0]);
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		FireSound,
		mMuzzles[0]->GetComponentLocation()
	);

	GetWorld()->GetTimerManager().SetTimer(
		mLaserTickHandle,
		this,
		&AEquatorWave::tickAllLasers,
		DamageInterval,
		true,
		0.0f
	);
}

void AEquatorWave::StopAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(mLaserTickHandle);
}

void AEquatorWave::UpdateMuzzleOrbit(const float _weeklyAngle)
{
	for (int32 i = 0; i < LaserNum; ++i)
	{
		if (!mMuzzles[i])
			continue;
        
		const float rotationAngle = (_weeklyAngle / DEGREES_PER_WEEK) * 360.0f * ROTATION_MULTIPLIERS[i];
		const float radians = FMath::DegreesToRadians(rotationAngle);
        
		const FVector orbitPosition = FVector(
			PlanetRadius * FMath::Cos(radians),
			PlanetRadius * FMath::Sin(radians),
			0.0f
		);
        
		mMuzzles[i]->SetRelativeLocation(orbitPosition);
		mMuzzles[i]->SetRelativeRotation(orbitPosition.Rotation());
	}
}

ADefaultLaser* AEquatorWave::spawnLaser(USceneComponent* _muzzle)
{
	check(_muzzle);
	check(LaserClass);
	
	ADefaultLaser* laser = GetWorld()->SpawnActor<ADefaultLaser>(LaserClass, _muzzle->GetComponentTransform());
	check(laser);
	
	laser->AttachToComponent(
		_muzzle,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale
	);
	laser->Initialize(this);
	
	return laser;
}

void AEquatorWave::tickAllLasers()
{
	for (ADefaultLaser* laser : mLasers)
	{
		if (laser)
		{
			laser->FireTick();
		}
	}
}

void AEquatorWave::initializeMuzzles()
{
	const TArray muzzleTags = {
		LaserMuzzle1Tag, LaserMuzzle2Tag, LaserMuzzle3Tag,
		LaserMuzzle4Tag, LaserMuzzle5Tag, LaserMuzzle6Tag
	};
    
	for (int32 i = 0; i < LaserNum; ++i)
	{
		if (!TryGetFirstComponentWithTag(this, muzzleTags[i], mMuzzles[i]))
		{
			checkf(false, TEXT("[AEquatorWave] %s->'%s' 태그를 가진 컴포넌트를 찾지 못했습니다."),
				*this->GetName(), *muzzleTags[i].ToString());
		}
		check(mMuzzles[i]);
	}
}

void AEquatorWave::bindControllerEvents()
{
	APlanetController* PC = Cast<APlanetController>(cOwner->GetController());
	check(PC);
	
	check(cOwner);
	check(cOwner->DayOfWeek);
	PC->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
	{
		UpdateMuzzleOrbit(cOwner->DayOfWeek->WeeklyAngle);
	});
}
