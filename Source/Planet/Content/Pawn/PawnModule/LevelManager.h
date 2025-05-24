// LevelManager.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelManager.generated.h"

class APlanetPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API ULevelManager : public UActorComponent
{
	GENERATED_BODY()

public:    
	ULevelManager();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Level")
	void GainXP(float XP);

private:
	void calculateNextLevelXP();
	void processLevelUp();

	UPROPERTY(VisibleAnywhere, Category="Level")
	int32 mCurrentLevel = 1;
	UPROPERTY(VisibleAnywhere, Category="Level")
	float mCurrentXP = 0.0f;
	UPROPERTY(VisibleAnywhere, Category="Level")
	float mXPToNextLevel = 0.0f;

	APlanetPawn* cOwner;
};
