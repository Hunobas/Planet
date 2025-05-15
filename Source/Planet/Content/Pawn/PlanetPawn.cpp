// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlayCamera.h"
#include "OrbitMover.h"
#include "JustAimManagerComponent.h"

APlanetPawn::APlanetPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	composeComponent();
}

void APlanetPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	updatePlanetRotation();
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
}

void APlanetPawn::updatePlanetRotation() const
{
	const FVector cameraLocation	= Camera->GetComponentLocation();
	const FVector cameraForward		= Camera->GetForwardVector();
	const FVector targetLoc			= cameraLocation + cameraForward * VisibleDistance;
	const FVector meshLocation		= PlanetMesh->GetComponentLocation();
	const FRotator lookAtRotation	= UKismetMathLibrary::FindLookAtRotation(meshLocation, targetLoc);

#ifdef DEBUG
	DrawDebugLine(
		GetWorld(),
		cameraLocation,
		targetLoc,
		FColor::Green,
		false,
		0.05f,
		0,
		0.05f
	);
#endif

	PlanetMesh->SetWorldRotation(lookAtRotation);
}