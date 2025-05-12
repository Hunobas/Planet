// EnemyPawn.h
#include "EnemyPawn.h"

#include "Components/CapsuleComponent.h"

#include "EnemySetting.h"
#include "EnemyScaleSetting.h"
#include "EnemyDataAsset.h"

AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body Collision Capsule"));
	RootComponent = BodyCollisionCapsule;

	HitDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Detection Capsule"));
	HitDetectionCapsule->SetupAttachment(RootComponent);
}

void AEnemyPawn::ResetToDefaultSettings(const FEnemyScaleSetting& scaleSettings)
{
	RuntimeSettings.HP			= BaseSettings->HPBase * scaleSettings.HPScale;
	RuntimeSettings.Damage		= BaseSettings->DamageBase * scaleSettings.DamageScale;
	RuntimeSettings.Speed		= BaseSettings->SpeedBase * scaleSettings.SpeedScale;
	RuntimeSettings.XPDrop		= BaseSettings->XPDropBase * scaleSettings.XPDropScale;
	RuntimeSettings.FieldScore	= BaseSettings->FieldScoreBase;
}