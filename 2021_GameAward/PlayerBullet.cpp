#include "PlayerBullet.h"
#include "TargetObject.h"
#include "Block.h"
#include "Player.h"
#include "Enemy.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include "SimEnemy.h"
#include "DefenceEnemy.h"
#include "HealEnemy.h"

PrimitiveModel PlayerBullet::modelData;
const int PlayerBullet::CREATE_NUMBER = 100;
HeapIndexManager PlayerBullet::heapIndexManager(CREATE_NUMBER);

bool PlayerBullet::IsEnemy(const Object* p)
{

	if (typeid(*p) == typeid(MoveEnemy))
	{
		return true;
	}

	return false;

		
}

PlayerBullet::PlayerBullet(const Vector3& pos, const Vector3& vel)
{
	Initialize();
	position = pos;
	velocity = vel;
}

PlayerBullet::~PlayerBullet()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}


void PlayerBullet::LoadResource()
{
	/*modelData.key = "pBullet";
	Library::create3DBox({ 0.5,0.5,0.5 }, modelData);
	Library::createHeapData2({ 255,255,0,255 }, CREATE_NUMBER, modelData);*/
	modelData.CreateBox({ 0.5,0.5,0.5 }, { 255,255,0,255 }, CREATE_NUMBER);
}

void PlayerBullet::Initialize()
{
	position = 0;
	velocity = 0;
	speed = 0.4;
	heapNum = heapIndexManager.GetHeapIndex();

	deadTimer = 0;
	deadTime = 60 * 3;

	collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.25f;
}

void PlayerBullet::Update()
{
	deadTimer++;
	if (deadTimer >= deadTime)
		eraseManager = true;

	position += velocity * speed;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}

void PlayerBullet::Draw()
{
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}

void PlayerBullet::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(TargetObject) ||
		typeid(*object) == typeid(Block))
		eraseManager = true;


	if (typeid(*object) == typeid(Player) &&
		deadTimer >= 60 * 0.1f)
		eraseManager = true;


	if (IsEnemy(object))
		eraseManager = true;
}

