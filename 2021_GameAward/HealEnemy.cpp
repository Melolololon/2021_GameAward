#include "HealEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"

PrimitiveModel HealEnemy::modelData;
int HealEnemy::createCount;
const int HealEnemy::CREATE_NUMBER = GENERATE_COUNT;
HeapIndexManager HealEnemy::heapIndexManager(CREATE_NUMBER);

HealEnemy::HealEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

HealEnemy::~HealEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void HealEnemy::Initialize()
{
	hp = 2;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
}

void HealEnemy::Update()
{
	if (isGameStart() == false)
	{
		setPosition(position);
		return;
	}

	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	setPosition(position);

}

void HealEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void HealEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void HealEnemy::LoadResource()
{
	//	std::string mtl;
	//
	//	modelData.key = "Healenemy";
	//	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 200,112,28,255 }, CREATE_NUMBER);
}

void HealEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void HealEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}