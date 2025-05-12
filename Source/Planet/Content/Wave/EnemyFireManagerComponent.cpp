// EnemyFireManagerComponent.h
#include "EnemyFireManagerComponent.h"

#include "Camera/CameraComponent.h"

#include "EnemyPawn.h"
#include "EnemySpawnCelestial.h"
#include "FiringComponent.h"

UEnemyFireManagerComponent::UEnemyFireManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

UEnemyFireManagerComponent* UEnemyFireManagerComponent::Initialize(AEnemySpawnCelestial* _enemySpawn)
{
	mEnemySpawn = _enemySpawn;
	return this;
}

void UEnemyFireManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UEnemyFireManagerComponent::AddEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint)
{
	if (_spawnedEnemy->EnemyType != EEnemyType::Ranged)
		return;

	_spawnedEnemy->AttachToComponent(_spawnPoint, FAttachmentTransformRules::KeepWorldTransform);
	mEnemySpawn->SetActiveSpawnPoint(_spawnPoint, false);
	mRangedEnemies.Add(_spawnedEnemy);
}

void UEnemyFireManagerComponent::RemoveEnemy(AEnemyPawn* _deadEnemy)
{
	if (_deadEnemy->EnemyType != EEnemyType::Ranged)
		return;

	USceneComponent* spawnPoint = _deadEnemy->GetRootComponent()->GetAttachParent();
	check(spawnPoint);
	
	mEnemySpawn->SetActiveSpawnPoint(spawnPoint, true);
	mRangedEnemies.RemoveSingleSwap(_deadEnemy);
}

UFiringComponent* UEnemyFireManagerComponent::FireRandomFacingEnemy()
{
	TArray<AEnemyPawn*> facingEnemies = getPlayerFacingEnemies();

	if (facingEnemies.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No facing enemies found"));
		return nullptr;
	}

	AEnemyPawn* enemy = facingEnemies[FMath::RandRange(0, facingEnemies.Num() - 1)];

	TArray<UFiringComponent*> firePoints;
	enemy->GetComponents<UFiringComponent>(firePoints);
	check(!firePoints.IsEmpty());

	UFiringComponent* firePoint = firePoints[FMath::RandRange(0, firePoints.Num() - 1)];
	firePoint->StartFireSequence();

	return firePoint;
}

TArray<AEnemyPawn*> UEnemyFireManagerComponent::getPlayerFacingEnemies()
{
	TArray<AEnemyPawn*> facingEnemies;
	
	check(mEnemySpawn->PlayerCamera);
	float camYaw = mEnemySpawn->PlayerCamera->GetComponentRotation().Yaw;
	
	for (AEnemyPawn* enemy : mRangedEnemies)
	{
		if (!IsValid(enemy))
			continue;
		
		float enemyYaw = enemy->GetActorRotation().Yaw;
		float deltaYaw = FMath::FindDeltaAngleDegrees(camYaw, enemyYaw);

		if (FMath::Abs(deltaYaw) >= PI_IN_DEGREES - mEnemySpawn->HalfFOV)
		{
			facingEnemies.Add(enemy);
		}
	}

	return facingEnemies;
}
