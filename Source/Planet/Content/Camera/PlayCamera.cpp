// PlayCamera.cpp
#include "PlayCamera.h"

#include "JustAimManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlanetPawn.h"

UPlayCamera::UPlayCamera(): mPlayerPawn(nullptr), mSpringArm(nullptr), mCamera(nullptr), mCurrentArmLength(0)
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

	check(mPlayerPawn->JustAimManager);
	mPlayerPawn->JustAimManager->OnSuccessJustAim.AddLambda([this](const USceneComponent* _firePoint)
	{
		onJustAimSuccess(_firePoint->GetComponentLocation());
	});
}

void UPlayCamera::TickComponent(float _deltaTime, enum ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	// updateArmLength(_deltaTime);
}

void UPlayCamera::StartAim()
{
	bIsAiming = true;
}

void UPlayCamera::StopAim()
{
	bIsAiming = false;
}

void UPlayCamera::onJustAimSuccess(const FVector& _targetLocation)
{
	check(mPlayerPawn);
	check(mCamera);
	
	const FVector cameraLocation = mCamera->GetComponentLocation();
    
	mStartControlRotation = mPlayerPawn->GetControlRotation();
	mTargetControlRotation = (_targetLocation - cameraLocation).Rotation();
	mJustAimingElapsedTime = 0.0f;

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

void UPlayCamera::updateArmLength(float _deltaTime)
{
	check(mSpringArm);

	float targetLength = bIsAiming ? AimedArmLength : DefaultArmLength;
	mCurrentArmLength = FMath::FInterpTo(mCurrentArmLength, targetLength, _deltaTime, ArmLengthInterpSpeed);
	mSpringArm->TargetArmLength = mCurrentArmLength;
}
