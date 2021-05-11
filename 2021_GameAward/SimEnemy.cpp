#include "SimEnemy.h"
#include "ObjectManager.h"

PrimitiveModel SimEnemy::modelData;
int SimEnemy::createCount;
const int SimEnemy::CREATE_NUMBER = 1;

SimEnemy::SimEnemy()
{
	Initialize();
}

void SimEnemy::Initialize()
{
	setHeapNum();
	hp = 2;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
}

void SimEnemy::Update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	setPosition(position);

}

void SimEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void SimEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void SimEnemy::LoadResource()
{
//	std::string mtl;
//
//	modelData.key = "Simenemy";
//	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
//	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 161,203,116,255 }, CREATE_NUMBER);
}

void SimEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void SimEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}