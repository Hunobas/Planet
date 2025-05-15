// OrbitMover.cpp
#include "OrbitMover.h"

#include "../Planet.h"

UOrbitMover::UOrbitMover(): cTargetSun(nullptr), mOwner(nullptr), mOrbitRadius(0), mOrbitPeriod(PLAYTIME), mCurrentAngle(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitMover::BeginPlay()
{
	Super::BeginPlay();

	mOwner = GetOwner();
	check(mOwner);

	if (SunTag != NAME_None)
	{
		if (TryGetFirstActorWithTag(GetWorld(), SunTag, cTargetSun))
		{
			mTargetLocation = cTargetSun->GetActorLocation();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] '%s' 태그를 가진 액터를 찾지 못했습니다."), 
				*mOwner->GetName(), *SunTag.ToString());
		}
	}
	
	mOrbitRadius = (mTargetLocation - mOwner->GetActorLocation()).Size();
	mOrbitPeriod = PLAYTIME / NumOrbits;
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

	mOwner->SetActorLocation(newPos);
}
