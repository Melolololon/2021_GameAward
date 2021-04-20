#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"

void ShotEnemy::initialize()
{
	setHeapNum();
	hp = 2;
}

void ShotEnemy::update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//正規化
	velocity = vector3Normalize(velocity);

	//一定間隔以上なら座標更新
	if (sqrt((pPlayer->getHeadPosition().x - position.x) * (pPlayer->getHeadPosition().x - position.x) +
		(pPlayer->getHeadPosition().y - position.y) * (pPlayer->getHeadPosition().y - position.y) +
		(pPlayer->getHeadPosition().z - position.z) * (pPlayer->getHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		position = position + velocity * moveSpeed;
	}
	//距離が一定未満だったら停止・弾を撃つ
	else
	{
		//ここに弾を撃つ処理
		if (shotWaitTimer == 60)
			ObjectManager::getInstance()->addObject(std::make_shared<EnemyBullet>(position, velocity));

		shotWaitTimer--;

	}
	if (shotWaitTimer < 0) shotWaitTimer = 60;
	setPosition(position);

}

void ShotEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "shotenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
}
