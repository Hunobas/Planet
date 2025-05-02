// PlanetPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlanetPawn.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlayCamera;
class UOrbitMover;
class APlanetController;

UCLASS()
class PLANET_API APlanetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanetPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VisibleDistance = 1500.0f;

private:
	APlanetController* mPlanetController;
	
	void composeComponent();
	void updatePlanetRotation() const;
};
