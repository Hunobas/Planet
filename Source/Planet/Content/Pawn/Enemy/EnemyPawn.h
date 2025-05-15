// EnemyPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemySetting.h"
#include "EnemyScaleSetting.h"
#include "EnemyPawn.generated.h"

struct FEnemyScaleSetting;
class UCapsuleComponent;
class UEnemyDataAsset;
class UFollowMover;
class UFlyingMover;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	None	= 0,
	Melee	UMETA(DisplayName="근접"),
	Ranged	UMETA(DisplayName="원거리"),
	Boss	UMETA(DisplayName="보스")
};

UCLASS()
class PLANET_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float _deltaTime) override;
	void ResetToDefaultSettings(const FEnemyScaleSetting& _scaleSettings, APawn* _targetPlayer = nullptr);

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* BodyCollisionCapsule;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* HitDetectionCapsule;

	UPROPERTY(EditAnywhere, Category = "Combat")
	EEnemyType EnemyType = EEnemyType::None;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	UEnemyDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FEnemySetting RuntimeSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FEnemyScaleSetting ActiveBuffs;

private:
	APawn* cTargetPawn;
	UFlyingMover* mFlyingMover;
	UFollowMover* mFollowMover;
};
