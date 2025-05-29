// XPGem.cpp
#include "XpGem.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PlanetPawn.h"
#include "FollowMover.h"
#include "LevelComponent.h"

AXpGem::AXpGem() : cTargetPlayer(nullptr), mPool(nullptr), mFollowMover(nullptr), mCurrentSpeed(10.0f), mCachedAttractCosAngle(0.0f), mMoveSpeedUpdateInterval(0.0f)
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
	check(Capsule);
	Capsule->OnComponentBeginOverlap.RemoveAll(this);
	
	mMoveSpeedUpdateInterval = 0.0f;
}

void AXpGem::updateMoveSpeed(float _deltaTime)
{
	mCurrentSpeed = BaseMoveSpeed;

	check(cTargetPlayer);
	if (!cTargetPlayer->bPlayerAiming)
		return;
    
	mMoveSpeedUpdateInterval += _deltaTime;
	if (mMoveSpeedUpdateInterval >= UPDATE_NORMAL_INTERVAL)
	{
		mCachedPlayerLocation = cTargetPlayer->GetActorLocation();
		mCachedPlayerForward = cTargetPlayer->PlanetMesh->GetForwardVector();
		mMoveSpeedUpdateInterval = 0.0f;
	}
    
	const FVector directionToGem = (GetActorLocation() - mCachedPlayerLocation).GetSafeNormal();
	const float dotProduct = FVector::DotProduct(directionToGem, mCachedPlayerForward);
    
	if (dotProduct >= mCachedAttractCosAngle)
	{
		mCurrentSpeed = CalculateXPSpeed(BaseMoveSpeed, cTargetPlayer->RuntimeSettings.XpSpeed);
	}
}
