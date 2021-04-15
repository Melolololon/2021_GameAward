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
	setPosition(position);
}

void MoveEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "moveenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 180,153,108,255 }, CREATE_NUMBER, modelData);
}

