// WeaponSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSlotComponent.generated.h"

class AWeaponPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLANET_API UWeaponSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponSlotComponent();

	UPROPERTY(EditAnywhere, Category = "Weapon Slot", meta=(ClampMin=1, ClampMax=12))
	int32 MaxSlots = 6;
	UPROPERTY(EditAnywhere, Category = "Weapon Slot")
	TSubclassOf<AWeaponPawn> DefaultWeaponClass;

protected:
	virtual void BeginPlay() override;

private:
	bool EquipWeapon(const TSubclassOf<AWeaponPawn>& _weaponClass);

	APawn* cOwner;
	TArray<AWeaponPawn*> mEquippedWeapons;

};
