#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
#pragma region 追加パラメータ
	//逃げる時間
	int escapeTimer = 300;
#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};