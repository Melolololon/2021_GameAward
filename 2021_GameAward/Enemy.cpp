#include "Enemy.h"
#include "PlayerBullet.h"
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

void Enemy::hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
	//åãç\ã∑Ç¢åÑä‘Ç≈Ç‡í ÇÍÇøÇ·Ç§Ç©ÇÁèCê≥Ç∑ÇÈÇ©ÅAåÑä‘Ç™Ç≈Ç´Ç»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
	if (typeid(*object) == typeid(Player) ||
		typeid(*object) == typeid(PlayerBullet))
	{

	}

}

void Enemy::loadModel()
{
	std::string mtl;

	modelData.key = "enemy";
	Library::create3DBox(Vector3{1,1,1}, modelData);
	Library::createHeapData2({ 0,0,255,255 }, CREATE_NUMBER, modelData);
}

void Enemy::setPosition(Vector3 pos)
{
	position = pos;
	Library::setPosition(position, modelData, heapNum);
}
