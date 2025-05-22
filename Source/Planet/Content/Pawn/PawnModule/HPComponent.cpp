// HPComponent.cpp
#include "HPComponent.h"

#include "EnemyPawn.h"
#include "PlanetPawn.h"
#include "SurvivorGameModeBase.h"
#include "WaveManagerComponent.h"

UHPComponent::UHPComponent(): MaxHP(100.0f), CurrentHP(100.0f), mPlanet(nullptr), mEnemy(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

UHPComponent* UHPComponent::Initialize()
{
	if (AEnemyPawn* enemy = Cast<AEnemyPawn>(GetOwner()))
	{
		mEnemy = enemy;
		MaxHP = mEnemy->RuntimeSettings.HP;
		mEnemy->OnTakeAnyDamage.AddDynamic(this, &UHPComponent::OnTakeAnyDamage);
	}
	else if (APlanetPawn* player = Cast<APlanetPawn>(GetOwner()))
	{
		mPlanet = player;
		MaxHP = mPlanet->RuntimeSettings.HP;
		mPlanet->OnTakeAnyDamage.AddDynamic(this, &UHPComponent::OnTakeAnyDamage);
	}
	
	CurrentHP = MaxHP;

	return this;
}

void UHPComponent::OnTakeAnyDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	if (_damage <= 0.0f)
		return;

	CurrentHP = FMath::Clamp(CurrentHP - _damage, 0.0f, MaxHP);

	if (CurrentHP <= 0.0f)
	{
		handleDeath();
	}
}

void UHPComponent::handleDeath() const
{
	UWorld* world = GetWorld();
	if (!world) return;

	if (ASurvivorGameModeBase* gm = GetPlanetGameMode(this))
	{
		if (mEnemy)
		{
			mEnemy->HandleDied();
		}
		else if (mPlanet)
		{
			// gm->GameOver();
		}
	}
}
