// OrbitMover.cpp
#include "OrbitMover.h"

#include "Kismet/GameplayStatics.h"

#include "PlanetConst.h"

UOrbitMover::UOrbitMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitMover::BeginPlay()
{
	Super::BeginPlay();

	cOwner = GetOwner();
	check(cOwner);

	if (!TargetSun)
	{
		TArray<AActor*> FoundSuns;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Sun"), FoundSuns);
		if (FoundSuns.Num() > 0)
		{
			TargetSun = FoundSuns[0];
			mTargetLocation = TargetSun->GetActorLocation();
		}
	}
	
	mOrbitRadius = (mTargetLocation - cOwner->GetActorLocation()).Size();
	mOrbitPeriod = PlanetConst::PLAYTIME / NumOrbits;
	mCurrentAngle = 0.0f;
}

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

	float X = mTargetLocation.X + mOrbitRadius * FMath::Cos(Rad);
	float Y = mTargetLocation.Y + mOrbitRadius * FMath::Sin(Rad);
	FVector NewPos = FVector(X, Y, mTargetLocation.Z);

	cOwner->SetActorLocation(NewPos);
}
