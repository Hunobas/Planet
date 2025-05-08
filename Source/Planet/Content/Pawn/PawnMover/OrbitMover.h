// OrbitMover.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UOrbitMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrbitMover();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category="OrbitMover")
	AActor* TargetSun;
	UPROPERTY(EditAnywhere, Category="OrbitMover")
	int32 NumOrbits = 2;

private:
	void moveStep(float DeltaTime);
	
	AActor* cOwner;
	FVector mTargetLocation = FVector::ZeroVector;
	float mOrbitRadius;
	float mOrbitPeriod;
	float mCurrentAngle;
	
};
