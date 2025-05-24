// PassiveItemRewardApplicator.cpp
#include "PassiveItemRewardApplicator.h"

#include "PlanetPawn.h"
#include "PassiveItemSlotComponent.h"
#include "PassiveItemRewardData.h"

void UPassiveItemRewardApplicator::Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer)
{
	if (UPassiveItemRewardData* ItemData = Cast<UPassiveItemRewardData>(_rewardData.GetObject()))
	{
		check(_targetPlayer->ItemSlot);
		
		if (UObject* Item = _targetPlayer->ItemSlot->GetItemByTypeOrNull(ItemData->PassiveItemType))
		{
			const int32 NewLevel = ItemData->GetCurrentLevel() + 1;
			ItemData->SetLevel(NewLevel);
			// TODO: 아이템 클래스 구현
			// Item->LevelUp(NewLevel);
		}
		else
		{
			_targetPlayer->ItemSlot->EquipItem(ItemData->PassiveItemType);
		}
	}
}
