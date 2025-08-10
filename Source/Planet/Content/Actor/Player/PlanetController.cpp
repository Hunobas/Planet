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

	bShowMouseCursor = true;
}

void APlanetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D currentMousePosition;
	if (!GetMousePosition(currentMousePosition.X, currentMousePosition.Y))
		return;
	
	const FVector2D mouseDelta = currentMousePosition - mPreviousMousePosition;
	if (!bUIInputMode && mouseDelta.SizeSquared() > MouseMoveThreshold)
	{
		OnLookValue.Broadcast(mouseDelta);
		mPreviousMousePosition = currentMousePosition;
	}
}

void APlanetController::SetInGameInputMode()
{
	bUIInputMode = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);
}

void APlanetController::SetUIInputMode(UUserWidget* _widget)
{
	bUIInputMode = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
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
	}
}