#include "FleeEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"

void FleeEnemy::initialize()
{
	setHeapNum();
	hp = 1;

}

void FleeEnemy::update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//正規化
	velocity = vector3Normalize(velocity);

	//一定間隔以上なら座標更新
	if (sqrt((pPlayer->getHeadPosition().x - position.x) * (pPlayer->getHeadPosition().x - position.x) +
		(pPlayer->getHeadPosition().y - position.y) * (pPlayer->getHeadPosition().y - position.y) +
		(pPlayer->getHeadPosition().z - position.z) * (pPlayer->getHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		position = position + velocity * moveSpeed;
	}
	//距離が一定未満だったら停止・弾を撃つ
	else
	{
		//逃げる
		if (escapeTimer < 300 - 60 * 2)
		{
			position = position - velocity * moveSpeed * 2;
		}
		//とまる
		else
		{

		}

		escapeTimer--;

	}
	if (escapeTimer < 0) escapeTimer = 300;

	setPosition(position);

}

void FleeEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "fleeenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 220,144,201,255 }, CREATE_NUMBER, modelData);
}
