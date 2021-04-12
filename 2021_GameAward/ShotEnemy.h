#pragma once
#include "Enemy.h"
class ShotEnemy :
	public Enemy
{
private:
#pragma region 追加パラメータ
	//球を撃つ間隔
	float shotInterval = 3;
#pragma endregion

public:
	void initialize()override;
	void update()override;
};