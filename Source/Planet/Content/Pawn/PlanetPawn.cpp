// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlayCamera.h"
#include "OrbitMover.h"
#include "PlanetController.h"
#include "EnemySpawnCelestial.h"

APlanetPawn::APlanetPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	composeComponent();
}

void APlanetPawn::BeginPlay()
{
	Super::BeginPlay();

	cPlanetController = Cast<APlanetController>(GetController());
	
	check(EnemySpawnClass);
	GetWorld()->SpawnActor<AEnemySpawnCelestial>(EnemySpawnClass, GetActorLocation(), GetActorRotation())->Initialize(this);
}

void APlanetPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	PlayCamera->UpdateSocketOffY();
	PlayCamera->UpdateArmLength(_deltaTime);
	updatePlanetRotation();
}

void APlanetPawn::composeComponent()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = Capsule;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet Mesh"));
	PlanetMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	PlayCamera = CreateDefaultSubobject<UPlayCamera>("Play Camera")->Initialize(this, SpringArm, Camera);
	OrbitMover = CreateDefaultSubobject<UOrbitMover>("Orbit Mover");
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
		FColor::Red,
		false,
		0.1f,
		0,
		0.1f
	);
#endif

	PlanetMesh->SetWorldRotation(lookAtRotation);
}