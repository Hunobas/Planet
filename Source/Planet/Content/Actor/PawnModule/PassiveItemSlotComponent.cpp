// PassiveItemSlotComponent.h
#include "PassiveItemSlotComponent.h"

#include "PassiveItem.h"
#include "PassiveItemType.h"
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

	for (EPassiveItemType& TestItemType : InitialItemTypes)
	{
		if (TestItemType != EPassiveItemType::None)
		{
			EquipItem(TestItemType);
		}
	}
}

bool UPassiveItemSlotComponent::EquipItem(const EPassiveItemType& _itemType)
{
	check(ItemTypeToClassMap[_itemType]);
	check(cOwner);
	check(cOwner->PlanetMesh);
	check(RemainSlots > 0);
	
	for (int32 i = 0; i < EquippedItems.Num(); i++)
	{
		if (!EquippedItems[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (APassiveItem* newItem = GetWorld()->SpawnActor<APassiveItem>(
				ItemTypeToClassMap[_itemType],
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				params))
			{
				EquippedItems[i] = newItem;
				newItem->AttachToComponent(
					cOwner->PlanetMesh,
					FAttachmentTransformRules::SnapToTargetNotIncludingScale
				);
				RemainSlots--;
				return true;
			}
		}
	}
	
	return false;
}

APassiveItem* UPassiveItemSlotComponent::GetItemByTypeOrNull(const EPassiveItemType& _itemType)
{
	for (APassiveItem* item : EquippedItems)
	{
		if (item && item->ItemType == _itemType)
		{
			return item;
		}
	}
	return nullptr;
}

void UPassiveItemSlotComponent::ReloadAllItems() const
{
	check(cOwner);
	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->OnItemSlotChanged(EquippedItems);
}