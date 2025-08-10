// RewardTextDataTable.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RewardTextDataTable.generated.h"

USTRUCT(BlueprintType)
struct PLANET_API FRewardTextData : public FTableRowBase {
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> LevelDescriptions;
};