#include "EnemyParticle.h"

std::vector<Texture> EnemyParticle::parTex(40);
EnemyParticle::EnemyParticle(const Vector3& pos)
{
	Initialize();
	parSpr.SetPosition(pos);


}

EnemyParticle::~EnemyParticle()
{
}

void EnemyParticle::LoadResources()
{
	std::string numStr;
	for (int i = 0;; i++)
	{
		if (i + 1 == 7)continue;

		numStr = std::to_string(i + 1);
		
		while (1)
		{
			numStr.insert(0, "0");
			if (numStr.size() == 4)break;
		}

		parTex[i].LoadSpriteTexture("Resources/Texture/breakEnemyAnimation/" + numStr + ".png");
		if (i == parTex.size() - 1)
		{
			break;
		}
	}
}

void EnemyParticle::Initialize()
{
	parSpr.CreateSprite(Vector2(16 * 2, 9 * 2));
	parSpr.SetBillboardFlag(true, true, true);

	sortNumber = 2;
}

void EnemyParticle::Update()
{
	textureNum++;
	if(textureNum == parTex.size())
	{
		eraseManager = true;
	}
}

void EnemyParticle::Draw()
{
	if (textureNum != 6) 
	{
		parSpr.Draw(&parTex[textureNum]);
	}
}

void EnemyParticle::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{

}
