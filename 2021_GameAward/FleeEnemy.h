#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
#pragma region �ǉ��p�����[�^
	//�����鎞��
	int escapeTimer = 300;
#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};