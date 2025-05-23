// PassiveItemSlotComponent.h
#include "PassiveItemSlotComponent.h"

#include "PassiveItemRewardApplicator.h"
#include "PlanetPawn.h"
#include "PassiveItemType.h"

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
	
	for (int32 i = 0; i < mEquippedItems.Num(); i++)
	{
		if (!mEquippedItems[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (UPassiveItemRewardApplicator* newItem = CreateDefaultSubobject<UPassiveItemRewardApplicator>(TEXT("Item")))
			{
				mEquippedItems[i] = newItem;
				RemainSlots--;
				return true;
			}
		}
	}
	
	return false;
}
