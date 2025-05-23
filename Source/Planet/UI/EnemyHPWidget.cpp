// EnemyHPWidget.cpp
#include "EnemyHPWidget.h"

#include "Components/ProgressBar.h"

void UEnemyHPWidget::UpdateHealthProgress(const float& _percent)
{
	if (!HPProgressBar)
		return;
	
	HPProgressBar->SetPercent(FMath::Clamp(_percent, 0, 1.0f));
}
