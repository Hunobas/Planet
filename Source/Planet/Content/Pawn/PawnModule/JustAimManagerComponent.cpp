// JustAimManagerComponent.cpp
#include "JustAimManagerComponent.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemySpawnCelestial.h"
#include "PlanetController.h"
#include "PlayCamera.h"

UJustAimManagerComponent::UJustAimManagerComponent(): JustAimSuccessTemplate(nullptr), cPlayerPawn(nullptr),
                                                      mDistanceToSpawnCelestial(2000.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UJustAimManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	cPlayerPawn = Cast<APlanetPawn>(GetOwner());
	check(cPlayerPawn);

	AEnemySpawnCelestial* enemySpawn;
	TryGetFirstActorWithTag(GetWorld(), DEFAULT_ENEMY_SPAWN_TAG, enemySpawn);
	check(enemySpawn);

	mDistanceToSpawnCelestial = enemySpawn->EnemySpawnRadius;
}

bool UJustAimManagerComponent::HasJustAimed(const USceneComponent* _firePoint) const
{
	check(cPlayerPawn);
	APlanetController* playerController = Cast<APlanetController>(cPlayerPawn->GetController());
	
	check(_firePoint);
	const FVector mouseDir	= (playerController->MouseHoverLocation - cPlayerPawn->GetActorLocation()).GetSafeNormal();
	const FVector fireDir	= (_firePoint->GetComponentLocation() - cPlayerPawn->GetActorLocation()).GetSafeNormal();
	
	const float angleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(mouseDir, fireDir)));

	return angleBetween < JustAimWindowAngle;
}

void UJustAimManagerComponent::SucceedJustAim(USceneComponent* _firePoint) const
{
	check(_firePoint);

	cPlayerPawn->PlayCamera->OnJustAimSuccess(_firePoint->GetComponentLocation());
	
	SpawnSystemAttachedFacingForward(JustAimSuccessTemplate, _firePoint);
}

void UJustAimManagerComponent::FailJustAim()
{
}

