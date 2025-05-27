// WeaponPawn.cpp
#include "WeaponPawn.h"

#include "Components/SceneComponent.h"

AWeaponPawn::AWeaponPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void AWeaponPawn::LevelUp(const int32& _newLevel)
{
	CurrentLevel = _newLevel;
}
