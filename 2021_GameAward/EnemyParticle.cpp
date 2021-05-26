#include "EnemyParticle.h"

EnemyParticle::EnemyParticle(const Vector3& pos)
{
	Initialize();

	parSpr.SetPosition(pos);

	for(int i = 0; i < _countof(parTex);i++)
	{
		parTex[i];
	}
}

EnemyParticle::~EnemyParticle()
{
}

void EnemyParticle::Initialize()
{
	parSpr.CreateSprite(Vector2(2, 2));
	parSpr.SetBillboardFlag(true, true, true);
}

void EnemyParticle::Update()
{
}

void EnemyParticle::Draw()
{
	for (int i = 0; i < _countof(parTex); i++) 
	{
		parSpr.Draw(&parTex[i]);
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
