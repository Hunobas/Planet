// XPGem.cpp
#include "XpGem.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PlanetPawn.h"
#include "FollowMover.h"
#include "LevelComponent.h"

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

	mCachedAttractCosAngle = FMath::Cos(FMath::DegreesToRadians(AttractAngleDegrees));
	mCurrentSpeed = BaseMoveSpeed;

	return this;
}

void AXpGem::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (!mFollowMover)
		return;

	updateMoveSpeed(_deltaTime);
	
	mFollowMover->MoveSpeed = mCurrentSpeed;
	mFollowMover->MoveStep(_deltaTime);
}

void AXpGem::StartAim()
{
	bPlayerAiming = true;
	bNeedRefreshMoveSpeed = true;
}

void AXpGem::StopAim()
{
	bPlayerAiming = false;
}

void AXpGem::OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
                            UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	check(cTargetPlayer);
	check(mPool);

	if (Cast<APlanetPawn>(_otherActor) != cTargetPlayer)
		return;

	cTargetPlayer->Level->GainXP(XP);
	SpawnSystemFacingForward(GainTemplate, this);
	
	reset();
	mPool->Release(this);
}

void AXpGem::reset()
{
	cTargetPlayer->OnAimStart.RemoveAll(this);
	cTargetPlayer->OnAimRelease.RemoveAll(this);

	check(Capsule);
	Capsule->OnComponentBeginOverlap.RemoveAll(this);

	bPlayerAiming = false;
	bNeedRefreshMoveSpeed = true;
	mMoveSpeedUpdateInterval = 0.0f;
}

void AXpGem::updateMoveSpeed(float _deltaTime)
{
	mCurrentSpeed = BaseMoveSpeed;
    
	if (!bPlayerAiming)
		return;
    
	mMoveSpeedUpdateInterval += _deltaTime;
	if (bNeedRefreshMoveSpeed || mMoveSpeedUpdateInterval >= CACHE_UPDATE_NORMAL_INTERVAL)
	{
		mCachedPlayerLocation = cTargetPlayer->GetActorLocation();
		mCachedPlayerForward = cTargetPlayer->PlanetMesh->GetForwardVector();
		mMoveSpeedUpdateInterval = 0.0f;
		bNeedRefreshMoveSpeed = false;
	}
    
	const FVector directionToGem = (GetActorLocation() - mCachedPlayerLocation).GetSafeNormal();
	const float dotProduct = FVector::DotProduct(directionToGem, mCachedPlayerForward);
    
	if (dotProduct >= mCachedAttractCosAngle)
	{
		mCurrentSpeed = CalculateXPSpeed(BaseMoveSpeed, cTargetPlayer->RuntimeSettings.XpSpeed);
	}
}
