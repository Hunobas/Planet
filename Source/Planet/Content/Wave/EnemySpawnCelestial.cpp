// EnemySpawnCelestial.cpp
#include "EnemySpawnCelestial.h"

#include "EnemyPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

#include "PlanetPawn.h"

AEnemySpawnCelestial::AEnemySpawnCelestial()
{
	PrimaryActorTick.bCanEverTick = true;

	composeSpawnPointScenes();
}

AEnemySpawnCelestial* AEnemySpawnCelestial::Initialize(APawn* _targetPawn)
{
	cPlayerPawn = _targetPawn;
	PlayerCamera = Cast<APlanetPawn>(_targetPawn)->Camera;
	mIsPointsActive.Init(false, SpawnPoints.Num());
	mIsPointsOccupied.Init(false, SpawnPoints.Num());

	FAttachmentTransformRules attachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		false
	);
	AttachToComponent(cPlayerPawn->GetRootComponent(), attachmentRules);

	return this;
}

USceneComponent* AEnemySpawnCelestial::GetRandomSpawnPoint()
{
	updatePlayerFacingSpawnPoint();

	return SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
}

USceneComponent* AEnemySpawnCelestial::GetRandomActiveSpawnPointOrNull()
{
	updatePlayerFacingSpawnPoint();
	
	TArray<USceneComponent*> activeSpawnPoints;
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		if (mIsPointsActive[i] && !mIsPointsOccupied[i])
		{
			activeSpawnPoints.Add(SpawnPoints[i]);
		}
	}

	if (activeSpawnPoints.Num() > 0)
	{
#ifdef DEBUG
		for (USceneComponent* spawnPoint : activeSpawnPoints)
		{
			DrawDebugBox(
				GetWorld(),
				spawnPoint->GetComponentLocation(),
				FVector::OneVector * 100,
				FColor::Orange,
				false,
				1.0f,
				0,
				1.0f
			);
		}
#endif
		
		return activeSpawnPoints[FMath::RandRange(0, activeSpawnPoints.Num() - 1)];
	}
	return nullptr;
}

TArray<USceneComponent*> AEnemySpawnCelestial::GetRandomRowSpawnPoints() const
{
	const int32 targetRow	= FMath::RandRange(1, NumRows);
	const int32 startIndex	= (targetRow - 1) * PointsPerRow;
	const int32 endIndex	= targetRow * PointsPerRow;

	TArray<USceneComponent*> targetRowPoints;
	for (int32 i = startIndex; i < endIndex && i < SpawnPoints.Num(); i++)
	{
		targetRowPoints.Add(SpawnPoints[i]);
	}

	return targetRowPoints;
}

TArray<USceneComponent*> AEnemySpawnCelestial::GetNthRowSpawnPoints(const int& n) const
{
	check(n > 0 && n <= NumRows);
	const int32 startIndex	= (n - 1) * PointsPerRow;
	const int32 endIndex	= n * PointsPerRow;

	TArray<USceneComponent*> targetRowPoints;
	for (int32 i = startIndex; i < endIndex && i < SpawnPoints.Num(); i++)
	{
		targetRowPoints.Add(SpawnPoints[i]);
	}

	return targetRowPoints;
}

void AEnemySpawnCelestial::SetOccupiedSpawnPoint(USceneComponent* _spawnPoint, bool _active)
{
	int32 i  = SpawnPoints.Find(_spawnPoint);
	if (i != INDEX_NONE)
	{
		mIsPointsOccupied[i] = _active;
	}
}

void AEnemySpawnCelestial::SetActiveAllSpawnPoints(bool _active)
{
	for (bool& active : mIsPointsActive)
	{
		active = _active;
	}
}

void AEnemySpawnCelestial::composeSpawnPointScenes()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	int32 pointIndex = 0;
	for (int32 row = 0; row < NumRows; row++)
	{
		float pitch = (row - (NumRows - 1) / 2.0f) * PitchInterval;

		for (int32 pointInRow = 0; pointInRow < PointsPerRow; pointInRow++)
		{
			float yaw = pointInRow * YawInterval;

			FString spawnPointName = FString::Printf(TEXT("Spawn Point %02d"), pointIndex++ + 1);

			FVector position;
			position.X = EnemySpawnRadius * FMath::Cos(FMath::DegreesToRadians(pitch)) * FMath::Cos(FMath::DegreesToRadians(yaw));
			position.Y = EnemySpawnRadius * FMath::Cos(FMath::DegreesToRadians(pitch)) * FMath::Sin(FMath::DegreesToRadians(yaw));
			position.Z = EnemySpawnRadius * FMath::Sin(FMath::DegreesToRadians(pitch));

			USceneComponent* spawnPoint = CreateDefaultSubobject<USceneComponent>(*spawnPointName);
			spawnPoint->SetupAttachment(RootComponent);
			spawnPoint->SetRelativeLocation(position);
            
			FRotator rotation = (-position).Rotation();
			spawnPoint->SetRelativeRotation(rotation);

			SpawnPoints.Add(spawnPoint);
		}
	}
}

void AEnemySpawnCelestial::updatePlayerFacingSpawnPoint()
{
	check(PlayerCamera);
	float camYaw = PlayerCamera->GetComponentRotation().Yaw;
	
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		float pointYaw = SpawnPoints[i]->GetRelativeLocation().Rotation().Yaw;
		float deltaYaw = FMath::FindDeltaAngleDegrees(camYaw, pointYaw);
		
		mIsPointsActive[i] = FMath::Abs(deltaYaw) <= HalfFOV;
	}
}