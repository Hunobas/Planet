// PlayCamera.cpp
#include "PlayCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

#include "../Planet.h"
#include "PlanetPawn.h"

UPlayCamera::UPlayCamera(): mPlayerPawn(nullptr), mSpringArm(nullptr), mCamera(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayCamera::BeginPlay()
{
	Super::BeginPlay();

	mPlayerPawn = Cast<APlanetPawn>(GetOwner());
	check(mPlayerPawn);
	
	mSpringArm = mPlayerPawn->SpringArm;
	mCamera = mPlayerPawn->Camera;

	mSpringArm->bUsePawnControlRotation = false;

	mSpringArm->SetRelativeRotation(SpringArmRotation);
	mSpringArm->TargetArmLength    = DefaultArmLength;
}

void UPlayCamera::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	updateJustAimRotation(_deltaTime);
}

void UPlayCamera::OnJustAimSuccess(const FVector& _targetLocation)
{
	check(mPlayerPawn);
	check(mCamera);
	
	const FVector cameraLocation = mCamera->GetComponentLocation();
    
	mStartControlRotation = mPlayerPawn->GetControlRotation();
	mTargetControlRotation = (_targetLocation - cameraLocation).Rotation();
	mJustAimingElapsedTime = 0.0f;
	bIsJustAiming = true;

	if (JustAimCameraShakeClass)
	{
		if (APlayerController* PC = Cast<APlayerController>(mPlayerPawn->GetController()))
		{
			PC->PlayerCameraManager->StartCameraShake(JustAimCameraShakeClass);
		}
		else
		{
			check(false);
		}
	}
}

void UPlayCamera::updateJustAimRotation(float _deltaTime)
{
	if (!bIsJustAiming)
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
		bIsJustAiming = false;
	}
}