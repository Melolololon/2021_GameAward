#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
#pragma region 追加パラメータ

#pragma endregion

public:
	void initialize()override;
	void update()override;
};

