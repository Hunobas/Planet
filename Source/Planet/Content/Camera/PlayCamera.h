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

	// Pawn::BeginPlay 에서 호출
	UPlayCamera* Initialize(APawn* InOwner, USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	void UpdateSocketOffY();
	void UpdateArmLength(float DeltaTime);

	void Look(const FInputActionValue& Value);
	void StartAim();
	void StopAim();

	UPROPERTY(EditAnywhere, Category="Transform")
	float DefaultRotationalSpeed =100.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	FVector SpringArmLocation = {0, 40.f, 70.f};
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float SocketOffYMax = 100.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float SocketOffYMin = 40.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float DefaultArmLength    = 500.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float AimedArmLength      = 250.f;
	UPROPERTY(EditAnywhere, Category="Spring Arm")
	float ArmLengthInterpSpeed=   5.f;

private:
	APawn* cOwner;
	USpringArmComponent* mSpringArm;
	UCameraComponent*    mCamera;

	float mCurrentArmLength;
	float mRotationalSpeed;
	bool  bIsAiming;
};