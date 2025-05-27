// PassiveItem.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PassiveItemType.h"
#include "PassiveItem.generated.h"

enum class EPassiveItemType : uint8;

UCLASS()
class PLANET_API APassiveItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APassiveItem();
	virtual void LevelUp(const int32& _newLevel);

	virtual void ApplyEffect() { }
    
	UPROPERTY(EditAnywhere, Category = "Blueprint Components")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Config")
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, Category = "Config")
	EPassiveItemType ItemType;
    
	UPROPERTY(EditAnywhere, Category = "DEBUG")
	int32 CurrentLevel  = 0;
};
