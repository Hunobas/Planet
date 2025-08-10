// PlayerPowerUpRewardApplicator.cpp
#include "PlayerPowerUpRewardApplicator.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlayerPowerUpRewardData.h"
#include "HPComponent.h"
#include "WeaponSlotComponent.h"

void UPlayerPowerUpRewardApplicator::Apply_Implementation(const TScriptInterface<IRewardData>& _rewardData,
	APlanetPawn* _targetPlayer)
{
	if (UPlayerPowerUpRewardData* PowerUpData = Cast<UPlayerPowerUpRewardData>(_rewardData.GetObject()))
	{
		switch(PowerUpData->PowerUpType)
		{
		case EPlayerPowerUpType::HP:
			_targetPlayer->RuntimeSettings.HP += HP_BOOST;

			check(_targetPlayer->HP);
			_targetPlayer->HP->AddMaxHP(HP_BOOST);
			break;
		case EPlayerPowerUpType::Damage:
			_targetPlayer->RuntimeSettings.Damage += DAMAGE_BOOST;
			break;
		case EPlayerPowerUpType::Critical:
			_targetPlayer->RuntimeSettings.Critical += CRITICAL_BOOST;
			break;
		case EPlayerPowerUpType::CriticalDamage:
			_targetPlayer->RuntimeSettings.CriticalDamage += CRITICAL_DAMAGE_BOOST;
			break;
		case EPlayerPowerUpType::Haste:
			_targetPlayer->RuntimeSettings.Haste += HASTE_BOOST;
			
			check(_targetPlayer->WeaponSlot);
			_targetPlayer->WeaponSlot->ReloadAllWeapons();
			break;
		case EPlayerPowerUpType::XpGain:
			_targetPlayer->RuntimeSettings.XpGain += XP_GAIN_BOOST;
			break;
		case EPlayerPowerUpType::XpSpeed:
			_targetPlayer->RuntimeSettings.XpSpeed += XP_SPEED_BOOST;
			break;
		default:
			checkNoEntry();
		}
	}
}
