#include "EnemyParticle.h"

std::vector<Texture> EnemyParticle::parTex;

EnemyParticle::EnemyParticle(const Vector3& pos)
{
	Initialize();

	parSpr.CreateSprite(Vector3());
	parSpr.SetPosition(pos);

	
}

EnemyParticle::~EnemyParticle()
{
}

void EnemyParticle::LoadResources()
{
	for(int i = 0;;i++)
	{
		parTex[i].LoadSpriteTexture("Resources/Texture/EnemyParticle" + std::to_string(i + 1) + ".png");
	}
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
	for (auto& tex : parTex)
	{
		parSpr.Draw(&tex);
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
