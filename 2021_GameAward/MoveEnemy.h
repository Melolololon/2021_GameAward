#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
#pragma region 追加パラメータ

#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};

