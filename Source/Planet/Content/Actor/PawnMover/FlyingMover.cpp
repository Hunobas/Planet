// FlyingMover.cpp
#include "FlyingMover.h"

#include "Components/StaticMeshComponent.h"

#include "../Planet.h"

UFlyingMover::UFlyingMover(): mOwner(nullptr), mOwnerMesh(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyingMover::BeginPlay()
{
	Super::BeginPlay();

	mOwner = GetOwner();
	check(mOwner);

	mElapsedTime += NeedRandomStart ? FMath::RandRange(0.0f, 2 * PI) : 0.0f;

	TryGetFirstComponentWithTag(mOwner, OwnerMeshTag, mOwnerMesh);
}

void UFlyingMover::MoveStep(float _deltaTime)
{
	mElapsedTime += _deltaTime * MoveSpeed;
	
	const FVector offset = FVector(
		Amplitude * FMath::Sin(mElapsedTime),
		Amplitude * FMath::Sin(2 * mElapsedTime),
		0
	);

	check(mOwnerMesh);
	mOwnerMesh->SetRelativeLocation(offset);
}