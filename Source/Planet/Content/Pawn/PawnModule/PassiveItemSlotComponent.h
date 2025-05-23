// PassiveItemSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "PassiveItemSlotComponent.generated.h"

class APlanetPawn;
class UPassiveItemRewardApplicator;
enum class EPassiveItemType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UPassiveItemSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPassiveItemSlotComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool EquipItem(const EPassiveItemType& _itemType);

	UPROPERTY(EditAnywhere, Category = "Item Slot")
	TMap<EPassiveItemType, TSubclassOf<UPassiveItemRewardApplicator>> ItemTypeToClassMap;
	UPROPERTY(EditAnywhere, Category = "Item Slot", meta=(ClampMin=1, ClampMax=12))
	int32 MaxSlots = ITEM_MAX_SLOT;

	int32 RemainSlots = ITEM_MAX_SLOT;

private:
	APlanetPawn* cOwner;
	TArray<UPassiveItemRewardApplicator*> mEquippedItems;

};
