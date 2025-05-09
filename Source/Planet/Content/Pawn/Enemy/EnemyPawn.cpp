// EnemyPawn.h
#include "EnemyPawn.h"

#include "Components/CapsuleComponent.h"

#include "EnemySetting.h"
#include "EnemyDataAsset.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body Collision Capsule"));
	RootComponent = BodyCollisionCapsule;

	HitDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Detection Capsule"));
	HitDetectionCapsule->SetupAttachment(RootComponent);
}

void AEnemyPawn::ResetToDefaultSettings()
{
	RuntimeSettings.HP			= BaseSettings->HPBase;
	RuntimeSettings.Damage		= BaseSettings->DamageBase;
	RuntimeSettings.Speed		= BaseSettings->SpeedBase;
	RuntimeSettings.XPDrop		= BaseSettings->XPDropBase;
	RuntimeSettings.FieldScore	= BaseSettings->FieldScoreBase;
}

