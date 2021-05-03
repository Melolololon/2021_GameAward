#include "FleeEnemy.h"

PrimitiveModel FleeEnemy::modelData;
int FleeEnemy::createCount;
const int FleeEnemy::CREATE_NUMBER = 1;

FleeEnemy::FleeEnemy()
{
	Initialize();
}

void FleeEnemy::Initialize()
{
	setHeapNum();
	hp = 1;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

}

void FleeEnemy::Update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	//一定間隔以上なら座標更新
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		position = position + velocity * moveSpeed;
	}
	//距離が一定未満だったら停止・弾を撃つ
	else
	{
		//逃げる
		if (escapeTimer < 300 - 60 * 2)
		{
			position = position - velocity * moveSpeed * 2;
		}
		//とまる
		else
		{

		}

		escapeTimer--;

	}
	if (escapeTimer < 0) escapeTimer = 300;

	setPosition(position);

}

void FleeEnemy::Draw()
{
	/*Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);*/
	modelData.Draw(heapNum);

}

void FleeEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void FleeEnemy::LoadResource()
{
	std::string mtl;

	//modelData.key = "fleeenemy";
	//Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//Library::createHeapData2({ 220,144,201,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, { 220,144,201,255 }, CREATE_NUMBER);
}

void FleeEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void FleeEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}