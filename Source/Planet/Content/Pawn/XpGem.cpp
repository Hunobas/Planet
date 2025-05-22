// XPGem.cpp
#include "XpGem.h"

#include "../Planet.h"
#include "ObjectPoolManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PlanetPawn.h"
#include "FollowMover.h"

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
	TryGetFirstComponentWithTag(this, FOLLOW_MOVER_TAG, mFollowMover);

	if (Capsule)
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AXpGem::OnOverlapBegin);
	}

	return this;
}

void AXpGem::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (mFollowMover)
	{
		mFollowMover->MoveStep(_deltaTime);
	}
}

void AXpGem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(cTargetPlayer);
	check(mPool);

	if (Cast<APlanetPawn>(OtherActor) != cTargetPlayer)
		return;
	
	// cTargetPlayer->GainExperience(XP);
	SpawnSystemFacingForward(GainTemplate, this);
	mPool->Release(this);
}
