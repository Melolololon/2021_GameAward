#include "EnemyBullet.h"
#include"TargetObject.h"
#include"Block.h"
#include"Player.h"

PrimitiveModel EnemyBullet::modelData;
int EnemyBullet::createCount;
const int EnemyBullet::CREATE_NUMBER = 100;


EnemyBullet::EnemyBullet(const Vector3& pos, const Vector3& vel)
{
	Initialize();
	position = pos;
	velocity = vel;
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void EnemyBullet::LoadResource()
{
	/*modelData.key = "eBullet";
	Library::create3DBox({ 0.2,0.2,0.2 }, modelData);
	Library::createHeapData2({ 255,255,0,255 }, CREATE_NUMBER, modelData);*/
	modelData.CreateBox({ 0.2,0.2,0.2 }, { 255,255,0,255 }, CREATE_NUMBER);
}

void EnemyBullet::Initialize()
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

void EnemyBullet::Update()
{
	if (heapNum == -1)
	{
		eraseManager = true;
		return;
	}

	deadTimer++;
	if (deadTimer >= deadTime)
		eraseManager = true;

	position += velocity * speed;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}

void EnemyBullet::Draw()
{
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}

void EnemyBullet::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(TargetObject) ||
		typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(Player))
		eraseManager = true;
}
