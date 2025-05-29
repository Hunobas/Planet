// WeaponSlotComponent.h
#include "WeaponSlotComponent.h"

#include "WeaponPawn.h"
#include "WeaponType.h"
#include "PlanetPawn.h"
#include "PlanetHUD.h"

UWeaponSlotComponent::UWeaponSlotComponent(): DefaultWeaponType(EWeaponType::None), cOwner(nullptr)
{
	EquippedWeapons.SetNum(MaxSlots);
}

void UWeaponSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	
	cOwner = Cast<APlanetPawn>(GetOwner());
	EquipWeapon(DefaultWeaponType);

	for (EWeaponType& testWeaponType : InitialWeaponTypes)
	{
		if (testWeaponType != EWeaponType::None)
		{
			EquipWeapon(testWeaponType);
		}
	}
}

bool UWeaponSlotComponent::EquipWeapon(const EWeaponType& _weaponType)
{
	check(WeaponTypeToClassMap[_weaponType]);
	check(cOwner);
	check(cOwner->PlanetMesh);
	check(RemainSlots > 0);
	
	for (int32 i = 0; i < EquippedWeapons.Num(); i++)
	{
		if (!EquippedWeapons[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (AWeaponPawn* newWeapon = GetWorld()->SpawnActor<AWeaponPawn>(
				WeaponTypeToClassMap[_weaponType],
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				params))
			{
				EquippedWeapons[i] = newWeapon;
				newWeapon->AttachToComponent(
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

AWeaponPawn* UWeaponSlotComponent::GetWeaponByTypeOrNull(const EWeaponType& _weaponType)
{
	for (AWeaponPawn* weapon : EquippedWeapons)
	{
		if (weapon && weapon->WeaponType == _weaponType)
		{
			return weapon;
		}
	}
	return nullptr;
}

void UWeaponSlotComponent::ReloadAllWeapons()
{
	for (AWeaponPawn* weapon : EquippedWeapons)
	{
		if (weapon)
		{
			weapon->StopAttack();
			weapon->StartAttack();
		}
	}

	check(cOwner);
	check(cOwner->PlanetHUD);
	cOwner->PlanetHUD->OnWeaponSlotChanged(EquippedWeapons);
}
