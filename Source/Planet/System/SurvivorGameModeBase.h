// SurvivorGameModeBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlanetConst.h"
#include "SurvivorGameModeBase.generated.h"

class UObjectPoolManagerComponent;
class UWaveManagerComponent;
class UEnemyFireManagerComponent;

UCLASS()
class PLANET_API ASurvivorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASurvivorGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category="Object Pool")
	UObjectPoolManagerComponent* ObjectPoolManager;

	UPROPERTY(EditAnywhere, Category = "Wave Manager")
	UWaveManagerComponent* WaveManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Fire Manager")
	UEnemyFireManagerComponent* FireManager;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Fire Manager")
	float EnemyFireDelay = PlanetConst::ENEMY_FIRE_DELAY;

	UPROPERTY(EditAnywhere, Category = "Player")
	TArray<TEnumAsByte<EAutoReceiveInput::Type>> Participants;

private:
	void winGame();
	
	FTimerHandle mWinGameTimerHandle;
};
