// // WeaponRewardApplicator.h
#include "WeaponRewardApplicator.h"

#include "PlanetPawn.h"
#include "WeaponPawn.h"
#include "WeaponSlotComponent.h"
#include "WeaponRewardData.h"

void UWeaponRewardApplicator::Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData, APlanetPawn* _targetPlayer)
{
	if (UWeaponRewardData* WeaponData = Cast<UWeaponRewardData>(_rewardData.GetObject()))
	{
		check(_targetPlayer->WeaponSlot);
		
		if (AWeaponPawn* Weapon = _targetPlayer->WeaponSlot->GetWeaponByTypeOrNull(WeaponData->WeaponType))
		{
			const int32 NewLevel = WeaponData->CurrentLevel + 1;
			WeaponData->SetLevel(NewLevel);
			Weapon->LevelUp(NewLevel);
		}
		else
		{
			_targetPlayer->WeaponSlot->EquipWeapon(WeaponData->WeaponType);
		}
	}
}