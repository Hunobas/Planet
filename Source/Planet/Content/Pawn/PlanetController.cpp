// PlanetController.cpp
#include "PlanetController.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "PlanetPawn.h"
#include "../Camera/PlayCamera.h"

void APlanetController::BeginPlay()
{
	Super::BeginPlay();

	mPlanetPawn = Cast<APlanetPawn>(GetPawn());

	mEISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(mEISubsystem != nullptr);
	mEISubsystem->AddMappingContext(PlayerIMC, 0);
}

void APlanetController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bindInputMappings(InPawn);
}

void APlanetController::bindInputMappings(APawn* InPawn)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UPlayCamera* PlayCamera = InPawn->FindComponentByClass<UPlayCamera>())
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, PlayCamera, &UPlayCamera::Look);
			EIC->BindAction(AimAction,  ETriggerEvent::Started,   PlayCamera, &UPlayCamera::StartAim);
			EIC->BindAction(AimAction,  ETriggerEvent::Completed, PlayCamera, &UPlayCamera::StopAim);
		}
	}
}