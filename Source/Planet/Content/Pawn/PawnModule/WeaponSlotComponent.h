// WeaponSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Planet.h"
#include "WeaponSlotComponent.generated.h"

class AWeaponPawn;
class APlanetPawn;
enum class EWeaponType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UWeaponSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponSlotComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool EquipWeapon(const EWeaponType& _weaponType);
	AWeaponPawn* GetWeaponByTypeOrNull(const EWeaponType& _weaponType);

	UPROPERTY(EditAnywhere, Category = "Weapon Slot")
	TMap<EWeaponType, TSubclassOf<AWeaponPawn>> WeaponTypeToClassMap;
	UPROPERTY(EditAnywhere, Category = "Weapon Slot", meta=(ClampMin=1, ClampMax=12))
	int32 MaxSlots = WEAPON_MAX_SLOT;
	UPROPERTY(EditAnywhere, Category = "Weapon Slot")
	EWeaponType DefaultWeaponType;

	int32 RemainSlots = WEAPON_MAX_SLOT;

private:
	APlanetPawn* cOwner;
	TArray<AWeaponPawn*> mEquippedWeapons;

};
