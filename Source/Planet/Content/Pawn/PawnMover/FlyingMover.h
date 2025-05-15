// FlyingMover.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlyingMover.generated.h"

class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UFlyingMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlyingMover();

protected:
	virtual void BeginPlay() override;

public:	
	void MoveStep(float _deltaTime);

	UPROPERTY(EditAnywhere, Category = "Flying Mover")
	FName OwnerMeshTag;
	
	UPROPERTY(EditAnywhere, Category = "Flying Mover", meta = (ClampMin=0))
	float Amplitude			= 50.0f;
	UPROPERTY(EditAnywhere, Category = "Flying Mover", meta = (ClampMin=0))
	float MoveSpeed			= 1.0f;
	UPROPERTY(EditAnywhere, Category = "Flying Mover")
	bool NeedRandomStart	= true;

private:
	AActor* mOwner;
	UStaticMeshComponent* mOwnerMesh;

	float mElapsedTime = 0.0f;
};
