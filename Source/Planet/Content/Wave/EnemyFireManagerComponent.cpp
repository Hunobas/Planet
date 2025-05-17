// EnemyFireManagerComponent.h
#include "EnemyFireManagerComponent.h"

#include "Camera/CameraComponent.h"

#include "EnemyPawn.h"
#include "EnemySpawnCelestial.h"
#include "FiringComponent.h"

UEnemyFireManagerComponent::UEnemyFireManagerComponent(): mEnemySpawn(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

UEnemyFireManagerComponent* UEnemyFireManagerComponent::Initialize(AEnemySpawnCelestial* _enemySpawn)
{
	mEnemySpawn = _enemySpawn;
	return this;
}

void UEnemyFireManagerComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	UFiringComponent* currentFireComponent = nullptr;
	if (mFireComponentQueue.Peek(currentFireComponent))
	{
		currentFireComponent->HandleJustAim();

// #ifdef DEBUG
		if (APawn* cPlayerPawn = currentFireComponent->TargetPawn)
		{
			const FVector PlayerForwardPos = cPlayerPawn->GetActorLocation() + mEnemySpawn->PlayerCamera->GetForwardVector() * mEnemySpawn->EnemySpawnRadius;
			DrawDebugLine(
				GetWorld(),
				PlayerForwardPos,
				currentFireComponent->MuzzlePoint->GetComponentLocation(),
				FColor::Yellow,
				false,
				0.1f,
				0,
				5.0f
			);
		}
// #endif
	}
}

void UEnemyFireManagerComponent::AddEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint)
{
	if (_spawnedEnemy->EnemyType != EEnemyType::Ranged)
		return;

	_spawnedEnemy->AttachToComponent(_spawnPoint, FAttachmentTransformRules::KeepWorldTransform);
	mEnemySpawn->SetOccupiedSpawnPoint(_spawnPoint, true);
	mRangedEnemies.Add(_spawnedEnemy);
}

void UEnemyFireManagerComponent::RemoveEnemy(AEnemyPawn* _deadEnemy)
{
	if (_deadEnemy->EnemyType != EEnemyType::Ranged)
		return;

	USceneComponent* spawnPoint = _deadEnemy->GetRootComponent()->GetAttachParent();
	check(spawnPoint);
	
	mEnemySpawn->SetOccupiedSpawnPoint(spawnPoint, false);
	mRangedEnemies.RemoveSingleSwap(_deadEnemy);
}

void UEnemyFireManagerComponent::EnqueueFireComponent(UFiringComponent* _fireComponent)
{
	FScopeLock Lock(&mQueueCriticalSection);
	mFireComponentQueue.Enqueue(_fireComponent);
	mActiveFireComponents.Add(_fireComponent);
}

void UEnemyFireManagerComponent::DequeueFireComponent(const UFiringComponent* _firedComponent)
{
	FScopeLock Lock(&mQueueCriticalSection);
	
	UFiringComponent* result;
	mFireComponentQueue.Dequeue(result);
	mActiveFireComponents.Remove(result);

	checkf(result == _firedComponent, TEXT("[UEnemyFireManagerComponent] dQ 실패: 예상 %s, 실제 %s"), 
			*_firedComponent->GetName(), *GetNameSafe(result));
}

void UEnemyFireManagerComponent::FireRandomFacingEnemy()
{
	TArray<AEnemyPawn*> facingEnemies = getPlayerFacingEnemies();

	TArray<UFiringComponent*> allFirePoints;
	for (AEnemyPawn* enemy : facingEnemies)
	{
		TArray<UFiringComponent*> firePoints;
		enemy->GetComponents<UFiringComponent>(firePoints);
		allFirePoints.Append(firePoints);
	}

	TArray<UFiringComponent*> availableFirePoints;
	for (UFiringComponent* fp : allFirePoints)
	{
		if (!mActiveFireComponents.Contains(fp))
		{
			availableFirePoints.Add(fp);
		}
	}

	if (availableFirePoints.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("[EnemyFireManagerComponent] 가용한 fire point가 없음."));
		return;
	}

	UFiringComponent* selectedFirePoint = availableFirePoints[FMath::RandRange(0, availableFirePoints.Num()-1)];
	EnqueueFireComponent(selectedFirePoint);
	selectedFirePoint->StartFireSequence([this](const UFiringComponent* _firedComponent){
		DequeueFireComponent(_firedComponent);
	});
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