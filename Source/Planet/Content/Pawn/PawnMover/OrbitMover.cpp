// OrbitMover.cpp
#include "OrbitMover.h"

#include "../Planet.h"

UOrbitMover::UOrbitMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitMover::BeginPlay()
{
	Super::BeginPlay();

	cOwner = GetOwner();
	check(cOwner);

	if (SunTag != NAME_None)
	{
		if (TryGetFirstActorWithTag(SunTag, cTargetSun))
		{
			mTargetLocation = cTargetSun->GetActorLocation();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] '%s' 태그를 가진 액터를 찾지 못했습니다."), 
				*cOwner->GetName(), *SunTag.ToString());
		}
	}
	
	mOrbitRadius = (mTargetLocation - cOwner->GetActorLocation()).Size();
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

	cOwner->SetActorLocation(newPos);
}
