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

UCLASS()
class PLANET_API APlanetController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* _pawn) override;

public:
	virtual void Tick(float _deltaTime) override;
	
	FVector2D GetEMAInput();

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

	FVector MouseHoverLocation		= FVector::ZeroVector;
	FVector2D ScreenMousePosition	= FVector2D::ZeroVector;
	
private:
	void bindInputMappings(const APawn* _pawn);
	void onLookTriggered(const FInputActionValue& Value);
	void setLastLookInput(const FInputActionValue& _value);
	void resetLastLookInput(const FInputActionValue& _value);
	
	UEnhancedInputLocalPlayerSubsystem* mEISubsystem;

	TArray<FVector2D> mInputHistory;
	float mResetDelayElapsed = 0.0f;
};
