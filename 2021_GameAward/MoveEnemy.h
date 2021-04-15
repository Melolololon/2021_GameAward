#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
#pragma region ’Ç‰Áƒpƒ‰ƒ[ƒ^

#pragma endregion

public:
	static void loadModel();
	void initialize()override;
	void update()override;
};

