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
	bool HasJustAimed(const USceneComponent* _firePoint);

	void SucceedJustAim(USceneComponent* _firePoint);
	void FailJustAim();

	UPROPERTY(EditAnywhere, Category = "Just Aim")
	float JustAimWindowRadius = 3000.0f;
	UPROPERTY(VisibleAnywhere, Category = "Just Aim")
	float JustAimWindowDuration = JUSTAIM_WINDOW_DURATION;
	UPROPERTY(EditAnywhere, Category = "Just Aim")
	float JustAimWindowAngle = 60.0f;
	UPROPERTY(EditAnywhere, Category = "Just Aim")
	float JustAimInputSpeedThreshold = 0.1f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* JustAimSuccessTemplate;

	bool IsInJustAimWindow = false;
	
private:
	APlanetPawn* cPlayerPawn;
	APlanetController* cPlayerController;
	
	float mDistanceToSpawnCelestial;
	FVector mCachedDestinationNorm;
};
