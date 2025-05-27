// ContinuousUpdateStrategy.h
#pragma once

#include "IUpdateStrategy.h"
#include "EnemyPawn.h"

class ContinuousUpdateStrategy : public IUpdateStrategy
{
public:
	ContinuousUpdateStrategy(APawn* _owner)
	{
		if (AEnemyPawn* enemy = Cast<AEnemyPawn>(_owner))
		{
			mEnemy = enemy;
		}
	}
	
	virtual void Update(float _deltaTime) override
	{
		if (mEnemy)
		{
			mEnemy->MoveStep(_deltaTime);
		}
	}

private:
	AEnemyPawn* mEnemy;
};