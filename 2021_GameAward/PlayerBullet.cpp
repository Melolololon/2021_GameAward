#include "PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"

ModelData PlayerBullet::modelData;
int PlayerBullet::createCount;
const int PlayerBullet::CREATE_NUMBER = 100;


PlayerBullet::PlayerBullet(const Vector3& pos, const Vector3& vel)
{
	initialize();
	position = pos;
	velocity = vel;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void PlayerBullet::loadModel()
{
	modelData.key = "pBullet";
	Library::create3DBox({ 0.2,0.2,0.2 }, modelData);
	Library::createHeapData2({ 255,255,0,255 }, CREATE_NUMBER, modelData);
}

void PlayerBullet::initialize()
{
	position = 0;
	velocity = 0;
	speed = 0.4;
	setHeapNum();

	deadTimer = 0;
	deadTime = 60 * 3;

	collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.1f;
}

void PlayerBullet::update()
{
	deadTimer++;
	if (deadTimer >= deadTime)
		eraseManager = true;

	position += velocity * speed;
	sphereData[0].position = position;
	Library::setPosition(position, modelData, heapNum);
}

void PlayerBullet::draw()
{
	Library::drawGraphic(modelData, heapNum);
}

void PlayerBullet::hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(TargetObject) ||
		typeid(*object) == typeid(Block))
		eraseManager = true;
}
