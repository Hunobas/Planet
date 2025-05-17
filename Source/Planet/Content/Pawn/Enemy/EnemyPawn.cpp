// EnemyPawn.h
#include "EnemyPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "EnemySetting.h"
#include "EnemyScaleSetting.h"
#include "EnemyDataAsset.h"
#include "FlyingMover.h"
#include "FollowMover.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body Collision Capsule"));
	RootComponent = BodyCollisionCapsule;
	
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh"));
	EnemyMesh->SetupAttachment(RootComponent);

	HitDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Detection Capsule"));
	HitDetectionCapsule->SetupAttachment(EnemyMesh);

	mFlyingMover = nullptr;
	mFollowMover = nullptr;
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	TryGetFirstComponentWithTag(this, FLYING_MOVER_TAG, mFlyingMover);
	TryGetFirstComponentWithTag(this, FOLLOW_MOVER_TAG, mFollowMover);
}

void AEnemyPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (mFollowMover)
	{
		mFollowMover->MoveStep(_deltaTime);
	}
	if (mFlyingMover)
	{
		mFlyingMover->MoveStep(_deltaTime);
	}

	if (cTargetPawn)
	{
		FVector direction = (cTargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorRotation(direction.Rotation());
	}
}

void AEnemyPawn::ResetToDefaultSettings(const FEnemyScaleSetting& _scaleSettings, APawn* _targetPlayer)
{
	cTargetPawn = _targetPlayer;
	
	RuntimeSettings.HP			= BaseSettings->HPBase * _scaleSettings.HPScale;
	RuntimeSettings.Damage		= BaseSettings->DamageBase * _scaleSettings.DamageScale;
	RuntimeSettings.Speed		= BaseSettings->SpeedBase * _scaleSettings.SpeedScale;
	RuntimeSettings.XPDrop		= BaseSettings->XPDropBase * _scaleSettings.XPDropScale;
	RuntimeSettings.FieldScore	= BaseSettings->FieldScoreBase;
}
