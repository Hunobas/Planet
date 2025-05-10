// PlayCamera.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UPlayCamera : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayCamera();

	// Pawn::생성자 에서 호출
	UPlayCamera* Initialize(APawn* _owner, USpringArmComponent* _springArm, UCameraComponent* _camera);

	void UpdateSocketOffY();
	void UpdateArmLength(float _deltaTime);

	void Look(const FInputActionValue& _value);
	void StartAim();
	void StopAim();

	UPROPERTY(EditAnywhere, Category="Transform")
	float DefaultRotationalSpeed	= 100.f;

	UPROPERTY(EditAnywhere, Category="Spring Arm")
	FVector SpringArmLocation		= {0, 40.f, 70.f};
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float SocketOffYMax				= 120.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float SocketOffYMin				= 215.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float DefaultArmLength			= 500.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float AimedArmLength			= 250.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float ArmLengthInterpSpeed		= 5.f;

private:
	APawn* mOwner;
	USpringArmComponent* mSpringArm;
	UCameraComponent* mCamera;

	float mCurrentArmLength;
	float mRotationalSpeed;
	bool bIsAiming;
};