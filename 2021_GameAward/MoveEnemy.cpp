#include "MoveEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"
#include"LibMath.h"

int MoveEnemy::createCount;
const int MoveEnemy::CREATE_NUMBER = 30;
ObjModel MoveEnemy::modelData;
HeapIndexManager MoveEnemy::heapIndexManager(CREATE_NUMBER);

MoveEnemy::MoveEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

MoveEnemy::~MoveEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void MoveEnemy::Initialize()
{
	//setHeapNum();

	hp = 3;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale({ 0.5,0.5,0.5 }, heapNum);

	moveAnimationTimer.SetMaxTime(60 * 2);
	moveAnimationTimer.SetStopFlag(false);
}

void MoveEnemy::Update()
{
	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
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


	//プレイヤーの方を向かせる処理
	float angleY = LibMath::Vecto2ToAngle({ velocity.x,velocity.z }, true);
	modelData.SetAngle({ 0,-angleY,0 }, heapNum);

	//アニメーション

}

void MoveEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}


void MoveEnemy::LoadResource()
{

	//modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 180,153,108,255 }, CREATE_NUMBER);

	modelData.LoadModel
	(
		"Resources/Model/MoveEnemy/MoveEnemy.obj",
		true,
		CREATE_NUMBER,
		0
	);
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
