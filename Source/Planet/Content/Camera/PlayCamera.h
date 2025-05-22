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

	void OnJustAimSuccess(const FVector& _targetLocation);
	
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
	FRotator SpringArmRotation		= {90.0f, -90.0f, 0};
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
	float DefaultArmLength			= 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Just Aim")
	TSubclassOf<UCameraShakeBase> JustAimCameraShakeClass;

private:
	APlanetPawn* mPlayerPawn;
	USpringArmComponent* mSpringArm;
	UCameraComponent* mCamera;

	FRotator mStartControlRotation;
	FRotator mTargetControlRotation;
	float mJustAimingElapsedTime = 0.0f;
	bool bIsJustAiming = false;
};