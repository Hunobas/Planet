// CursorWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CursorWidget.generated.h"

UCLASS()
class PLANET_API UCursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Cursor")
	void SetCursorPosition(FVector2D _newPosition);

protected:
	virtual void NativeTick(const FGeometry& _myGeometry, float _deltaTime) override;
};
