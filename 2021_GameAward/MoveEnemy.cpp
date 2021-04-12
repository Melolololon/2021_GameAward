#include "MoveEnemy.h"

void MoveEnemy::initialize()
{
	setHeapNum();
}

void MoveEnemy::update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = {pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//正規化
	velocity = vector3Normalize(velocity);

	//座標更新
	position = position + velocity * moveSpeed;
	Library::setPosition(position, modelData, heapNum);
}
