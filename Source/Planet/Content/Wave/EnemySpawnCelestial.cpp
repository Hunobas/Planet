// EnemySpawnCelestial.cpp
#include "EnemySpawnCelestial.h"

#include "Components/SceneComponent.h"

#include "PlanetPawn.h"

AEnemySpawnCelestial::AEnemySpawnCelestial() : cPlayerPawn(nullptr), PlayerCamera(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	composeSpawnPointScenes();
}

AEnemySpawnCelestial* AEnemySpawnCelestial::Initialize(APawn* _targetPawn)
{
	cPlayerPawn = _targetPawn;
	PlayerCamera = Cast<APlanetPawn>(_targetPawn)->Camera;
	mIsPointsOccupied.Init(false, RangedSpawnPoints.Num());

	FAttachmentTransformRules attachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		false
	);
	AttachToComponent(cPlayerPawn->GetRootComponent(), attachmentRules);

	return this;
}

USceneComponent* AEnemySpawnCelestial::GetRandomMeleeSpawnPoint()
{
	return MeleeSpawnPoints[FMath::RandRange(0, MeleeSpawnPoints.Num() - 1)];
}

USceneComponent* AEnemySpawnCelestial::GetRandomRangedSpawnPointOrNull()
{
	TArray<USceneComponent*> activeSpawnPoints;
	for (int32 i = 0; i < RangedSpawnPoints.Num(); i++)
	{
		if (!mIsPointsOccupied[i])
		{
			activeSpawnPoints.Add(RangedSpawnPoints[i]);
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

TArray<USceneComponent*> AEnemySpawnCelestial::GetNthRowSpawnPoints(const int32& n) const
{
	check(n > 0 && n <= NumCircles);
	const int32 startIndex	= (n - 1) * PointsPerCircle;
	const int32 endIndex	= n * PointsPerCircle;

	TArray<USceneComponent*> targetRowPoints;
	for (int32 i = startIndex; i < endIndex && i < MeleeSpawnPoints.Num(); i++)
	{
		targetRowPoints.Add(MeleeSpawnPoints[i]);
	}

	return targetRowPoints;
}

TArray<USceneComponent*> AEnemySpawnCelestial::GetRandomRowSpawnPoints() const
{
	const int32 targetRow = FMath::RandRange(1, NumCircles);
	return GetNthRowSpawnPoints(targetRow);
}

void AEnemySpawnCelestial::SetOccupiedSpawnPoint(USceneComponent* _spawnPoint, bool _active)
{
	int32 i  = RangedSpawnPoints.Find(_spawnPoint);
	if (i != INDEX_NONE)
	{
		mIsPointsOccupied[i] = _active;
	}
}

void AEnemySpawnCelestial::composeSpawnPointScenes()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MeleeSpawnPoints.Empty();
	RangedSpawnPoints.Empty();

	YawInterval = 360.0f / PointsPerCircle;
	int32 pointIndex = 0;

	for (int32 num = 0; num < NumCircles; ++num)
	{
		const float offsetMultiplier = num - (NumCircles - 1) / 2.0f;
		const float ringRadius       = MeleeSpawnRadius + offsetMultiplier * RadiusInterval;

		for (int32 point = 0; point < PointsPerCircle; ++point)
		{
			const float yawDeg = point * YawInterval;
			const float rad    = FMath::DegreesToRadians(yawDeg);

			const FVector position = FVector(
				ringRadius * FMath::Cos(rad),
				ringRadius * FMath::Sin(rad),
				0.0f
			);

			const FString pointName = FString::Printf(TEXT("SpawnPoint_%02d"), ++pointIndex);
			USceneComponent* spawnPoint = CreateDefaultSubobject<USceneComponent>(*pointName);

			spawnPoint->SetupAttachment(RootComponent);
			spawnPoint->SetRelativeLocation(position);

			const FRotator rotation = (-position).Rotation();
			spawnPoint->SetRelativeRotation(rotation);

			MeleeSpawnPoints.Add(spawnPoint);
		}
	}

	for (int32 point = 0; point < PointsPerCircle; ++point)
	{
		const float yawDeg = point * YawInterval;
		const float rad    = FMath::DegreesToRadians(yawDeg);

		const FVector position = FVector(
			RangedSpawnRadius * FMath::Cos(rad),
			RangedSpawnRadius * FMath::Sin(rad),
			0.0f
		);

		const FString pointName = FString::Printf(TEXT("SpawnPoint_%02d"), ++pointIndex);
		USceneComponent* spawnPoint = CreateDefaultSubobject<USceneComponent>(*pointName);

		spawnPoint->SetupAttachment(RootComponent);
		spawnPoint->SetRelativeLocation(position);

		const FRotator rotation = (-position).Rotation();
		spawnPoint->SetRelativeRotation(rotation);

		RangedSpawnPoints.Add(spawnPoint);
	}
}