// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlayCamera.h"
#include "OrbitMover.h"
#include "HPComponent.h"
#include "JustAimManagerComponent.h"
#include "PlanetController.h"
#include "PlayerDataAsset.h"

APlanetPawn::APlanetPawn() : cPlanetController(nullptr), mHP(nullptr)
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

	PlayCamera = CreateDefaultSubobject<UPlayCamera>(TEXT("Play Camera"));
	OrbitMover = CreateDefaultSubobject<UOrbitMover>(TEXT("Orbit Mover"));
	JustAimManager = CreateDefaultSubobject<UJustAimManagerComponent>(TEXT("Just Aim Manager"));
	mHP = CreateDefaultSubobject<UHPComponent>(TEXT("HP"));
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

	if (mHP)
	{
		mHP->Initialize();
	}
}
