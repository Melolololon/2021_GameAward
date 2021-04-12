#include "TargetObject.h"
#include"SceneManager.h"
#include"Player.h"
#include"Block.h"
#include"PlayerBullet.h"
ModelData TargetObject::modelData;
int TargetObject::createCount;
const int TargetObject::CREATE_NUMBER = 100;
bool TargetObject::hitSegment;

TargetObject::TargetObject(const Vector3& pos) 
{
	position = pos;
	initialize();
}

TargetObject::~TargetObject()
{
}


void TargetObject::loadModel()
{
	std::string mtl;

	modelData.key = "tergetObject";
	Library::create3DBox({ 2,5,2 }, modelData);
	Library::createHeapData2({120,200,120,255}, CREATE_NUMBER, modelData);
}

void TargetObject::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void TargetObject::initialize()
{
	setHeapNum();

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].position = position;
	boxData[0].size = { 2,5,2 };
	//collisionFlag.sphere = true;
	//sphereData.resize(1);
	//sphereData[0].position = position;
	//sphereData[0].r = 1.0f;

	Library::setPosition(position, modelData, heapNum);
	
	hp = 20;
	setEnd = false;
}

void TargetObject::update()
{
	hitSegment = false;
	createHitObject = false;


	boxData[0].position = position;
	Library::setPosition(position, modelData, heapNum);


	if (hp <= 0)
		eraseManager = true;
}

void TargetObject::draw()
{
	
	Library::drawGraphic(modelData, heapNum);
}


void TargetObject::hit
(
	const Object *const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(Player))
		createHitObject = true;
	if(typeid(*object) == typeid(Block))
		createHitObject = true;


	if (typeid(*object) == typeid(PlayerBullet))
		hp--;

}


void TargetObject::trueEraseManager() { eraseManager = true; }

void TargetObject::trueSetEnd() { setEnd = true; }

bool TargetObject::getSetEnd() { return setEnd; }
bool TargetObject::getCreateHitObject() { return createHitObject; }

void TargetObject::setPosition(const Vector3& pos)
{
	position = pos;
	Library::setPosition(position, modelData, heapNum);
	boxData[0].position = position;
}


