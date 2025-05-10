// OrbitMover.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
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
	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Orbit Mover")
	FName SunTag	= DEFAULT_SUN_TAG;
	UPROPERTY(EditAnywhere, Category = "Orbit Mover")
	int32 NumOrbits	= 2;

private:
	void moveStep(float _deltaTime);
	
	AActor* cOwner;
	AActor* cTargetSun;
	
	FVector mTargetLocation = FVector::ZeroVector;
	float mOrbitRadius;
	float mOrbitPeriod;
	float mCurrentAngle;
};
