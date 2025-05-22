// EnemyPawn.h
#include "EnemyPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"

#include "EnemySetting.h"
#include "EnemyScaleSetting.h"
#include "EnemyDataAsset.h"
#include "ContinuousUpdateStrategy.h"
#include "InputDrivenUpdateStrategy.h"
#include "PlanetController.h"
#include "FlyingMover.h"
#include "FollowMover.h"
#include "HPComponent.h"
#include "WaveManagerComponent.h"
#include "ObjectPoolManagerComponent.h"
#include "XpGem.h"

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
	mHP = nullptr;
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	if (BodyCollisionCapsule)
	{
		BodyCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn::OnOverlapBegin);
	}

	TryGetFirstComponentWithTag(this, FLYING_MOVER_TAG, mFlyingMover);
	TryGetFirstComponentWithTag(this, FOLLOW_MOVER_TAG, mFollowMover);
	TryGetFirstComponentWithTag(this, HP_TAG, mHP);
}

void AEnemyPawn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (mUpdateStrategy.IsValid())
	{
		mUpdateStrategy->Update(_deltaTime);
	}
}

void AEnemyPawn::MoveStep(float _deltaTime)
{
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

	if (mHP)
	{
		mHP->Initialize();
	}

	setUpdateStrategy();
}

void AEnemyPawn::HandleDied()
{
	SpawnSystemFacingForward(EnemyDieTemplate, this);
	
	if (ASurvivorGameModeBase* gm = GetPlanetGameMode(this))
	{
		if (mHP && mHP->CurrentHP <= 0.0f)
		{
			spawnXpGem();
		}
		
		gm->WaveManager->EnemyDied(this);
	}

	mUpdateStrategy.Reset();
	cTargetPawn = nullptr;
}

void AEnemyPawn::OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	check(cTargetPawn);

	if (EnemyType != EEnemyType::Melee || Cast<APawn>(_otherActor) != cTargetPawn)
		return;
	
	FDamageEvent damageEvent;
	cTargetPawn->TakeDamage(RuntimeSettings.Damage, damageEvent, GetController(), this);
	HandleDied();
}

void AEnemyPawn::setUpdateStrategy()
{
	if (UpdateType == EUpdateType::Continuous)
	{
		mUpdateStrategy = MakeUnique<ContinuousUpdateStrategy>(this);
	}
	else if (UpdateType == EUpdateType::InputDriven)
	{
		check(cTargetPawn);
		
		mUpdateStrategy = MakeUnique<InputDrivenUpdateStrategy>(this);

		if (APlanetController* PC = Cast<APlanetController>(cTargetPawn->GetController()))
		{
			PC->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
			{
				if (mUpdateStrategy.IsValid())
				{
					mUpdateStrategy->OnLookInput(_inputValue * 0.05f);
				}
			});
		}
	}
}

void AEnemyPawn::spawnXpGem() const
{
	UObjectPoolManagerComponent* pool = GetObjectPoolManager(this);
	
	if (AXpGem* XpGem = pool->AcquireOrNull(XpGemClass, GetActorTransform()))
	{
		XpGem->Initialize(cTargetPawn, RuntimeSettings.XPDrop, pool);
	}
}
