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
		TArray<AActor*> foundSuns;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Sun"), foundSuns);
		if (foundSuns.Num() > 0)
		{
			TargetSun = foundSuns[0];
			mTargetLocation = TargetSun->GetActorLocation();
		}
	}
	
	mOrbitRadius = (mTargetLocation - cOwner->GetActorLocation()).Size();
	mOrbitPeriod = PlanetConst::PLAYTIME / NumOrbits;
	mCurrentAngle = 0.0f;
}

void UOrbitMover::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	moveStep(_deltaTime);
}

void UOrbitMover::moveStep(float _deltaTime)
{
	mCurrentAngle += (_deltaTime / mOrbitPeriod) * 360.0f;
	mCurrentAngle = FMath::Fmod(mCurrentAngle, 360.0f);
	
	const float rad		 = FMath::DegreesToRadians(mCurrentAngle);
	const float X		 = mTargetLocation.X + mOrbitRadius * FMath::Cos(rad);
	const float Y		 = mTargetLocation.Y + mOrbitRadius * FMath::Sin(rad);
	const FVector newPos = FVector(X, Y, mTargetLocation.Z);

	cOwner->SetActorLocation(newPos);
}
