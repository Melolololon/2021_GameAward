#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
#pragma region �ǉ��p�����[�^

#pragma endregion

public:
	void initialize()override;
	void update()override;
};

