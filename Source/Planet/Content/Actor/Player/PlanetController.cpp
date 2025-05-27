// PlanetController.cpp
#include "PlanetController.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlanetPawn.h"

#include "../Planet.h"

void APlanetController::BeginPlay()
{
	Super::BeginPlay();

	mEISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(mEISubsystem);
	mEISubsystem->AddMappingContext(PlayerIMC, DEFAULT_ORDER);

	OnLookValue.AddLambda([this](const FVector2D& _inputValue)
	{
		FVector worldLocation, worldDirection;
		if (DeprojectMousePositionToWorld(worldLocation, worldDirection))
		{
			WorldMouseLocation = FMath::LinePlaneIntersection(
				worldLocation, 
				worldLocation + worldDirection * 10000.0f, 
				FVector::ZeroVector, 
				FVector::UpVector
			);
		}
	});
}

void APlanetController::EndPlay(const EEndPlayReason::Type _endPlayReason)
{
	Super::EndPlay(_endPlayReason);

	OnLookValue.Clear();
}

void APlanetController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	SetViewTarget(_pawn);
	bindInputMappings(_pawn);
	SetInGameInputMode();
}

void APlanetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D currentMousePosition;
	if (!GetMousePosition(currentMousePosition.X, currentMousePosition.Y))
		return;
        
	const FVector2D mouseDelta = currentMousePosition - mPreviousMousePosition;
	if (mouseDelta.SizeSquared() > MouseMoveThreshold)
	{
		OnLookValue.Broadcast(mouseDelta);
		mPreviousMousePosition = currentMousePosition;
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

void APlanetController::SetInGameInputMode()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);
}

void APlanetController::SetUIInputMode(UUserWidget* _widget)
{
	bShowMouseCursor = true;
	FInputModeUIOnly inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(inputMode);
}

void APlanetController::bindInputMappings(APawn* _pawn)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (APlanetPawn* planetPawn = Cast<APlanetPawn>(_pawn))
		{
			EIC->BindAction(AimAction,  ETriggerEvent::Started,   planetPawn, &APlanetPawn::StartAim);
			EIC->BindAction(AimAction,  ETriggerEvent::Completed, planetPawn, &APlanetPawn::StopAim);
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
