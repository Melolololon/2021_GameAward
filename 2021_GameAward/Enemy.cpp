#include "Enemy.h"
#include "Block.h"
#include "PlayerBullet.h"
#include "SceneManager.h"
#include "Play.h"

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
		if (attackAfterTimer >= 60 * 2)
		{
			attackAfterTimer--;
			//ここにプレイヤーの体力を減らす処理
			pPlayer->DamageFromEnemy();
		}
	}

	//プレイヤーの弾との衝突判定
	if (typeid(*object) == typeid(PlayerBullet))
	{
		hp--;
		if (hp <= 0)
		{
			//ここにスコアを与える処理

			eraseManager = true;
		}
	}
}

void Enemy::loadModel()
{
}

void Enemy::setPosition(Vector3 pos)
{
}

bool Enemy::isGameStart()
{
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	//準備前は動かないように
	if (typeid(*currentScene) == typeid(Play))
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET ||
			Play::GetPlaySceneState() == Play::PLAY_SCENE_START_PREVIOUS)
		{
			return false;
		}
	}

	return true;
}
