// PlanetHUD.cpp
#include "PlanetHUD.h"

#include "Blueprint/UserWidget.h"

#include "../Planet.h"
#include "CursorWidget.h"

void APlanetHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CursorWidgetClass)
	{
		mCursorWidget = CreateWidget<UCursorWidget>(GetWorld(), CursorWidgetClass);
		mCursorWidget->AddToViewport(HIGH_ORDER);
	}
}