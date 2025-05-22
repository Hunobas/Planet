// WeaponSlotComponent.h
#include "WeaponSlotComponent.h"

#include "WeaponPawn.h"
#include "PlanetPawn.h"

UWeaponSlotComponent::UWeaponSlotComponent(): cOwner(nullptr)
{
	mEquippedWeapons.SetNum(MaxSlots);
}

void UWeaponSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	
	cOwner = Cast<APlanetPawn>(GetOwner());
	EquipWeapon(DefaultWeaponClass);
}

bool UWeaponSlotComponent::EquipWeapon(const TSubclassOf<AWeaponPawn>& _weaponClass)
{
	if (!_weaponClass)
		return false;

	check(cOwner);
	
	for (int32 i = 0; i < mEquippedWeapons.Num(); i++)
	{
		if (!mEquippedWeapons[i])
		{
			FActorSpawnParameters params;
			params.Owner = cOwner;
            
			if (AWeaponPawn* newWeapon = GetWorld()->SpawnActor<AWeaponPawn>(
				_weaponClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				params))
			{
				mEquippedWeapons[i] = newWeapon;
				newWeapon->AttachToComponent(
					cOwner->PlanetMesh,
					FAttachmentTransformRules::SnapToTargetNotIncludingScale
				);
				return true;
			}
		}
	}
	
	return false;
}
