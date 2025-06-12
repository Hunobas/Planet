// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

#include "PlanetConst.h"
#include "PlanetController.h"
#include "PlanetHUD.h"
#include "PlayCamera.h"
#include "OrbitMover.h"
#include "HPComponent.h"
#include "ShieldComponent.h"
#include "LevelComponent.h"
#include "RewardManager.h"
#include "JustAimManagerComponent.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"
#include "DayOfWeekComponent.h"
#include "PlayerDataAsset.h"

APlanetPawn::APlanetPawn() : cPlanetController(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	composeComponent();
}

void APlanetPawn::BeginPlay()
{
	Super::BeginPlay();

	resetToDefaultSettings();

	cPlanetController = Cast<APlanetController>(GetController());
	check(cPlanetController);

	PlanetHUD = Cast<APlanetHUD>(cPlanetController->GetHUD());
	check(PlanetHUD);

	check(PlanetMesh);
	PlanetMesh->SetRelativeRotation(FRotator(0, DEGREES_PER_DAY / 2, 0));

	mCurrentRotationSpeed = DefaultRotationSpeed;
	cPlanetController->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
	{
		check(cPlanetController);
		updatePlanetRotationSpeed(cPlanetController->WorldMouseLocation);
	});
}

void APlanetPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if(!bIsMouseMoving)
	{
		mCurrentRotationSpeed = FMath::FInterpTo(
			mCurrentRotationSpeed,
			DefaultRotationSpeed,
			_deltaTime,
			FrictionCoefficient
		);
	}
	bIsMouseMoving = false;

	check(PlanetMesh);
	const float deltaRotation = mCurrentRotationSpeed * _deltaTime;
	const FRotator newRotation = FRotator(
		0.0f,
		PlanetMesh->GetRelativeRotation().Yaw + deltaRotation,
		0.0f
	);

	PlanetMesh->SetRelativeRotation(newRotation);

	check(DayOfWeek);
	DayOfWeek->UpdateRotation(newRotation.Yaw);
}

void APlanetPawn::StartAim()
{
	check(PlayCamera);
	PlayCamera->StartAim();

	if (IsValid(mCurrentAimEffect))
	{
		FadeOutNiagaraComponent(GetWorld(), *mCurrentAimEffect, 2.0f);
		mCurrentAimEffect = nullptr;
	}
	check(AimEffectTemplate);
	mCurrentAimEffect = SpawnSystemAttachedFacingForward(AimEffectTemplate, PlanetMesh, false);

	bPlayerAiming = true;
}

void APlanetPawn::StopAim()
{
	check(PlayCamera);
	PlayCamera->StopAim();

	if (IsValid(mCurrentAimEffect))
	{
		FadeOutNiagaraComponent(GetWorld(), *mCurrentAimEffect, 2.0f);
		mCurrentAimEffect = nullptr;
	}

	bPlayerAiming = false;
}

void APlanetPawn::HandleDied()
{
	SpawnSystemFacingForward(PlanetDieTemplate, this);
	PlanetHUD->ShowLoseGame();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void APlanetPawn::composeComponent()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = Capsule;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet Mesh"));
	PlanetMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	LookPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Look Point"));
	LookPoint->SetupAttachment(RootComponent);

	PlayCamera		= CreateDefaultSubobject<UPlayCamera>(TEXT("Play Camera"));
	OrbitMover		= CreateDefaultSubobject<UOrbitMover>(TEXT("Orbit Mover"));
	HP				= CreateDefaultSubobject<UHPComponent>(TEXT("HP"));
	Shield			= CreateDefaultSubobject<UShieldComponent>(TEXT("Shield"));
	Level			= CreateDefaultSubobject<ULevelComponent>(TEXT("Level Manager"));
	RewardManager	= CreateDefaultSubobject<URewardManager>(TEXT("Reward Manager"));
	JustAimManager	= CreateDefaultSubobject<UJustAimManagerComponent>(TEXT("Just Aim Manager"));
	WeaponSlot		= CreateDefaultSubobject<UWeaponSlotComponent>(TEXT("Weapon Slot"));
	ItemSlot		= CreateDefaultSubobject<UPassiveItemSlotComponent>(TEXT("Item Slot"));
	DayOfWeek		= CreateDefaultSubobject<UDayOfWeekComponent>(TEXT("Day Of Week"));
}

void APlanetPawn::updatePlanetRotationSpeed(const FVector& _worldMousePosition)
{
	const FRotator currentRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), 
		_worldMousePosition
	);

	LookPoint->SetWorldRotation(currentRotation);
	
	const float currentYaw = currentRotation.Yaw;
	const float deltaYaw = currentYaw - PreviousYaw;
	RotationSpeedMultiplier = FMath::Clamp(EaseInCubic(deltaYaw * MouseYawCoefficient) + 1, MinSpeedMultiplier, MaxSpeedMultiplier);
	mCurrentRotationSpeed = DefaultRotationSpeed * RotationSpeedMultiplier;

	PreviousYaw = currentYaw;
	bIsMouseMoving = true;

	WeeklyPointAngle = FMath::Modulo(WeeklyPointAngle + deltaYaw, DEGREES_PER_WEEK);
}

void APlanetPawn::resetToDefaultSettings()
{
	RuntimeSettings.HP				= BaseSettings->HPBase;
	RuntimeSettings.Damage			= BaseSettings->DamageBase;
	RuntimeSettings.Critical		= BaseSettings->CriticalBase;
	RuntimeSettings.CriticalDamage	= BaseSettings->CriticalBase;
	RuntimeSettings.Haste			= BaseSettings->HasteBase;
	RuntimeSettings.XpGain			= BaseSettings->XPGainBase;
	RuntimeSettings.XpSpeed			= BaseSettings->XPSpeedBase;

	if (HP)
	{
		HP->Initialize();
	}
}

// [Obsolete]
void APlanetPawn::updatePlanetRotation(const FVector& _worldMousePosition) const
{
	const FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), 
		_worldMousePosition
	);

	PlanetMesh->SetWorldRotation(FRotator(0, newRotation.Yaw, 0));

	check(DayOfWeek);
	DayOfWeek->UpdateRotation(newRotation.Yaw);
}