// PassiveItemSlotComponent.h
#include "PassiveItemSlotComponent.h"

#include "PlanetPawn.h"

UPassiveItemSlotComponent::UPassiveItemSlotComponent(): cOwner(nullptr)
{
	mEquippedItems.SetNum(MaxSlots);
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
	
	for (int32 i = 0; i < mEquippedItems.Num(); i++)
	{
		if (!mEquippedItems[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (UObject* newItem = NewObject<UObject>())
			{
				mEquippedItems[i] = newItem;
				RemainSlots--;
				return true;
			}
		}
	}
	
	return false;
}

UObject* UPassiveItemSlotComponent::GetItemByTypeOrNull(const EPassiveItemType& _itemType)
{
	for (UObject* item : mEquippedItems)
	{
		// TODO: 아이템 클래스 구현
		// if (item->ItemType == _itemType)
		// {
		// 	return item;
		// }
	}
	return nullptr;
}
