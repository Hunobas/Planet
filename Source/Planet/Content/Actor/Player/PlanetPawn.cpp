// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

#include "PlanetController.h"
#include "PlanetHUD.h"
#include "PlayCamera.h"
#include "OrbitMover.h"
#include "JustAimManagerComponent.h"
#include "RewardManager.h"
#include "HPComponent.h"
#include "LevelComponent.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"
#include "DayOfWeekComponent.h"
#include "PlayerDataAsset.h"
#include "UObject/UnrealTypePrivate.h"

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

	cPlanetController->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
	{
		updatePlanetRotation(cPlanetController->WorldMouseLocation);
	});
}

void APlanetPawn::StartAim()		// const 시 컴파일 에러
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

	OnAimStart.Broadcast();
}

void APlanetPawn::StopAim()			// const 시 컴파일 에러
{
	check(PlayCamera);
	PlayCamera->StopAim();

	if (IsValid(mCurrentAimEffect))
	{
		FadeOutNiagaraComponent(GetWorld(), *mCurrentAimEffect, 2.0f);
		mCurrentAimEffect = nullptr;
	}

	OnAimRelease.Broadcast();
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

	PlayCamera		= CreateDefaultSubobject<UPlayCamera>(TEXT("Play Camera"));
	OrbitMover		= CreateDefaultSubobject<UOrbitMover>(TEXT("Orbit Mover"));
	JustAimManager	= CreateDefaultSubobject<UJustAimManagerComponent>(TEXT("Just Aim Manager"));
	HP				= CreateDefaultSubobject<UHPComponent>(TEXT("HP"));
	Level	= CreateDefaultSubobject<ULevelComponent>(TEXT("Level Manager"));
	RewardManager	= CreateDefaultSubobject<URewardManager>(TEXT("Reward Manager"));
	WeaponSlot		= CreateDefaultSubobject<UWeaponSlotComponent>(TEXT("Weapon Slot"));
	ItemSlot		= CreateDefaultSubobject<UPassiveItemSlotComponent>(TEXT("Item Slot"));
	DayOfWeek		= CreateDefaultSubobject<UDayOfWeekComponent>(TEXT("Day Of Week"));
}

void APlanetPawn::updatePlanetRotation(const FVector& _worldMousePosition)
{
	const FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), 
		_worldMousePosition
	);

	PlanetMesh->SetWorldRotation(FRotator(0, newRotation.Yaw, 0));

	check(DayOfWeek);
	const float yawDelta = mPreviousYaw - newRotation.Yaw;			// 반시계 자전
	DayOfWeek->UpdateRotation(yawDelta);
	mPreviousYaw = newRotation.Yaw;

#ifdef DEBUG
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		_worldMousePosition,
		FColor::Green,
		false,
		0.1f,
		0,
		2.0f
	);

	const FRotator meridianRotation = FRotator(0.0f, DayOfWeek->MeridianYaw, 0.0f);
	const FVector meridianDirection = meridianRotation.Vector();
    
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (meridianDirection * 1000.0f),
		FColor::White,
		false,
		0.1f,
		0,
		5.0f
	);
#endif
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
