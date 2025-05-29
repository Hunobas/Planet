// PassiveItemSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "PassiveItemType.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "PassiveItemSlotComponent.generated.h"

enum class EPassiveItemType : uint8;
class APassiveItem;
class APlanetPawn;

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
	APassiveItem* GetItemByTypeOrNull(const EPassiveItemType& _itemType);
	void ReloadAllItems() const;

	UPROPERTY(EditAnywhere, Category = "Item Slot")
	TMap<EPassiveItemType, TSubclassOf<APassiveItem>> ItemTypeToClassMap;
	UPROPERTY(EditAnywhere, Category = "Item Slot", meta=(ClampMin=1, ClampMax=12))
	int32 MaxSlots = ITEM_MAX_SLOT;
	UPROPERTY(VisibleAnywhere, Category = "Item Slot")
	int32 RemainSlots = ITEM_MAX_SLOT;

	UPROPERTY(EditAnywhere, Category = "Debug")
	EPassiveItemType TestItemType = EPassiveItemType::None;

	TArray<APassiveItem*> EquippedItems;
	
private:
	APlanetPawn* cOwner;
};
