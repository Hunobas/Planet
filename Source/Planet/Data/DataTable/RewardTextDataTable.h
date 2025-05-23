// RewardTextDataTable.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RewardTextDataTable.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FRewardTextData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TArray<FText> LevelDescriptions;
};
