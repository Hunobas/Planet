#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "JustAimManagerComponent.generated.h"

class USceneComponent;
class APlanetPawn;
class APlanetController;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UJustAimManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJustAimManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	bool HasJustAimed(const USceneComponent* _firePoint) const;

	void SucceedJustAim(USceneComponent* _firePoint) const;
	void FailJustAim();

	UPROPERTY(EditAnywhere, Category = "Just Aim")
	float JustAimWindowAngle = 15.0f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* JustAimSuccessTemplate;

private:
	APlanetPawn* cPlayerPawn;
	
	float mDistanceToSpawnCelestial;
	FVector mCachedDestinationNorm;
};
