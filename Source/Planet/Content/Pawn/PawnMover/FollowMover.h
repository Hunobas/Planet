// FollowMover.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "FollowMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UFollowMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFollowMover();

protected:
	virtual void BeginPlay() override;

public:
	void MoveStep(float _deltaTime) const;

	UPROPERTY(EditAnywhere, Category = "Follow Mover")
	TEnumAsByte<EAutoReceiveInput::Type> FollowPlayer	= SINGLEPLAYER_INDEX;
	UPROPERTY(EditAnywhere, Category = "Follow Mover")
	FName FollowActorTag								= NAME_None;
	UPROPERTY(EditAnywhere, Category = "Follow Mover", meta=(ClampMin=0))
	float MoveSpeedScale								= 1.0f;

private:
	AActor* mOwner;
	AActor* mFollowActor;

	float mMoveSpeed = 100.0f;
};
