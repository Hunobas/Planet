// XPGem.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XpGem.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class APlanetPawn;
class UObjectPoolManagerComponent;
class UFollowMover;
class UNiagaraSystem;

UCLASS()
class PLANET_API AXpGem : public AActor
{
	GENERATED_BODY()
	
public:	
	AXpGem();
	AXpGem* Initialize(APawn* _targetPlayer, const float& _XP, UObjectPoolManagerComponent* _pool);
	virtual void Tick(float _deltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UStaticMeshComponent* ShellMesh;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UStaticMeshComponent* GemMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float XP = 2.f;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* GainTemplate;

private:
	APlanetPawn* cTargetPlayer;
	UObjectPoolManagerComponent* mPool;
	UFollowMover* mFollowMover;
};
