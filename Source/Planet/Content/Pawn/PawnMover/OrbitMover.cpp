// OrbitMover.cpp
#include "OrbitMover.h"

#include "../../../System/PlanetConst.h"

// Sets default values for this component's properties
UOrbitMover::UOrbitMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOrbitMover::BeginPlay()
{
	Super::BeginPlay();

	mOwner = GetOwner();
	check(TargetSun && mOwner);

	mOrbitRadius = (TargetSun->GetActorLocation() - mOwner->GetActorLocation()).Size();
	mOrbitPeriod = PlanetConst::PLAYTIME;
	mCurrentAngle = 0.0f;
}

// Called every frame
void UOrbitMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	moveStep(DeltaTime);
}

void UOrbitMover::moveStep(float DeltaTime)
{
	mCurrentAngle += (DeltaTime / mOrbitPeriod) * 360.0f;
	mCurrentAngle = FMath::Fmod(mCurrentAngle, 360.0f);
	const float Rad = FMath::DegreesToRadians(mCurrentAngle);

	FVector Center = TargetSun->GetActorLocation();
	float X = Center.X + mOrbitRadius * FMath::Cos(Rad);
	float Y = Center.Y + mOrbitRadius * FMath::Sin(Rad);
	FVector NewPos = FVector(X, Y, Center.Z);

	mOwner->SetActorLocation(NewPos);
}
