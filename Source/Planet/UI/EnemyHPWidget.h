// EnemyHPWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

UCLASS()
class PLANET_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateHealthProgress(const float& _percent);

protected:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPProgressBar;
	
};
