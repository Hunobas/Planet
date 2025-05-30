// PassiveItemSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "PassiveItemType.h"
#include "Components/ActorComponent.h"
#include "PlanetConst.h"
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
	int32 MaxSlots = PlanetConst::ITEM_MAX_SLOT;
	UPROPERTY(VisibleAnywhere, Category = "Item Slot")
	int32 RemainSlots = PlanetConst::ITEM_MAX_SLOT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (ArraySizeEnum = "6", 
			ToolTip = "게임 시작 시 각 슬롯에 장착할 아이템 타입을 설정합니다. 빈 슬롯은 None으로 설정하세요."))
	TArray<EPassiveItemType> InitialItemTypes;

	TArray<APassiveItem*> EquippedItems;
	
private:
	APlanetPawn* cOwner;
};
