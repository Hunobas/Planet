// PlayCamera.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayCamera.generated.h"

class APlanetPawn;
class USpringArmComponent;
class UCameraComponent;
class UCameraShakeBase; 
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UPlayCamera : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float _deltaTime, enum ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

	void StartAim();
	void StopAim();
	
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
	FRotator SpringArmRotation		= {0.0f, -90.0f, 0};
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
	float DefaultArmLength			= 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
	float AimedArmLength			= 2900.0f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float ArmLengthInterpSpeed		= 5.0f;

	UPROPERTY(EditAnywhere, Category = "Just Aim")
	TSubclassOf<UCameraShakeBase> JustAimCameraShakeClass;
	

private:
	void onJustAimSuccess(const FVector& _targetLocation);
	void updateArmLength(float _deltaTime);
	
	APlanetPawn* mPlayerPawn;
	USpringArmComponent* mSpringArm;
	UCameraComponent* mCamera;

	float mCurrentArmLength;
	bool bIsAiming = false;

	FRotator mStartControlRotation;
	FRotator mTargetControlRotation;
	float mJustAimingElapsedTime = 0.0f;
};