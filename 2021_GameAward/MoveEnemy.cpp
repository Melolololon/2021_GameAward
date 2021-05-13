#include "MoveEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"

PrimitiveModel MoveEnemy::modelData;
int MoveEnemy::createCount;
const int MoveEnemy::CREATE_NUMBER = 50;

MoveEnemy::MoveEnemy()
{
	Initialize();
}

void MoveEnemy::Initialize()
{
	setHeapNum();
	hp = 3;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
	
}

void MoveEnemy::Update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = {pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	if (attackAfterTimer == 60 * 2)
	{
		//座標更新
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	else
	{
		attackAfterTimer--;
		if (attackAfterTimer < 0)
			attackAfterTimer = 60 * 2;
	}

}

void MoveEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}


void MoveEnemy::LoadResource()
{
	//std::string mtl;

	//modelData.key = "moveenemy";
	//Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//Library::createHeapData2({ 180,153,108,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 180,153,108,255 }, CREATE_NUMBER);
}

void MoveEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void MoveEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}
