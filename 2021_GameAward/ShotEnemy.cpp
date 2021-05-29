#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"

#include"LibMath.h"

ObjModel ShotEnemy::modelData;
int ShotEnemy::createCount;
const int ShotEnemy::CREATE_NUMBER = GENERATE_COUNT;
HeapIndexManager ShotEnemy::heapIndexManager(CREATE_NUMBER);


ShotEnemy::ShotEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

ShotEnemy::~ShotEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void ShotEnemy::Initialize()
{
	hp = 4;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.8f, heapNum);


	modelData.SetBoneAngle(0, 0, heapNum);
	modelData.SetBoneAngle(0, 1, heapNum);

	modelData.SetAngle(0, heapNum);

	////プレイヤーへの方向ベクトルを求める
	//velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//LockPlayer();
}

void ShotEnemy::Update()
{
	if (heapNum == -1)
	{
		eraseManager = true;
		return;
	}

	if (isGameStart() == false)
	{
		setPosition(position);

		////プレイヤーへの方向ベクトルを求める
		velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
		LockPlayer();
		velocity = 0;
		modelData.SetAngle(angle, heapNum);
		return;
	}

	//やられたらアニメーション&return
	if (hp <= 0)
	{
		UpdateDeadAnimationData();
		modelData.SetAngle(angle, heapNum);
		return;
	}

	//プレイヤーへの方向ベクトルを求める
	velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//正規化
	velocity = Vector3Normalize(velocity);

	//プレイヤーの方を向かせる処理
	LockPlayer();


	//一定間隔以上なら座標更新
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		if (attackAfterTimer == 60 * 2)
		{
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
	//距離が一定未満だったら停止・弾を撃つ
	else
	{
		//ここに弾を撃つ処理
		if (shotWaitTimer == 60) 
		{
			ObjectManager::GetInstance()->AddObject(std::make_shared<EnemyBullet>(position, velocity));
			Library::PlaySoundEveryLoad("Resources/Sound/SE/ShotSE/EnemyShot.wav");
		}
		shotWaitTimer--;

		if (attackAfterTimer != 60 * 2)
		{
			attackAfterTimer--;
			if (attackAfterTimer < 0)
				attackAfterTimer = 60 * 2;
		}

		//アニメーションのために0代入
		velocity = 0;

	}

	if (shotWaitTimer < 0) shotWaitTimer = 60;
	

	//アニメーション
	//アニメーション更新
	UpdateMoveAnimationData(velocity);

	//ボーンをセット
	//右足 1 左足 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);


	modelData.SetAngle(angle, heapNum);
}

void ShotEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void ShotEnemy::LoadResource()
{
	//modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 200,112,28,255 }, CREATE_NUMBER);

	modelData.LoadModel
	(
		"Resources/Model/ShotEnemy/ShotEnemy_Bone.obj",
		true,
		CREATE_NUMBER,
		0
	);
}

void ShotEnemy::setHeapNum()
{
	heapNum = createCount; 
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void ShotEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}