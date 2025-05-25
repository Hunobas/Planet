// PlanetController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlanetController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UEnhancedInputLocalPlayerSubsystem;
class APlanetPawn;
class UUserWidget;

UCLASS()
class PLANET_API APlanetController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type _endPlayReason) override;

	virtual void OnPossess(APawn* _pawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	FVector2D GetEMAInput();
	void SetInGameInputMode();
	void SetUIInputMode(UUserWidget* _widget);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLookValue, const FVector2D&);
	FOnLookValue OnLookValue;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerIMC;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JustAimAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 InputBufferSize = 5;
	UPROPERTY(EditAnywhere, Category = "Input")
	float RecentInputWeight = 0.6f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float InputResetDelay = 0.2f;

	FVector WorldMouseLocation = FVector::ZeroVector;
	
private:
	void bindInputMappings();
	void onLookTriggered(const FInputActionValue& Value);
	void setLastLookInput(const FInputActionValue& _value);
	void resetLastLookInput(const FInputActionValue& _value);
	
	UEnhancedInputLocalPlayerSubsystem* mEISubsystem;
	
	FVector2D mPreviousMousePosition = FVector2D::ZeroVector;
	TArray<FVector2D> mInputHistory;
	float mResetDelayElapsed = 0.0f;
};
