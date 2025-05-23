// // WeaponRewardApplicator.h
#include "WeaponRewardApplicator.h"

#include "PlanetPawn.h"
#include "WeaponPawn.h"
#include "WeaponSlotComponent.h"
#include "WeaponRewardData.h"

void UWeaponRewardApplicator::Apply_Implementation(const TScriptInterface<IRewardData>& RewardData)
{
	if (UWeaponRewardData* WeaponData = Cast<UWeaponRewardData>(RewardData.GetObject()))
	{
		if (APlanetPawn* PlayerPawn = Cast<APlanetPawn>(GetOuter()))
		{
			if (AWeaponPawn* Weapon = PlayerPawn->WeaponSlot->GetWeaponByTypeOrNull(WeaponData->WeaponType))
			{
				const int32 NewLevel = WeaponData->GetCurrentLevel() + 1;
				WeaponData->SetLevel(NewLevel);
				Weapon->LevelUp(NewLevel);
			}
			else
			{
				PlayerPawn->WeaponSlot->EquipWeapon(WeaponData->WeaponType);
			}
		}
	}
}