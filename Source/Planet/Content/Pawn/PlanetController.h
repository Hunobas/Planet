// PlanetController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlanetController.generated.h"

class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class APlanetPawn;

UCLASS()
class PLANET_API APlanetController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* _pawn) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerIMC;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;

private:
	void bindInputMappings(const APawn* _pawn);
	
	APlanetPawn* mPlanetPawn;
	UEnhancedInputLocalPlayerSubsystem* mEISubsystem;
};
