// EnemyPawn.h
#include "EnemyPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
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
#include "EnemyHPWidget.h"
#include "WaveManagerComponent.h"
#include "ObjectPoolManagerComponent.h"
#include "XpGem.h"

AEnemyPawn::AEnemyPawn() : cTargetPawn(nullptr), mFlyingMover(nullptr), mFollowMover(nullptr), mHP(nullptr), mHPWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body Collision Capsule"));
	RootComponent = BodyCollisionCapsule;
	
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh"));
	EnemyMesh->SetupAttachment(RootComponent);

	HitDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Detection Capsule"));
	HitDetectionCapsule->SetupAttachment(EnemyMesh);
}

void AEnemyPawn::Initialize()
{
	check(BodyCollisionCapsule);
	BodyCollisionCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEnemyPawn::OnOverlapBegin);

	TryGetFirstComponentWithTag(this, FLYING_MOVER_TAG, mFlyingMover);
	TryGetFirstComponentWithTag(this, FOLLOW_MOVER_TAG, mFollowMover);
	TryGetFirstComponentWithTag(this, HP_TAG, mHP);
	TryGetFirstComponentWithTag(this, HP_WIDGET_TAG, mHPWidget);

	if (mHP && mHPWidget)
	{
		mHP->Initialize();
		OnTakeAnyDamage.AddUniqueDynamic(this, &AEnemyPawn::HandleDamageTaken);
		mHPWidget->SetVisibility(false);
	}

	setUpdateStrategy();
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
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

	check(cTargetPawn);
	FVector direction = (cTargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	SetActorRotation(direction.Rotation());
}

void AEnemyPawn::ResetToDefaultSettings(const FEnemyScaleSetting& _scaleSettings, APawn* _targetPlayer)
{
	cTargetPawn = _targetPlayer;
	
	RuntimeSettings.HP			= BaseSettings->HPBase * _scaleSettings.HPScale;
	RuntimeSettings.Damage		= BaseSettings->DamageBase * _scaleSettings.DamageScale;
	RuntimeSettings.Speed		= BaseSettings->SpeedBase * _scaleSettings.SpeedScale;
	RuntimeSettings.XPDrop		= BaseSettings->XPDropBase * _scaleSettings.XPDropScale;
	RuntimeSettings.FieldScore	= BaseSettings->FieldScoreBase;

	Initialize();
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

void AEnemyPawn::HandleDamageTaken(AActor* _damagedActor, float _damage, const class UDamageType* _damageType,
	class AController* _instigatedBy, AActor* _damageCauser)
{
	if (!mHP || !mHPWidget)
		return;

	mHPWidget->SetVisibility(true);

	if (UEnemyHPWidget* enemyHPWidget = Cast<UEnemyHPWidget>(mHPWidget->GetUserWidgetObject()))
	{
		const float percent = mHP->CurrentHP / mHP->MaxHP;
		enemyHPWidget->UpdateHealthProgress(percent);
	}
}

void AEnemyPawn::setUpdateStrategy()
{
	if (UpdateType == EUpdateType::Continuous)
	{
		mUpdateStrategy = MakeUnique<ContinuousUpdateStrategy>(this);
	}
	else if (cTargetPawn != nullptr && UpdateType == EUpdateType::InputDriven)
	{
		mUpdateStrategy = MakeUnique<InputDrivenUpdateStrategy>(this);

		if (APlanetController* PC = Cast<APlanetController>(cTargetPawn->GetController()))
		{
			PC->OnLookValue.AddLambda([this](const FVector2D& _inputValue)
			{
				if (mUpdateStrategy.IsValid())
				{
					const FVector2D inputValue = _inputValue * InputDrivenUpdateStrategy::InputDrivenUpdateScale;
					mUpdateStrategy->OnLookInput(inputValue);
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
