#pragma once
#include "Enemy.h"
class ShotEnemy :
	public Enemy
{
private:
#pragma region 追加パラメータ
	//球を撃つ間隔
	float shotInterval = 3;
	int shotWaitTimer = 60;
#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};