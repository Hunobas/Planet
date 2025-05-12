// SurvivorGameModeBase.cpp
#include "SurvivorGameModeBase.h"

#include "ObjectPoolManagerComponent.h"
#include "WaveManagerComponent.h"
#include "EnemyFireManagerComponent.h"

ASurvivorGameModeBase::ASurvivorGameModeBase()
{
	ObjectPoolManager = CreateDefaultSubobject<UObjectPoolManagerComponent>(TEXT("Object Pool Manager"));

	WaveManager = CreateDefaultSubobject<UWaveManagerComponent>(TEXT("Wave Manager"));

	FireManager = CreateDefaultSubobject<UEnemyFireManagerComponent>(TEXT("Enemy Fire Manager"));
}
