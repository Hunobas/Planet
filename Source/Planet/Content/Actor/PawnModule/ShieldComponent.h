// ShieldComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShieldComponent.generated.h"

class UHPComponent;
class APlanetPawn;
class AEnemyPawn;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShieldComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;
    
	void AddShield(float _shieldAmount);
    
	UFUNCTION()
	void OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser);

	DECLARE_MULTICAST_DELEGATE(FOnShieldOff);
	FOnShieldOff OnShieldOff;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DecayDelay		= 5.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DecayMinAmount	= 0.7f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DecayMaxAmount	= 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float CurrentShield;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* ShieldEffectTemplate;

private:
	void startDecayTimer();
	void updateDecay();
	float calculateDecayAmount() const;
	void updateUI() const;
    
	UHPComponent* cHPComponent;
	APlanetPawn* mPlanet;
	AEnemyPawn* mEnemy;
	
	UPROPERTY()
	UNiagaraComponent* mCurrentShieldEffect;
    
	FTimerHandle mDecayDelayHandle;
	bool bIsDecaying;
};