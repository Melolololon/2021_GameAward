#include "Enemy.h"
#include "Block.h"
#include "PlayerBullet.h"

const float Enemy::OBJSIZE = 1.0f;

void Enemy::setHeapNum()
{
}

void Enemy::Initialize()
{
	setHeapNum();
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
	//ブロックとの衝突判定
	if (typeid(*object) == typeid(Block))
	{
		position -= velocity * moveSpeed;

		setPosition(position);

	}

	//プレイヤーとの衝突判定
	if (typeid(*object) == typeid(Player))
	{

	}

	//プレイヤーの弾との衝突判定
	if (typeid(*object) == typeid(PlayerBullet))
	{
		hp--;
		if (hp <= 0)
			eraseManager = true;
	}

}

void Enemy::loadModel()
{
}

void Enemy::setPosition(Vector3 pos)
{
}
