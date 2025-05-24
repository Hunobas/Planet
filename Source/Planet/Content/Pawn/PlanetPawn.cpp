// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlanetController.h"
#include "PlanetHUD.h"
#include "PlayCamera.h"
#include "OrbitMover.h"
#include "JustAimManagerComponent.h"
#include "RewardManager.h"
#include "LevelManager.h"
#include "WeaponSlotComponent.h"
#include "PassiveItemSlotComponent.h"
#include "HPComponent.h"
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

	cPlanetController->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
	{
		updatePlanetRotation(cPlanetController->WorldMouseLocation);
	});
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
	LevelManager	= CreateDefaultSubobject<ULevelManager>(TEXT("Level Manager"));
	RewardManager	= CreateDefaultSubobject<URewardManager>(TEXT("Reward Manager"));
	WeaponSlot		= CreateDefaultSubobject<UWeaponSlotComponent>(TEXT("Weapon Slot"));
	ItemSlot		= CreateDefaultSubobject<UPassiveItemSlotComponent>(TEXT("Item Slot"));
	mHP				= CreateDefaultSubobject<UHPComponent>(TEXT("HP"));
}

void APlanetPawn::updatePlanetRotation(const FVector& _worldMousePosition) const
{
	const FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), 
		_worldMousePosition
	);
	
	PlanetMesh->SetWorldRotation(FRotator(0, newRotation.Yaw, 0));

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
	RuntimeSettings.XpSpeed	= BaseSettings->XPSpeedScaleBase;

	if (mHP)
	{
		mHP->Initialize();
	}
}
