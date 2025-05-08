// PlayCamera.cpp
#include "PlayCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

UPlayCamera::UPlayCamera()
  : cOwner(nullptr)
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
	cOwner = InOwner;
	mSpringArm = InSpringArm;
	mCamera    = InCamera;

	mCurrentArmLength = DefaultArmLength;
	mRotationalSpeed  = DefaultRotationalSpeed;
	bIsAiming        = false;

	mSpringArm->SetWorldLocation(SpringArmLocation);
	mSpringArm->SocketOffset = FVector({0, SocketOffYMax, 0});
	mSpringArm->TargetArmLength = mCurrentArmLength;
	mSpringArm->bUsePawnControlRotation = true;

	mCamera->bUsePawnControlRotation = false;

	return this;
}

void UPlayCamera::UpdateSocketOffY()
{
	check(cOwner && mSpringArm);
	
	float PawnYaw = cOwner->GetActorRotation().Yaw;
	float CamYaw  = cOwner->GetController()->GetControlRotation().Yaw;

	float DeltaYaw = FMath::FindDeltaAngleDegrees(PawnYaw, CamYaw);
	float AbsYaw   = FMath::Abs(DeltaYaw);

	float t = FMath::Clamp(AbsYaw / 90.f, 0.f, 1.f);
	float TargetY = FMath::Lerp(SocketOffYMax, SocketOffYMin, t);

	mSpringArm->SocketOffset.Y = TargetY;
}

void UPlayCamera::UpdateArmLength(float DeltaTime)
{
	check(mSpringArm);

	float Target = bIsAiming ? AimedArmLength : DefaultArmLength;
	mCurrentArmLength = FMath::FInterpTo(mCurrentArmLength, Target, DeltaTime, ArmLengthInterpSpeed);
	mSpringArm->TargetArmLength = mCurrentArmLength;
}

void UPlayCamera::Look(const FInputActionValue& Value)
{
	check(cOwner);
	
	FVector2D Axis = Value.Get<FVector2D>();
	float Delta = GetWorld()->GetDeltaSeconds();
	cOwner->AddControllerYawInput  (Axis.X * Delta * mRotationalSpeed);
	cOwner->AddControllerPitchInput(Axis.Y * Delta * mRotationalSpeed);
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