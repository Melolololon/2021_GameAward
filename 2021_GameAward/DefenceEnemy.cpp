#include "DefenceEnemy.h"
#include "ObjectManager.h"

PrimitiveModel DefenceEnemy::modelData;
int DefenceEnemy::createCount;
const int DefenceEnemy::CREATE_NUMBER = 50;
HeapIndexManager DefenceEnemy::heapIndexManager(CREATE_NUMBER);

DefenceEnemy::DefenceEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

DefenceEnemy::~DefenceEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void DefenceEnemy::Initialize()
{
	hp = 2;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
}

void DefenceEnemy::Update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	setPosition(position);

}

void DefenceEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void DefenceEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void DefenceEnemy::LoadResource()
{
	//	std::string mtl;
	//
	//	modelData.key = "shotenemy";
	//	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 200,112,28,255 }, CREATE_NUMBER);
}

void DefenceEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void DefenceEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}