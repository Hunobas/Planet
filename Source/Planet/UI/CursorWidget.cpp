// CursorWidget.cpp
#include "CursorWidget.h"

#include "PlanetController.h"

void UCursorWidget::NativeTick(const FGeometry& _myGeometry, float _deltaTime)
{
	Super::NativeTick(_myGeometry, _deltaTime);

	if (APlanetController* PC = Cast<APlanetController>(GetOwningPlayer()))
	{
		SetPositionInViewport(PC->ScreenMousePosition);
	}
}

void UCursorWidget::SetCursorPosition(FVector2D _newPosition)
{
	SetPositionInViewport(_newPosition, false);
}
