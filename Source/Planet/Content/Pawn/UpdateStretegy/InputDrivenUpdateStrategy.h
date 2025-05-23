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
	
	virtual void OnLookInput(const FVector2D& _inputValue) override
	{
		if (mEnemy)
		{
			mEnemy->MoveStep(_inputValue.Size());
		}
	}
	
	virtual void Update(float _deltaTime) override
	{
	}

	static constexpr float InputDrivenUpdateScale = 0.05f;

private:
	AEnemyPawn* mEnemy;
};