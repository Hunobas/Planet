// PassiveItem.cpp
#include "PassiveItem.h"

APassiveItem::APassiveItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void APassiveItem::LevelUp(const int32& _newLevel)
{
	CurrentLevel = _newLevel;
}