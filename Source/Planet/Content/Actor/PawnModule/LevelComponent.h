// LevelComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelComponent.generated.h"

class APlanetPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API ULevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:    
	ULevelComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Level")
	void GainXP(float XP);

	UPROPERTY(VisibleAnywhere, Category="Level")
	int32 CurrentLevel = 1;
	UPROPERTY(VisibleAnywhere, Category="Level")
	float CurrentXP = 0.0f;
	UPROPERTY(VisibleAnywhere, Category="Level")
	float XPToNextLevel = 0.0f;

private:
	void calculateNextLevelXP();
	void processLevelUp();

	APlanetPawn* cOwner;
};
