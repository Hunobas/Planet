// PlayCamera.cpp
#include "PlayCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

// Sets default values for this component's properties
UPlayCamera::UPlayCamera()
  : mOwner(nullptr)
  , mSpringArm(nullptr)
  , mCamera(nullptr)
  , mCurrentArmLength(0.f)
  , mRotationalSpeed(DefaultRotationalSpeed)
  , bIsAiming(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

UPlayCamera* UPlayCamera::Initialize(APawn* InOwner, USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	mOwner = InOwner;
	mSpringArm = InSpringArm;
	mCamera    = InCamera;

	mCurrentArmLength = DefaultArmLength;
	mRotationalSpeed  = DefaultRotationalSpeed;
	bIsAiming        = false;

	mSpringArm->SetWorldLocation(SpringArmLocation);
	mSpringArm->SocketOffset = SocketOff;
	mSpringArm->TargetArmLength = mCurrentArmLength;
	mSpringArm->bUsePawnControlRotation = true;

	mCamera->bUsePawnControlRotation = false;

	return this;
}

void UPlayCamera::UpdateArmLength(float DeltaTime)
{
	if (!mSpringArm) return;

	float Target = bIsAiming ? AimedArmLength : DefaultArmLength;
	mCurrentArmLength = FMath::FInterpTo(mCurrentArmLength, Target, DeltaTime, ArmLengthInterpSpeed);
	mSpringArm->TargetArmLength = mCurrentArmLength;
}

void UPlayCamera::Look(const FInputActionValue& Value)	// const로 삼으면 안됨.
{
	check(mOwner);
	
	FVector2D Axis = Value.Get<FVector2D>();
	float Delta = GetWorld()->GetDeltaSeconds();
	mOwner->AddControllerYawInput  (Axis.X * Delta * mRotationalSpeed);
	mOwner->AddControllerPitchInput(Axis.Y * Delta * mRotationalSpeed);
}

void UPlayCamera::StartAim()
{
	bIsAiming       = true;
	mRotationalSpeed = DefaultRotationalSpeed * 0.5f;
}

void UPlayCamera::StopAim()
{
	bIsAiming        = false;
	mRotationalSpeed  = DefaultRotationalSpeed;
}