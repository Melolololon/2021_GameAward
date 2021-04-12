#pragma once
#include "Enemy.h"
class ShotEnemy :
	public Enemy
{
private:
#pragma region ’Ç‰Áƒpƒ‰ƒ[ƒ^
	//‹…‚ğŒ‚‚ÂŠÔŠu
	float shotInterval = 3;
#pragma endregion

public:
	void initialize()override;
	void update()override;
};