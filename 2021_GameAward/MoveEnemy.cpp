#include "MoveEnemy.h"

ModelData MoveEnemy::modelData;
int MoveEnemy::createCount;
const int MoveEnemy::CREATE_NUMBER = 1;

MoveEnemy::MoveEnemy()
{
	initialize();
}

void MoveEnemy::initialize()
{
	setHeapNum();
	hp = 3;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.5f;
	
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

void MoveEnemy::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);

}

void MoveEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "moveenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 180,153,108,255 }, CREATE_NUMBER, modelData);
}

void MoveEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void MoveEnemy::setPosition(Vector3 pos)
{
	position = pos;
	Library::setPosition(position, modelData, heapNum);
}
