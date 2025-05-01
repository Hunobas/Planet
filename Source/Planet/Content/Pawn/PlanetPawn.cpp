// PlanetPawn.cpp
#include "PlanetPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
// EnhancedInput
#include "Components/InputComponent.h"

#include "../Camera/PlayCamera.h"
#include "PlanetController.h"

// Sets default values
APlanetPawn::APlanetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	composeComponent();
}

// Called when the game starts or when spawned
void APlanetPawn::BeginPlay()
{
	Super::BeginPlay();

	mPlanetController = Cast<APlanetController>(GetController());
}

// Called every frame
void APlanetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayCamera = CreateDefaultSubobject<UPlayCamera>("PlayCameraComp")->Initialize(this, SpringArm, Camera);
}

void APlanetPawn::updatePlanetRotation() const
{
	const FVector CameraLoc    = Camera->GetComponentLocation();
	const FVector CameraForward= Camera->GetForwardVector();
	const FVector TargetLoc    = CameraLoc + CameraForward * VisibleDistance;

	const FVector MeshLoc      = PlanetMesh->GetComponentLocation();
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MeshLoc, TargetLoc);

	PlanetMesh->SetWorldRotation(LookAtRot);
}