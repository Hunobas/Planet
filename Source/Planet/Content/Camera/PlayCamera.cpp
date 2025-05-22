// PlayCamera.cpp
#include "PlayCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

void UPlayCamera::OnJustAimSuccess(const FVector& _targetLocation)
{
	check(mPlayerPawn);
	check(mCamera);
	
	const FVector cameraLocation = mCamera->GetComponentLocation();
    
	mStartControlRotation = mPlayerPawn->GetControlRotation();
	mTargetControlRotation = (_targetLocation - cameraLocation).Rotation();
	mJustAimingElapsedTime = 0.0f;
	// bIsJustAiming = true;

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