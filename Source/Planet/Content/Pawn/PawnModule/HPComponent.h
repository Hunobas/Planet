#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

class APlanetPawn;
class AEnemyPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHPComponent();
	UHPComponent* Initialize();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser);
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxHP;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	float CurrentHP;

private:
	void handleDeath() const;

	APlanetPawn* mPlanet;
	AEnemyPawn* mEnemy;
};
