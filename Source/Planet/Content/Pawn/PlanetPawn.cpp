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
	FVector WorldLocation, WorldDirection;
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		const FVector PlaneNormal = FVector::UpVector;
		const FVector PlaneOrigin = GetActorLocation();
		const FVector HitLocation = FMath::LinePlaneIntersection(
			WorldLocation, 
			WorldLocation + WorldDirection * 10000.0f, 
			PlaneOrigin, 
			PlaneNormal
		);
		const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(), 
			HitLocation
		);
		
		PlanetMesh->SetWorldRotation(FRotator(0, NewRotation.Yaw, 0));

#ifdef DEBUG
		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			HitLocation,
			FColor::Green,
			false,
			0.1f,
			0,
			2.0f
		);
#endif
	}
}