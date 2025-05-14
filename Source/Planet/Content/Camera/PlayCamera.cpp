// PlayCamera.cpp
#include "PlayCamera.h"

#include "../Planet.h"
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
	mSpringArm->bEnableCameraRotationLag = true;
	mSpringArm->CameraRotationLagSpeed = DefaultRotationLagSpeed;

	mCamera->bUsePawnControlRotation = false;

	return this;
}

void UPlayCamera::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	updateArmLength(_deltaTime);
	updateSocketOffY();
	updateJustAimRotation(_deltaTime);
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

void UPlayCamera::OnJustAimSuccess(const FVector& _targetLocation)
{
	const FVector cameraLocation = mCamera->GetComponentLocation();
    
	mStartControlRotation = mOwner->GetControlRotation();
	mTargetControlRotation = (_targetLocation - cameraLocation).Rotation();
	mJustAimingElapsedTime = 0.0f;
	bIsJustAimRotating = true;
}

void UPlayCamera::updateSocketOffY()
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

void UPlayCamera::updateArmLength(float _deltaTime)
{
	check(mSpringArm);

	float targetLength = bIsAiming ? AimedArmLength : DefaultArmLength;
	mCurrentArmLength = FMath::FInterpTo(mCurrentArmLength, targetLength, _deltaTime, ArmLengthInterpSpeed);
	mSpringArm->TargetArmLength = mCurrentArmLength;
}

void UPlayCamera::updateJustAimRotation(float _deltaTime)
{
	if (!bIsJustAimRotating)
		return;

	mJustAimingElapsedTime += _deltaTime;
	const float t = FMath::Clamp(mJustAimingElapsedTime / JustAimingDuration, 0.0f, 1.0f);
    
	const FRotator NewRotation = LerpAngle(
		mStartControlRotation,
		mTargetControlRotation,
		EaseOutQuint(t)
	);
    
	mOwner->GetController()->SetControlRotation(NewRotation);

	if (t >= 1.0f)
	{
		mOwner->GetController()->SetControlRotation(mTargetControlRotation);
		bIsJustAimRotating = false;
	}
}