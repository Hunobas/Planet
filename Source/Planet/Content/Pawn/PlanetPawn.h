// PlanetPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSetting.h"
#include "PlanetPawn.generated.h"

struct FInputActionValue;
class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlayCamera;
class UOrbitMover;
class UHPComponent;
class UJustAimManagerComponent;
class APlanetController;
class UPlayerDataAsset;

UCLASS()
class PLANET_API APlanetPawn : public APawn
{
	GENERATED_BODY()

public:
	APlanetPawn();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UStaticMeshComponent* PlanetMesh;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UPlayCamera* PlayCamera;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UOrbitMover* OrbitMover;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UJustAimManagerComponent* JustAimManager;

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VisibleDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category="Combat")
	UPlayerDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FPlayerSetting RuntimeSettings;
	
private:
	void composeComponent();
	void updatePlanetRotation(const FVector& _worldMousePosition) const;
	void resetToDefaultSettings();

	APlanetController* cPlanetController;
	UHPComponent* mHP;
};
