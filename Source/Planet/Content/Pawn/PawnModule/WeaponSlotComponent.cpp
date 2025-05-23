// WeaponSlotComponent.h
#include "WeaponSlotComponent.h"

#include "WeaponPawn.h"
#include "PlanetPawn.h"
#include "WeaponType.h"

UWeaponSlotComponent::UWeaponSlotComponent(): DefaultWeaponType(EWeaponType::None), cOwner(nullptr)
{
	mEquippedWeapons.SetNum(MaxSlots);
}

void UWeaponSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	
	cOwner = Cast<APlanetPawn>(GetOwner());
	EquipWeapon(DefaultWeaponType);
}

bool UWeaponSlotComponent::EquipWeapon(const EWeaponType& _weaponType)
{
	check(WeaponTypeToClassMap[_weaponType]);
	check(cOwner);
	
	for (int32 i = 0; i < mEquippedWeapons.Num(); i++)
	{
		if (!mEquippedWeapons[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (AWeaponPawn* newWeapon = GetWorld()->SpawnActor<AWeaponPawn>(
				WeaponTypeToClassMap[_weaponType],
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				params))
			{
				mEquippedWeapons[i] = newWeapon;
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
	for (AWeaponPawn* weapon : mEquippedWeapons)
	{
		if (weapon->WeaponType == _weaponType)
		{
			return weapon;
		}
	}
	return nullptr;
}
