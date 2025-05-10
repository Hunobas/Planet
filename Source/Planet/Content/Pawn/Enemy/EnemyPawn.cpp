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

void AEnemyPawn::ResetToDefaultSettings(FEnemyScaleSetting _scaleSetting)
{
	RuntimeSettings.HP			= BaseSettings->HPBase * _scaleSetting.HPScale;
	RuntimeSettings.Damage		= BaseSettings->DamageBase * _scaleSetting.DamageScale;
	RuntimeSettings.Speed		= BaseSettings->SpeedBase * _scaleSetting.SpeedScale;
	RuntimeSettings.XPDrop		= BaseSettings->XPDropBase * _scaleSetting.XPDropScale;
	RuntimeSettings.FieldScore	= BaseSettings->FieldScoreBase;
}

