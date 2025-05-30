// SurvivorGameModeBase.cpp
#include "SurvivorGameModeBase.h"

#include "ObjectPoolManagerComponent.h"
#include "WaveManagerComponent.h"
#include "EnemyFireManagerComponent.h"
#include "PlanetPawn.h"
#include "PlanetHUD.h"

ASurvivorGameModeBase::ASurvivorGameModeBase()
{
	ObjectPoolManager = CreateDefaultSubobject<UObjectPoolManagerComponent>(TEXT("Object Pool Manager"));

	WaveManager = CreateDefaultSubobject<UWaveManagerComponent>(TEXT("Wave Manager"));

	FireManager = CreateDefaultSubobject<UEnemyFireManagerComponent>(TEXT("Enemy Fire Manager"));
}

void ASurvivorGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		mWinGameTimerHandle,
		this,
		&ASurvivorGameModeBase::winGame,
		PlanetConst::PLAYTIME,
		false
	);
}

void ASurvivorGameModeBase::winGame()
{
	for (TEnumAsByte participant : Participants)
	{
		if (participant != EAutoReceiveInput::Disabled)
		{
			if (APlanetPawn* _player = Cast<APlanetPawn>(GetTargetPlayerPawn(participant, this)))
			{
				check(_player->PlanetHUD);
				_player->PlanetHUD->ShowWinGame();
			}
		}
	}
}
