// PlanetController.cpp
#include "PlanetController.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "PlayCamera.h"

void APlanetController::BeginPlay()
{
	Super::BeginPlay();

	mEISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(mEISubsystem);
	mEISubsystem->AddMappingContext(PlayerIMC, 0);
}

void APlanetController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	SetViewTarget(_pawn);
	bindInputMappings(_pawn);
}

FVector2D APlanetController::GetEMAInput()
{
	if (mInputHistory.IsEmpty())
		return FVector2D::ZeroVector;
    
	FVector2D ema = mInputHistory[0];
    
	for (int32 i = 1; i < mInputHistory.Num(); i++)
	{
		ema.X = RecentInputWeight * mInputHistory[i].X + (1 - RecentInputWeight) * ema.X;
		ema.Y = RecentInputWeight * mInputHistory[i].Y + (1 - RecentInputWeight) * ema.Y;
	}
    
	return ema;
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
		
		EIC->BindAction(JustAimAction, ETriggerEvent::Triggered, this, &APlanetController::setLastLookInput);
		EIC->BindAction(JustAimAction, ETriggerEvent::None, this, &APlanetController::resetLastLookInput);
	}
}

void APlanetController::setLastLookInput(const FInputActionValue& _value)
{
	mResetDelayElapsed = 0.0f;
	
	FVector2D NewInput = _value.Get<FVector2D>();

	mInputHistory.Add(NewInput);
	if (mInputHistory.Num() > InputBufferSize)
		mInputHistory.RemoveAt(0);
}

void APlanetController::resetLastLookInput(const FInputActionValue& _value)
{
	mResetDelayElapsed += 0.01f;
	if (mResetDelayElapsed <= InputResetDelay)
		return;

	mResetDelayElapsed = 0.0f;

	mInputHistory.Add(FVector2D::ZeroVector);
	if (mInputHistory.Num() > InputBufferSize)
		mInputHistory.RemoveAt(0);
}
