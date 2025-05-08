// AEnemySpawnCelestial.cpp
#include "EnemySpawnCelestial.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

#include "PlanetPawn.h"

#define DEBUG

AEnemySpawnCelestial::AEnemySpawnCelestial()
{
	PrimaryActorTick.bCanEverTick = true;

	int32 PointIndex = 0;
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		float Pitch = (Row - (NumRows - 1) / 2.0f) * PitchInterval;

		for (int32 PointInRow = 0; PointInRow < PointsPerRow; ++PointInRow)
		{
			float Yaw = PointInRow * YawInterval;

			FString SpawnPointName = FString::Printf(TEXT("Spawn Point %02d"), PointIndex++ + 1);
			USceneComponent* SpawnPoint = CreateDefaultSubobject<USceneComponent>(*SpawnPointName);
			SpawnPoint->SetupAttachment(RootComponent);

			FVector Position;
			Position.X = SphereRadius * FMath::Cos(FMath::DegreesToRadians(Pitch)) * FMath::Cos(FMath::DegreesToRadians(Yaw));
			Position.Y = SphereRadius * FMath::Cos(FMath::DegreesToRadians(Pitch)) * FMath::Sin(FMath::DegreesToRadians(Yaw));
			Position.Z = SphereRadius * FMath::Sin(FMath::DegreesToRadians(Pitch));

			SpawnPoint->SetRelativeLocation(Position);
            
			FRotator Rotation = (-Position).Rotation();
			SpawnPoint->SetRelativeRotation(Rotation);

			SpawnPoints.Add(SpawnPoint);
		}
	}
}

AEnemySpawnCelestial* AEnemySpawnCelestial::Initialize(APawn* InTargetPawn)
{
	cTargetPawn = InTargetPawn;
	cTargetCamera = Cast<APlanetPawn>(InTargetPawn)->Camera;
	mIsPointsActive.Init(false, SpawnPoints.Num());

	return this;
}

void AEnemySpawnCelestial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PawnLocation = cTargetPawn->GetActorLocation();
	SetActorLocation(PawnLocation);

	updateSpawnPointActivation();

#ifdef DEBUG
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		if (mIsPointsActive[i])
		{
			DrawDebugBox(
				GetWorld(),
				PawnLocation + SpawnPoints[i]->GetRelativeLocation(),
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

USceneComponent* AEnemySpawnCelestial::GetRandomActiveSpawnPointOrNull()
{
	updateSpawnPointActivation();
	
	TArray<USceneComponent*> ActiveSpawnPoints;
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		if (mIsPointsActive[i])
		{
			ActiveSpawnPoints.Add(SpawnPoints[i]);
		}
	}

	if (ActiveSpawnPoints.Num() > 0)
	{
		return ActiveSpawnPoints[FMath::RandRange(0, ActiveSpawnPoints.Num() - 1)];
	}
	return nullptr;
}

TArray<USceneComponent*> AEnemySpawnCelestial::GetRandomRowSpawnPoints() const
{
	int32 TargetRow = FMath::RandRange(1, NumRows);
	TArray<USceneComponent*> TargetRowPoints;

	int32 StartIndex = (TargetRow - 1) * PointsPerRow / 2;
	int32 EndIndex = TargetRow * PointsPerRow / 2;

	for (int32 i = StartIndex; i < EndIndex && i < SpawnPoints.Num(); i++)
	{
		TargetRowPoints.Add(SpawnPoints[i]);
	}
	check(!TargetRowPoints.IsEmpty());

	return TargetRowPoints;
}

void AEnemySpawnCelestial::SetActiveSpawnPoint(USceneComponent* AimPoint, bool Active)
{
	int32 Index = SpawnPoints.Find(AimPoint);
	if (Index != INDEX_NONE)
	{
		mIsPointsActive[Index] = Active;
	}
}

void AEnemySpawnCelestial::SetActiveAllSpawnPoints(bool Active)
{
	for (bool& Point : mIsPointsActive)
	{
		Point = Active;
	}
}

void AEnemySpawnCelestial::updateSpawnPointActivation()
{
	float CamYaw = cTargetCamera->GetComponentRotation().Yaw;
	
	for (int32 i = 0; i < SpawnPoints.Num(); ++i)
	{
		float PointYaw = SpawnPoints[i]->GetRelativeLocation().Rotation().Yaw;
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CamYaw, PointYaw);

		mIsPointsActive[i] = FMath::Abs(DeltaYaw) <= HalfFOV;
	}
}

