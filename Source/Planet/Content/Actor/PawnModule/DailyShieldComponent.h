// DailyShieldComponent.h
#pragma once

#include "CoreMinimal.h"
#include "DayOfWeekComponent.h"
#include "Components/ActorComponent.h"
#include "DailyShieldComponent.generated.h"

enum class EPlanetDayOfWeek : uint8;
class UHPComponent;
class AEnemyPawn;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UDailyShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDailyShieldComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddShield(float _shieldAmount);
    
	UFUNCTION()
	void OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser);

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float CurrentShield;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	EPlanetDayOfWeek WeaknessDay = EPlanetDayOfWeek::Monday;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* ShieldEffectTemplate;

private:
	bool isWeaknessDay() const;
	void destroyShieldEffect();
	void updateUI() const;
    
	UHPComponent* cHPComponent;
	AEnemyPawn* mEnemy;
	
	UPROPERTY()
	UNiagaraComponent* mCurrentShieldEffect;
};
