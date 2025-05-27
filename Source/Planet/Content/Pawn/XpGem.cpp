// XPGem.cpp
#include "XpGem.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PlanetPawn.h"
#include "FollowMover.h"
#include "LevelManager.h"

AXpGem::AXpGem()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = Capsule;

	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell Mesh"));
	ShellMesh->SetupAttachment(Capsule);

	GemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gem Mesh"));
	GemMesh->SetupAttachment(ShellMesh);
}

AXpGem* AXpGem::Initialize(APawn* _targetPlayer, const float& _XP, UObjectPoolManagerComponent* _pool)
{
	cTargetPlayer = Cast<APlanetPawn>(_targetPlayer);
	XP = _XP;
	mPool = _pool;
	
	reset();
	TryGetFirstComponentWithTag(this, FOLLOW_MOVER_TAG, mFollowMover);

	check(cTargetPlayer);
	cTargetPlayer->OnAimStart.AddUFunction(this, FName("StartAim"));
	cTargetPlayer->OnAimRelease.AddUFunction(this, FName("StopAim"));

	check(Capsule);
	Capsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AXpGem::OnOverlapBegin);

	return this;
}

void AXpGem::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (!mFollowMover)
		return;

	mCurrentSpeed = DefaultSpeed;

	if (bIsPlayerAiming)
	{
		const FVector targetLocation	= cTargetPlayer->GetActorLocation();
		const FVector targetForward		= cTargetPlayer->PlanetMesh->GetForwardVector().GetSafeNormal();
		const FVector direction			= (GetActorLocation() - targetLocation).GetSafeNormal();
		const float angleBetween		= FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(direction, targetForward)));

		if (FMath::Abs(angleBetween) < PlayerAttractWindowAngle)
		{
			mCurrentSpeed = CalculateXPSpeed(DefaultSpeed, cTargetPlayer->RuntimeSettings.XpSpeed);
		}
	}
	
	mFollowMover->MoveSpeed = mCurrentSpeed;
	mFollowMover->MoveStep(_deltaTime);
}

void AXpGem::StartAim()
{
	bIsPlayerAiming = true;
}

void AXpGem::StopAim()
{
	bIsPlayerAiming = false;
}

void AXpGem::OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
                            UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	check(cTargetPlayer);
	check(mPool);

	if (Cast<APlanetPawn>(_otherActor) != cTargetPlayer)
		return;

	cTargetPlayer->LevelManager->GainXP(XP);
	SpawnSystemFacingForward(GainTemplate, this);
	
	reset();
	mPool->Release(this);
}

void AXpGem::reset() const
{
	cTargetPlayer->OnAimStart.RemoveAll(this);
	cTargetPlayer->OnAimRelease.RemoveAll(this);

	check(Capsule);
	Capsule->OnComponentBeginOverlap.RemoveAll(this);
}
