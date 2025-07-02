// WeaponSlotComponent.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponType.h"
#include "Components/ActorComponent.h"
#include "PlanetConst.h"
#include "WeaponSlotComponent.generated.h"

enum class EWeaponType : uint8;
class AWeaponPawn;
class APlanetPawn;

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
	void ReloadAllWeapons();

	UPROPERTY(EditAnywhere, Category = "Weapon Slot")
	TMap<EWeaponType, TSubclassOf<AWeaponPawn>> WeaponTypeToClassMap;
	UPROPERTY(EditAnywhere, Category = "Weapon Slot", meta=(ClampMin=1, ClampMax=12))
	int32 MaxSlots					= PlanetConst::WEAPON_MAX_SLOT;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Slot")
	int32 RemainSlots				= PlanetConst::WEAPON_MAX_SLOT;
	UPROPERTY(EditAnywhere, Category = "Weapon Slot")
	EWeaponType DefaultWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (ArraySizeEnum = "5", 
			ToolTip = "게임 시작 시 각 슬롯에 장착할 무기 타입을 설정합니다. 빈 슬롯은 None으로 설정하세요."))
	TArray<EWeaponType> InitialWeaponTypes;

	TArray<AWeaponPawn*> EquippedWeapons;
	
private:
	APlanetPawn* cOwner;

};