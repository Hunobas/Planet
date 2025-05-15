// FollowMover.cpp
#include "FollowMover.h"

#include "EnemyPawn.h"

UFollowMover::UFollowMover(): mOwner(nullptr), mFollowActor(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFollowMover::BeginPlay()
{
	Super::BeginPlay();

	mOwner = GetOwner();
	check(mOwner);

	if (FollowPlayer != EAutoReceiveInput::Disabled)
	{
		mFollowActor = GetTargetPlayerPawn(FollowPlayer, this);
	}
	else if (FollowActorTag != NAME_None)
	{
		if (!TryGetFirstActorWithTag(GetWorld(), FollowActorTag, mFollowActor))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] '%s' 태그를 가진 액터를 찾지 못했습니다."), 
				*mOwner->GetName(), *FollowActorTag.ToString());
		}
	}

	if (AEnemyPawn* enemy = Cast<AEnemyPawn>(mOwner))
	{
		mMoveSpeed = enemy->RuntimeSettings.Speed;
	}
}

void UFollowMover::MoveStep(float _deltaTime) const
{
	check(mOwner);
	check(mFollowActor);
	
	const FVector direction = (mFollowActor->GetActorLocation() - mOwner->GetActorLocation()).GetSafeNormal();
	const FVector newLocation = mOwner->GetActorLocation() + direction * mMoveSpeed * MoveSpeedScale * _deltaTime;

	mOwner->SetActorLocation(newLocation);
}
