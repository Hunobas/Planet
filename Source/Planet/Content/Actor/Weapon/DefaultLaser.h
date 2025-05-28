// ADefaultLaser.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultLaser.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class AWeaponPawn;
class APlanetPawn;

UENUM(BlueprintType)
enum class ELaserType : uint8
{
	Infinite UMETA(DisplayName = "Infinite"),
	Finite UMETA(DisplayName = "Finite")
};

UCLASS()
class PLANET_API ADefaultLaser : public AActor
{
	GENERATED_BODY()

public:
	ADefaultLaser();

	void Initialize(AWeaponPawn* _owner);
	void Fire();

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UNiagaraComponent* LaserEffect;

	UPROPERTY(EditAnywhere, Category = "Combat")
	ELaserType LaserType = ELaserType::Finite;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpan = 4.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxPierce = 1;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageInterval = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* LaserHitTemplate;

private:
	void performLaserTrace();
	void updateLaserEndPosition(const TArray<AActor*>& _targetEnemies) const;
	void destroyLaser();

	AWeaponPawn* cOwner;
	APlanetPawn* cOwnPlanet;
	
	FTimerHandle mLifeSpanTimerHandle;
	FTimerHandle mDamageTimerHandle;
};
