// EnemyPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemySetting.h"
#include "EnemyPawn.generated.h"

class UCapsuleComponent;
class UEnemyDataAsset;

UCLASS()
class PLANET_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

	void ResetToDefaultSettings();

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* BodyCollisionCapsule;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* HitDetectionCapsule;

	UPROPERTY(EditAnywhere, Category="Combat")
	UEnemyDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FEnemySetting RuntimeSettings;

};
