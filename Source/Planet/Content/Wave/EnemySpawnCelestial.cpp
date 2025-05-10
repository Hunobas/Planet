// EnemySpawnCelestial.cpp
#include "EnemySpawnCelestial.h"

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
	cTargetPawn = _targetPawn;
	cTargetCamera = Cast<APlanetPawn>(_targetPawn)->Camera;
	mIsPointsActive.Init(false, SpawnPoints.Num());

	return this;
}

void AEnemySpawnCelestial::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	
	check(cTargetPawn);
	FVector pawnLocation = cTargetPawn->GetActorLocation();
	SetActorLocation(pawnLocation);

	updateSpawnPointActivation();

#ifdef DEBUG
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		if (mIsPointsActive[i])
		{
			DrawDebugBox(
				GetWorld(),
				pawnLocation + SpawnPoints[i]->GetRelativeLocation(),
				FVector::OneVector * 100,
				FColor::Orange,
				false,
				0.1f,
				0,
				1.0f
			);
		}
	}
#endif
}

USceneComponent* AEnemySpawnCelestial::GetRandomSpawnPoint()
{
	updateSpawnPointActivation();

	return SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
}

USceneComponent* AEnemySpawnCelestial::GetRandomActiveSpawnPointOrNull()
{
	updateSpawnPointActivation();
	
	TArray<USceneComponent*> activeSpawnPoints;
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		if (mIsPointsActive[i])
		{
			activeSpawnPoints.Add(SpawnPoints[i]);
		}
	}

	if (activeSpawnPoints.Num() > 0)
	{
		return activeSpawnPoints[FMath::RandRange(0, activeSpawnPoints.Num() - 1)];
	}
	return nullptr;
}

TArray<USceneComponent*> AEnemySpawnCelestial::GetRandomRowSpawnPoints() const
{
	const int32 targetRow	= FMath::RandRange(1, NumRows);
	const int32 startIndex	= (targetRow - 1) * PointsPerRow / 2;
	const int32 endIndex	= targetRow * PointsPerRow / 2;

	TArray<USceneComponent*> targetRowPoints;
	for (int32 i = startIndex; i < endIndex && i < SpawnPoints.Num(); i++)
	{
		targetRowPoints.Add(SpawnPoints[i]);
	}

	return targetRowPoints;
}

void AEnemySpawnCelestial::SetActiveSpawnPoint(USceneComponent* _spawnPoint, bool _active)
{
	int32 i  = SpawnPoints.Find(_spawnPoint);
	if (i != INDEX_NONE)
	{
		mIsPointsActive[i] = _active;
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
			position.X = SphereRadius * FMath::Cos(FMath::DegreesToRadians(pitch)) * FMath::Cos(FMath::DegreesToRadians(yaw));
			position.Y = SphereRadius * FMath::Cos(FMath::DegreesToRadians(pitch)) * FMath::Sin(FMath::DegreesToRadians(yaw));
			position.Z = SphereRadius * FMath::Sin(FMath::DegreesToRadians(pitch));

			USceneComponent* spawnPoint = CreateDefaultSubobject<USceneComponent>(*spawnPointName);
			spawnPoint->SetupAttachment(RootComponent);
			spawnPoint->SetRelativeLocation(position);
            
			FRotator rotation = (-position).Rotation();
			spawnPoint->SetRelativeRotation(rotation);

			SpawnPoints.Add(spawnPoint);
		}
	}
}

void AEnemySpawnCelestial::updateSpawnPointActivation()
{
	check(cTargetCamera);
	float camYaw = cTargetCamera->GetComponentRotation().Yaw;
	
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		float pointYaw = SpawnPoints[i]->GetRelativeLocation().Rotation().Yaw;
		float deltaYaw = FMath::FindDeltaAngleDegrees(camYaw, pointYaw);

		mIsPointsActive[i] = FMath::Abs(deltaYaw) <= HalfFOV;
	}
}
