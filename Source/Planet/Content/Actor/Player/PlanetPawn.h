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
class UShieldComponent;
class UJustAimManagerComponent;
class URewardManager;
class ULevelComponent;
class UWeaponSlotComponent;
class UPassiveItemSlotComponent;
class UDayOfWeekComponent;
class UPlayerDataAsset;
class APlanetController;
class APlanetHUD;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class PLANET_API APlanetPawn : public APawn
{
	GENERATED_BODY()

public:
	APlanetPawn();

protected:
	virtual void BeginPlay() override;

public:
	void StartAim();
	void StopAim();

	void HandleDied();
	
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
	UHPComponent* HP;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UShieldComponent* Shield;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	ULevelComponent* Level;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	URewardManager* RewardManager;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UWeaponSlotComponent* WeaponSlot;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UPassiveItemSlotComponent* ItemSlot;
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	UDayOfWeekComponent* DayOfWeek;

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VisibleDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category="Combat")
	UPlayerDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FPlayerSetting RuntimeSettings;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* AimEffectTemplate;
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* PlanetDieTemplate;

	APlanetHUD* PlanetHUD;
	bool bPlayerAiming;
	
private:
	void composeComponent();
	void updatePlanetRotation(const FVector& _worldMousePosition) const;
	void resetToDefaultSettings();
	
	UPROPERTY()
	UNiagaraComponent* mCurrentAimEffect;
	
	APlanetController* cPlanetController;
};
