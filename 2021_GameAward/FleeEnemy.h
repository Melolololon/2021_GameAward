#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
#pragma region ’Ç‰Áƒpƒ‰ƒ[ƒ^
	//“¦‚°‚éŠÔ
	int escapeTimer = 300;
#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};