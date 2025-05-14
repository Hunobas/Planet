// PlayCamera.cpp
#include "PlayCamera.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

UPlayCamera::UPlayCamera(): mPlayerPawn(nullptr), mSpringArm(nullptr), mCamera(nullptr), mCurrentArmLength(0),
                            mRotationalSpeed(0),
                            bIsAiming(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayCamera::BeginPlay()
{
	Super::BeginPlay();

	mPlayerPawn = Cast<APlanetPawn>(GetOwner());
	mSpringArm = mPlayerPawn->SpringArm;
	mCamera = mPlayerPawn->Camera;

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
	check(mPlayerPawn);
	
	FVector2D axis = _value.Get<FVector2D>();
	float deltaTime = GetWorld()->GetDeltaSeconds();
	mPlayerPawn->AddControllerYawInput  (axis.X * deltaTime * mRotationalSpeed);
	mPlayerPawn->AddControllerPitchInput(axis.Y * deltaTime * mRotationalSpeed);
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
	check(mPlayerPawn);
	check(mCamera);
	
	const FVector cameraLocation = mCamera->GetComponentLocation();
    
	mStartControlRotation = mPlayerPawn->GetControlRotation();
	mTargetControlRotation = (_targetLocation - cameraLocation).Rotation();
	mJustAimingElapsedTime = 0.0f;
	bIsJustAimRotating = true;
}

void UPlayCamera::updateSocketOffY()
{
	check(mPlayerPawn);
	check(mCamera);
	check(mSpringArm);
	
	const float pawnYaw	 = mPlayerPawn->GetActorRotation().Yaw + 45.0f;
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

	check(mPlayerPawn);

	mJustAimingElapsedTime += _deltaTime;
	const float t = FMath::Clamp(mJustAimingElapsedTime / JustAimingDuration, 0.0f, 1.0f);
    
	const FRotator NewRotation = LerpAngle(
		mStartControlRotation,
		mTargetControlRotation,
		EaseOutQuint(t)
	);
    
	mPlayerPawn->GetController()->SetControlRotation(NewRotation);

	if (t >= 1.0f)
	{
		mPlayerPawn->GetController()->SetControlRotation(mTargetControlRotation);
		bIsJustAimRotating = false;
	}
}