// EnemyFireManagerComponent.h
#include "EnemyFireManagerComponent.h"

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
	mFireComponentQueue.Empty();
	return this;
}

void UEnemyFireManagerComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	UFiringComponent* fp = nullptr;
	if (mFireComponentQueue.Peek(fp))
	{
		fp->HandleJustAim();

#ifdef DEBUG
		if (APawn* cPlayerPawn = fp->TargetPawn)
		{
			DrawDebugLine(
				GetWorld(),
				cPlayerPawn->GetActorLocation(),
				fp->MuzzlePoint->GetComponentLocation(),
				FColor::Yellow,
				false,
				0.1f,
				0,
				5.0f
			);
		}
#endif
	}
}

void UEnemyFireManagerComponent::FireOnBeat()
{
	if (UFiringComponent* fp = getNextPointOrNull())
	{
		fp->StopFireSequence();
		EnqueueFireComponent(fp);
		fp->StartFireSequence([this](const UFiringComponent* _firedComponent){
			DequeueFireComponent(_firedComponent);
		});
	}
}

void UEnemyFireManagerComponent::FireOnBeatAt(const int32 _index)
{
	if (UFiringComponent* fp = getPointAtIndexOrNull(_index))
	{
		fp->StopFireSequence();
		EnqueueFireComponent(fp);
		fp->StartFireSequence([this](const UFiringComponent* _firedComponent){
			DequeueFireComponent(_firedComponent);
		});
	}
}

void UEnemyFireManagerComponent::RegisterRangedEnemy(AEnemyPawn* _spawnedEnemy, USceneComponent* _spawnPoint)
{
	if (_spawnedEnemy->EnemyType != EEnemyType::Ranged)
		return;

	_spawnedEnemy->AttachToComponent(_spawnPoint, FAttachmentTransformRules::KeepWorldTransform);
	mEnemySpawn->SetOccupiedSpawnPoint(_spawnPoint, true);
	mRangedEnemies.AddUnique(_spawnedEnemy);
}

void UEnemyFireManagerComponent::UnregisterRangedEnemy(AEnemyPawn* _deadEnemy)
{
	if (_deadEnemy->EnemyType != EEnemyType::Ranged)
		return;

	if (UFiringComponent* fp = _deadEnemy->GetComponentByClass<UFiringComponent>())
	{
		fp->StopFireSequence();
	}

	USceneComponent* spawnPoint = _deadEnemy->GetRootComponent()->GetAttachParent();
	check(spawnPoint);
	
	mEnemySpawn->SetOccupiedSpawnPoint(spawnPoint, false);
	mRangedEnemies.RemoveSingle(_deadEnemy);
}

void UEnemyFireManagerComponent::EnqueueFireComponent(UFiringComponent* _fireComponent)
{
	FScopeLock lock(&mQueueCriticalSection);
	mFireComponentQueue.Enqueue(_fireComponent);
}

void UEnemyFireManagerComponent::DequeueFireComponent(const UFiringComponent* _firedComponent)
{
	FScopeLock lock(&mQueueCriticalSection);
	UFiringComponent* result;
	mFireComponentQueue.Dequeue(result);

	if (result != _firedComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("[UEnemyFireManagerComponent] dQ 실패: 예상 %s, 실제 %s"), 
				*_firedComponent->GetName(), *GetNameSafe(result));
	}
}

UFiringComponent* UEnemyFireManagerComponent::getNextPointOrNull()
{
	if (mRangedEnemies.IsEmpty())
		return nullptr;
    
	const int32 safeIndex = mCurrentFireIndex++ % mRangedEnemies.Num();
	AEnemyPawn* enemy = mRangedEnemies[safeIndex];
    
	if (isEnemyValidForFiring(enemy))
	{
		if (UFiringComponent* firingComp = enemy->GetComponentByClass<UFiringComponent>())
		{
			if (!firingComp->bIsCurrentlyFiring)
			{
				return firingComp;
			}
		}
	}
    
	return nullptr;
}

UFiringComponent* UEnemyFireManagerComponent::getPointAtIndexOrNull(const int32 _index) const
{
	if (mRangedEnemies.IsEmpty())
		return nullptr;
    
	const int32 safeIndex = _index % mRangedEnemies.Num();
	AEnemyPawn* enemy = mRangedEnemies[safeIndex];
    
	if (isEnemyValidForFiring(enemy))
	{
		if (UFiringComponent* firingComp = enemy->GetComponentByClass<UFiringComponent>())
		{
			if (!firingComp->bIsCurrentlyFiring)
			{
				return firingComp;
			}
		}
	}
    
	return nullptr;
}

bool UEnemyFireManagerComponent::isEnemyValidForFiring(const AEnemyPawn* _enemy)
{
	return _enemy && 
	   _enemy->IsValidLowLevel() && 
	   !_enemy->IsActorBeingDestroyed() &&
	   _enemy->IsActorTickEnabled() &&
	   _enemy->EnemyType == EEnemyType::Ranged;
}
