// JustAimManagerComponent.cpp
#include "JustAimManagerComponent.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "PlanetController.h"
#include "PlayCamera.h"

UJustAimManagerComponent::UJustAimManagerComponent(): JustAimSuccessTemplate(nullptr), cPlayerPawn(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UJustAimManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	cPlayerPawn = Cast<APlanetPawn>(GetOwner());
	check(cPlayerPawn);
}

bool UJustAimManagerComponent::HasJustAimed(const USceneComponent* _firePoint) const
{
	check(cPlayerPawn);
	APlanetController* playerController = Cast<APlanetController>(cPlayerPawn->GetController());
	
	check(_firePoint);
	const FVector mouseDir	= (playerController->WorldMouseLocation - cPlayerPawn->GetActorLocation()).GetSafeNormal();
	const FVector fireDir	= (_firePoint->GetComponentLocation() - cPlayerPawn->GetActorLocation()).GetSafeNormal();
	
	const float angleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(mouseDir, fireDir)));

	return angleBetween < JustAimWindowAngle;
}

void UJustAimManagerComponent::SucceedJustAim(USceneComponent* _firePoint) const
{
	check(_firePoint);

	cPlayerPawn->PlayCamera->OnJustAimSuccess(_firePoint->GetComponentLocation());
	
	SpawnSystemAttachedFacingForward(JustAimSuccessTemplate, _firePoint);
}

void UJustAimManagerComponent::FailJustAim()
{
}

