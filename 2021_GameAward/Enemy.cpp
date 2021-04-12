#include "Enemy.h"
ModelData Enemy::modelData;
int Enemy::createCount;
const int Enemy::CREATE_NUMBER = 1;

void Enemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Enemy::initialize()
{
	setHeapNum();
}

void Enemy::update()
{
}

void Enemy::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);

}

void Enemy::loadModel()
{
	std::string mtl;

	modelData.key = "enemy";
	Library::create3DBox(Vector3{5,5,5}, modelData);
	Library::createHeapData2({ 0,0,255,255 }, CREATE_NUMBER, modelData);
}
