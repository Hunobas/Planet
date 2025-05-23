// EnemyPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemySetting.h"
#include "EnemyScaleSetting.h"
#include "IUpdateStrategy.h"
#include "EnemyPawn.generated.h"

struct FEnemyScaleSetting;
class UCapsuleComponent;
class UStaticMeshComponent;
class AXpGem;
class UEnemyDataAsset;
class UFollowMover;
class UFlyingMover;
class UHPComponent;
class UWidgetComponent;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	None	= 0,
	Melee	UMETA(DisplayName="근접"),
	Ranged	UMETA(DisplayName="원거리"),
	Boss	UMETA(DisplayName="보스")
};

UENUM(BlueprintType)
enum class EUpdateType : uint8
{
	Continuous	UMETA(DisplayName="시간 연속적"),
	InputDriven	UMETA(DisplayName="플레이어 입력")
};

UCLASS()
class PLANET_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();
	void Initialize();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float _deltaTime) override;

	void MoveStep(float _deltaTime);
	
	void ResetToDefaultSettings(const FEnemyScaleSetting& _scaleSettings, APawn* _targetPlayer = nullptr);
	void HandleDied();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);

	UFUNCTION()
	void HandleDamageTaken(AActor* _damagedActor, float _damage, const class UDamageType* _damageType, class AController* _instigatedBy, AActor* _damageCauser);

	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* BodyCollisionCapsule;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UStaticMeshComponent* EnemyMesh;
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	UCapsuleComponent* HitDetectionCapsule;

	UPROPERTY(EditAnywhere, Category = "Combat")
	EEnemyType EnemyType	= EEnemyType::None;
	UPROPERTY(EditAnywhere, Category = "Combat")
	EUpdateType UpdateType	= EUpdateType::Continuous;
	UPROPERTY(EditAnywhere, Category = "Combat", meta=(AllowAbstract="false"))
	TSubclassOf<AXpGem> XpGemClass;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	UEnemyDataAsset* BaseSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FEnemySetting RuntimeSettings;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	FEnemyScaleSetting ActiveBuffs;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* EnemyDieTemplate;

private:
	void setUpdateStrategy();
	void spawnXpGem() const;
	
	APawn* cTargetPawn;
	UFlyingMover* mFlyingMover;
	UFollowMover* mFollowMover;
	UHPComponent* mHP;
	UWidgetComponent* mHPWidget;
	
	TUniquePtr<IUpdateStrategy> mUpdateStrategy;
};
