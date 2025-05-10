// FiringComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "FiringComponent.generated.h"

class UNiagaraSystem;
class AEnemyPawn;
class AWeaponPawn;
class USceneComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UFiringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFiringComponent();

	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

	void StartFireSequence();
	void Fire() const;

	UPROPERTY(EditAnywhere, Category = "Firing")
	TEnumAsByte<EAutoReceiveInput::Type> TargetPlayer;
	UPROPERTY(EditAnywhere, Category = "Firing")
	FName MuzzlePointTag;
	
	UPROPERTY(EditAnywhere, Category = "Firing")
	float FireDelay		= 2.0f;
	UPROPERTY(EditAnywhere, Category = "Firing")
	float JustAimDelay	= 1.5f;
	UPROPERTY(EditAnywhere, Category = "Firing")
	float JustAimDamage	= 40.0f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* JustAimTemplate;
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* MuzzleTemplate;

private:
	void setOwnerParams();
	void setTargetParams();
	void startJustAimWindow();
	void endJustAimWindow();
	float getOwnerDamage() const;
	
	APawn* cOwner;
	AEnemyPawn* cEnemyOwner;
	AWeaponPawn* cWeaponOwner;
	APawn* cTargetPawn;
	USceneComponent* mMuzzlePoint;

	FTimerHandle mJustAimWindowTimerHandle;
	FTimerHandle mFireTimerHandle;
	bool bIsInJustAimWindow = false;
};
