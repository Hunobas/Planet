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
	virtual void Tick(float _deltaTime) override;
	
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
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	USceneComponent* LookPoint;
	
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
	float VisibleDistance		= 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Planet")
	float DefaultRotationSpeed	= 30.0f;
	UPROPERTY(EditAnywhere, Category = "Planet")
	float MouseYawCoefficient	= 0.1f;
	UPROPERTY(EditAnywhere, Category = "Planet")
	float MaxSpeedMultiplier	= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Planet")
	float MinSpeedMultiplier	= 0.1f;
	UPROPERTY(EditAnywhere, Category = "Planet")
	float FrictionCoefficient	= 0.2f;

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
	float RotationSpeedMultiplier;
	float WeeklyPointAngle;
	
private:
	void composeComponent();
	void updatePlanetRotationSpeed(const FVector& _worldMousePosition);
	void resetToDefaultSettings();
	
	UPROPERTY()
	UNiagaraComponent* mCurrentAimEffect;
	
	APlanetController* cPlanetController;

	float mPreviousYaw;
	bool bIsMouseMoving = false;
	float mCurrentRotationSpeed;

	// Obsolete
	void updatePlanetRotation(const FVector& _worldMousePosition) const;
};
