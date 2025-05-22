// InputDrivenUpdateStrategy.h
#pragma once

#include "IUpdateStrategy.h"
#include "EnemyPawn.h"

class InputDrivenUpdateStrategy : public IUpdateStrategy
{
public:
	InputDrivenUpdateStrategy(APawn* _owner)
	{
		if (AEnemyPawn* enemy = Cast<AEnemyPawn>(_owner))
		{
			mEnemy = enemy;
		}
	}
	
	virtual void OnLookInput(const FVector2D& _lookValue) override
	{
		if (mEnemy)
		{
			mEnemy->MoveStep(_lookValue.Size());
		}
	}
	
	virtual void Update(float _deltaTime) override
	{
	}

private:
	AEnemyPawn* mEnemy;
};