#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
#pragma region �ǉ��p�����[�^

#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};

