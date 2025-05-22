// PlanetController.cpp
#include "PlanetController.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../Planet.h"

void APlanetController::BeginPlay()
{
	Super::BeginPlay();

	mEISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(mEISubsystem);
	mEISubsystem->AddMappingContext(PlayerIMC, DEFAULT_ORDER);

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void APlanetController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	SetViewTarget(_pawn);

	bindInputMappings(_pawn);
}

void APlanetController::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		MouseHoverLocation = FMath::LinePlaneIntersection(
			WorldLocation, 
			WorldLocation + WorldDirection * 10000.0f, 
			FVector::ZeroVector, 
			FVector::UpVector
		);

		ProjectWorldLocationToScreen(
			MouseHoverLocation, 
			ScreenMousePosition, 
			true
		);
	}
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
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlanetController::onLookTriggered);
		EIC->BindAction(JustAimAction, ETriggerEvent::Triggered, this, &APlanetController::setLastLookInput);
		EIC->BindAction(JustAimAction, ETriggerEvent::None, this, &APlanetController::resetLastLookInput);
	}
}

void APlanetController::onLookTriggered(const FInputActionValue& Value)
{
	OnLookValue.Broadcast(Value.Get<FVector2D>());
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
