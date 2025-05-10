// PlanetController.cpp
#include "PlanetController.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "PlanetPawn.h"
#include "PlayCamera.h"

void APlanetController::BeginPlay()
{
	Super::BeginPlay();

	mPlanetPawn = Cast<APlanetPawn>(GetPawn());

	mEISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(mEISubsystem);
	mEISubsystem->AddMappingContext(PlayerIMC, 0);
}

void APlanetController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	bindInputMappings(_pawn);
}

void APlanetController::bindInputMappings(const APawn* _pawn)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UPlayCamera* playCamera = _pawn->FindComponentByClass<UPlayCamera>())
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, playCamera, &UPlayCamera::Look);
			EIC->BindAction(AimAction,  ETriggerEvent::Started,   playCamera, &UPlayCamera::StartAim);
			EIC->BindAction(AimAction,  ETriggerEvent::Completed, playCamera, &UPlayCamera::StopAim);
		}
	}
}