// PassiveItemSlotComponent.h
#include "PassiveItemSlotComponent.h"

#include "PlanetPawn.h"
#include "PlanetHUD.h"

UPassiveItemSlotComponent::UPassiveItemSlotComponent(): cOwner(nullptr)
{
	EquippedItems.SetNum(MaxSlots);
}

void UPassiveItemSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	cOwner = Cast<APlanetPawn>(GetOwner());
}

bool UPassiveItemSlotComponent::EquipItem(const EPassiveItemType& _itemType)
{
	check(ItemTypeToClassMap[_itemType]);
	check(cOwner);
	check(RemainSlots > 0);
	
	for (int32 i = 0; i < EquippedItems.Num(); i++)
	{
		if (!EquippedItems[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (UObject* newItem = NewObject<UObject>())
			{
				EquippedItems[i] = newItem;
				RemainSlots--;
				return true;
			}
		}
	}
	
	return false;
}

UObject* UPassiveItemSlotComponent::GetItemByTypeOrNull(const EPassiveItemType& _itemType)
{
	for (UObject* item : EquippedItems)
	{
		// TODO: 아이템 클래스 구현
		// if (item && item->ItemType == _itemType)
		// {
		// 	return item;
		// }
	}
	return nullptr;
}

void UPassiveItemSlotComponent::ReloadAllItems() const
{
	check(cOwner);
	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->OnItemSlotChanged(EquippedItems);
}
