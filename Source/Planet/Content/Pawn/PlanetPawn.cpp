// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"

#include "PlayCamera.h"
#include "OrbitMover.h"
#include "PlanetController.h"
#include "EnemySpawnCelestial.h"

#define DEBUG

APlanetPawn::APlanetPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	composeComponent();
}

void APlanetPawn::BeginPlay()
{
	Super::BeginPlay();

	mPlanetController = Cast<APlanetController>(GetController());
	
	check(EnemySpawnClass);
	GetWorld()->SpawnActor<AEnemySpawnCelestial>(EnemySpawnClass, GetActorLocation(), GetActorRotation())->Initialize(this);
}

void APlanetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayCamera->UpdateSocketOffY();
	PlayCamera->UpdateArmLength(DeltaTime);
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
	const FVector CameraLoc    = Camera->GetComponentLocation();
	const FVector CameraForward= Camera->GetForwardVector();
	const FVector TargetLoc    = CameraLoc + CameraForward * VisibleDistance;

#ifdef DEBUG
	DrawDebugLine(
		GetWorld(),               // 월드
		CameraLoc,                // 시작점
		TargetLoc,                // 끝점
		FColor::Red,              // 색상
		false,                    // bPersistentLines
		0.1f,                     // LifeTime (0.1초)
		0,                        // DepthPriority
		0.1f                      // Thickness
	);
#endif

	const FVector MeshLoc      = PlanetMesh->GetComponentLocation();
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MeshLoc, TargetLoc);

	PlanetMesh->SetWorldRotation(LookAtRot);
}