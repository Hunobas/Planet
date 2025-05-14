// JustAimManagerComponent.cpp
#include "JustAimManagerComponent.h"

#include "Camera/CameraComponent.h"

#include "../Planet.h"
#include "PlanetPawn.h"
#include "EnemySpawnCelestial.h"
#include "PlanetController.h"
#include "PlayCamera.h"

UJustAimManagerComponent::UJustAimManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UJustAimManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	cPlayerPawn = Cast<APlanetPawn>(GetOwner());
	check(cPlayerPawn);

	AEnemySpawnCelestial* enemySpawn;
	TryGetFirstActorWithTag(GetWorld(), DEFAULT_ENEMY_SPAWN_TAG, enemySpawn);
	check(enemySpawn);

	mDistanceToSpawnCelestial = enemySpawn->EnemySpawnRadius;
}

bool UJustAimManagerComponent::HasJustAimed(const USceneComponent* _firePoint)
{
	// 1. 플레이어에서 발사 지점으로 향하는 벡터
	check(_firePoint);
	const FVector playerForwardPos		= cPlayerPawn->GetActorLocation() + cPlayerPawn->Camera->GetForwardVector() * mDistanceToSpawnCelestial;
	const FVector playerToFirePoint		= _firePoint->GetComponentLocation() - playerForwardPos;

	if (!IsInJustAimWindow)
	{
		mCachedDestinationNorm = playerToFirePoint.GetSafeNormal();
		IsInJustAimWindow = true;
	}
    
	// 2. 플레이어의 현재 시선 입력을 3D 벡터로 변환
	APlanetController* playerController = Cast<APlanetController>(cPlayerPawn->GetController());
	check(playerController);
	const FVector2D currentAverageInput = playerController->GetEMAInput();
	const FVector inputDirection		= FVector(currentAverageInput.X, currentAverageInput.Y, 0.0f).GetSafeNormal();
    
	// 3. 플레이어 카메라의 우측 및 상단 벡터
	const FVector cameraRight			= cPlayerPawn->Camera->GetRightVector();
	const FVector cameraUp				= cPlayerPawn->Camera->GetUpVector();
    
	// 4. 입력 방향을 월드 공간으로 변환 (플레이어 시선이 향하는 방향)
	const FVector worldInputDirection	= cameraRight * inputDirection.X + cameraUp * inputDirection.Y;
    
	// 5. 두 벡터 간의 각도 계산
	const float angleBetween			= FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(worldInputDirection.GetSafeNormal(), mCachedDestinationNorm)));
    
	// 6. 거리 및 각도 확인
	check(!FMath::IsNearlyZero(JustAimWindowRadius));
	const float distance				= playerToFirePoint.Size();
	const float maxAngle				= FMath::Lerp(0.75f * JustAimWindowAngle, JustAimWindowAngle, FMath::Clamp(distance / JustAimWindowRadius, 0.0f, 1.0f));
	const float inputSpeed				= currentAverageInput.Size();
    
	// 거리 내에 있고, 입력 마우스 속도가 일정값 이상이며, 각도가 허용 범위 내인 경우 성공
	return distance < JustAimWindowRadius && inputSpeed >= JustAimInputSpeedThreshold && angleBetween < maxAngle;
}

void UJustAimManagerComponent::SucceedJustAim(USceneComponent* _firePoint)
{
	check(_firePoint);

	IsInJustAimWindow = false;
	cPlayerPawn->PlayCamera->OnJustAimSuccess(_firePoint->GetComponentLocation());
	
	SpawnSystemAttachedFacingForward(JustAimSuccessTemplate, _firePoint);
}

void UJustAimManagerComponent::FailJustAim()
{
	IsInJustAimWindow = false;
}

