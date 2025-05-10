// PlayCamera.cpp
#include "PlayCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

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

UPlayCamera* UPlayCamera::Initialize(APawn* _owner, USpringArmComponent* _springArm, UCameraComponent* _camera)
{
	mOwner = _owner;
	mSpringArm = _springArm;
	mCamera = _camera;

	mCurrentArmLength = DefaultArmLength;
	mRotationalSpeed = DefaultRotationalSpeed;
	bIsAiming = false;

	mSpringArm->SetWorldLocation(SpringArmLocation);
	mSpringArm->SocketOffset = FVector({0, SocketOffYMax, 0});
	mSpringArm->TargetArmLength = mCurrentArmLength;
	mSpringArm->bUsePawnControlRotation = true;

	mCamera->bUsePawnControlRotation = false;

	return this;
}

void UPlayCamera::UpdateSocketOffY()
{
	check(mOwner);
	check(mCamera);
	check(mSpringArm);
	
	const float pawnYaw	 = mOwner->GetActorRotation().Yaw + 45.0f;
	const float camYaw	 = mCamera->GetComponentRotation().Yaw;
	const float deltaYaw = FMath::FindDeltaAngleDegrees(pawnYaw, camYaw);
	const float absYaw	 = FMath::Abs(deltaYaw);

	const float t		 = FMath::Clamp(absYaw / 180.f, 0.f, 1.f);
	const float targetY	 = FMath::Lerp(SocketOffYMax, SocketOffYMin, t);

	mSpringArm->SocketOffset.Y = targetY;
}

void UPlayCamera::UpdateArmLength(float _deltaTime)
{
	check(mSpringArm);

	float targetLength = bIsAiming ? AimedArmLength : DefaultArmLength;
	mCurrentArmLength = FMath::FInterpTo(mCurrentArmLength, targetLength, _deltaTime, ArmLengthInterpSpeed);
	mSpringArm->TargetArmLength = mCurrentArmLength;
}

void UPlayCamera::Look(const FInputActionValue& _value)
{
	check(mOwner);
	
	FVector2D axis = _value.Get<FVector2D>();
	float deltaTime = GetWorld()->GetDeltaSeconds();
	mOwner->AddControllerYawInput  (axis.X * deltaTime * mRotationalSpeed);
	mOwner->AddControllerPitchInput(axis.Y * deltaTime * mRotationalSpeed);
}

void UPlayCamera::StartAim()
{
	bIsAiming = true;
	mRotationalSpeed = DefaultRotationalSpeed * 0.5f;
}

void UPlayCamera::StopAim()
{
	bIsAiming = false;
	mRotationalSpeed = DefaultRotationalSpeed;
}