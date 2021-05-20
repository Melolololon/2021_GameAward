#include "FleeEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"
#include"LibMath.h"
#include "Random.h"

ObjModel FleeEnemy::modelData;
int FleeEnemy::createCount;
const int FleeEnemy::CREATE_NUMBER = 50;
HeapIndexManager FleeEnemy::heapIndexManager(CREATE_NUMBER);

FleeEnemy::FleeEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

FleeEnemy::~FleeEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void FleeEnemy::Initialize()
{
	hp = 1;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.5f, heapNum);
}

void FleeEnemy::Update()
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


	//プレイヤーの方を向かせる処理
	float angleY = LibMath::Vecto2ToAngle({ velocity.x,velocity.z }, true);
	modelData.SetAngle({ 0,-angleY,0 }, heapNum);

	//一定間隔以上なら座標更新
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	//距離が一定未満だったら
	else
	{
		
		//逃げる
		if (escapeTimer < 300 - 60 * 2)
		{
			if (attackAfterTimer == 60 * 2)
			{
				//座標更新
				position = position - velocity * moveSpeed * 3;
				setPosition(position);

				angleY = LibMath::Vecto2ToAngle({ -velocity.x,-velocity.z }, true);
				modelData.SetAngle({ 0,-angleY,0 }, heapNum);
			}
			else
			{
				attackAfterTimer--;
				if (attackAfterTimer < 0)
					attackAfterTimer = 60 * 2;
			}
		}
		//とまる
		else
		{
			velocity = 0;
			position = position + Vector3(Random::GetRandomNumberRangeSelectFloat(-20, 20) / 100 , 0, Random::GetRandomNumberRangeSelectFloat(-20, 20) / 100);
			setPosition(position);
		}

		escapeTimer--;
		
	}
	if (escapeTimer < 0) escapeTimer = 300;



	//アニメーション
	//アニメーション更新
	UpdateAnimationData(velocity);

	//ボーンをセット
	//右足 1 左足 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);
}

void FleeEnemy::Draw()
{
	/*Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);*/
	modelData.Draw(heapNum);

}

void FleeEnemy::LoadResource()
{
	//modelData.CreateBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, { 220,144,201,255 }, CREATE_NUMBER);
	modelData.LoadModel
	(
		"Resources/Model/FleeEnemy/FleeEnemy_Bone.obj",
		true,
		CREATE_NUMBER,
		0
	);
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