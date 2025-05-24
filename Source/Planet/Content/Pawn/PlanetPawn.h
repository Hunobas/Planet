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
class URewardManager;
class ULevelManager;
class UWeaponSlotComponent;
class UPassiveItemSlotComponent;
class UPlayerDataAsset;
class APlanetController;
class APlanetHUD;

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
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	ULevelManager* LevelManager;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	URewardManager* RewardManager;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UWeaponSlotComponent* WeaponSlot;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UPassiveItemSlotComponent* ItemSlot;

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VisibleDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category="Combat")
	UPlayerDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FPlayerSetting RuntimeSettings;

	APlanetHUD* PlanetHUD;
	
private:
	void composeComponent();
	void updatePlanetRotation(const FVector& _worldMousePosition) const;
	void resetToDefaultSettings();

	APlanetController* cPlanetController;
	UHPComponent* mHP;
};
