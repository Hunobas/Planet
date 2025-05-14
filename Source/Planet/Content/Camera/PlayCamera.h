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
	
	virtual void BeginPlay() override;

protected:
	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

public:
	void Look(const FInputActionValue& _value);
	void StartAim();
	void StopAim();

	void OnJustAimSuccess(const FVector& _targetLocation);

	UPROPERTY(EditAnywhere, Category = "Look")
	float DefaultRotationalSpeed	= 30.0f;

	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	FVector SpringArmLocation		= {0, 40.f, 70.f};
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float SocketOffYMax				= 120.f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float SocketOffYMin				= 215.f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float DefaultArmLength			= 500.f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float AimedArmLength			= 250.f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float ArmLengthInterpSpeed		= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float DefaultRotationLagSpeed	= 30.0f;

	UPROPERTY(EditAnywhere, Category = "Just Aim")
	float JustAimingDuration		= 0.08f;
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	float JustAimedArmLength		= 350.f;
	UPROPERTY(EditAnywhere, Category = "Just Aim")
	TSubclassOf<UCameraShakeBase> JustAimCameraShakeClass;

private:
	void updateSocketOffY();
	void updateArmLength(float _deltaTime);
	void updateJustAimRotation(float _deltaTime);
	
	APlanetPawn* mPlayerPawn;
	USpringArmComponent* mSpringArm;
	UCameraComponent* mCamera;

	float mCurrentArmLength;
	float mRotationalSpeed;
	bool bIsAiming;

	FRotator mStartControlRotation;
	FRotator mTargetControlRotation;
	float mJustAimingElapsedTime = 0.0f;
	bool bIsJustAiming = false;
};