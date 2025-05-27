// FiringComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "FiringComponent.generated.h"

class AEnemyPawn;
class AWeaponPawn;
class APlanetPawn;
class USceneComponent;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UFiringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFiringComponent();

	virtual void BeginPlay() override;

	void StartFireSequence(const TFunction<void(const UFiringComponent*)>& _callback = TFunction<void(const UFiringComponent*)>());
	void Fire();
	void HandleJustAim();

	UPROPERTY(EditAnywhere, Category = "Firing")
	TEnumAsByte<EAutoReceiveInput::Type> TargetPlayer;
	UPROPERTY(EditAnywhere, Category = "Firing")
	FName MuzzlePointTag;
	
	UPROPERTY(EditAnywhere, Category = "Firing")
	float FireDelay		= ENEMY_FIRE_DELAY;
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float JustAimDelay	= ENEMY_FIRE_DELAY - JUSTAIM_WINDOW_DURATION;
	UPROPERTY(EditAnywhere, Category = "Firing")
	float JustAimDamage	= 40.0f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* JustAimDefaultNS;
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* MuzzleTemplate;

	USceneComponent* MuzzlePoint;
	APawn* TargetPawn;

private:
	void setOwnerParams();
	void setTargetParams();
	void startJustAimWindow();
	void endJustAimWindow();
	void invokeDequeueCallback();
	float getOwnerDamage() const;
	
	APawn* mOwner;
	AEnemyPawn* mEnemyOwner;
	AWeaponPawn* cWeaponOwner;
	APlanetPawn* mTargetPlanet;
	TFunction<void(const UFiringComponent*)> mDequeueCallback;

	FTimerHandle mJustAimWindowTimerHandle;
	FTimerHandle mFireTimerHandle;
	bool bIsInJustAimWindow = false;
	
};