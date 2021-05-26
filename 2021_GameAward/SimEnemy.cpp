#include "SimEnemy.h"
#include "ObjectManager.h"
#include "LibMath.h"
#include "Block.h"
#include "PlayerBullet.h"

ObjModel SimEnemy::modelData;
int SimEnemy::createCount;
const int SimEnemy::CREATE_NUMBER = GENERATE_COUNT * 3;
HeapIndexManager SimEnemy::heapIndexManager(CREATE_NUMBER);

SimEnemy::SimEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(4);
}

SimEnemy::~SimEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void SimEnemy::Initialize()
{
	hp = 1;


	float mag = 1.5f;
	//各ID用
	sphereData[0].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 0)) * mag, 0, std::sin(LibMath::AngleConversion(0, 0)) * mag);
	sphereData[0].r = OBJSIZE / 2;
	sphereData[1].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 120)) * mag, 0, std::sin(LibMath::AngleConversion(0, 120)) * mag);
	sphereData[1].r = OBJSIZE / 2;
	sphereData[2].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 240)) * mag, 0, std::sin(LibMath::AngleConversion(0, 240)) * mag);
	sphereData[2].r = OBJSIZE / 2;
	//中心　壁避け用
	sphereData[3].position = position;
	sphereData[3].r = OBJSIZE / 2 * 2.0f;

	setPosition(position);


	modelData.SetScale(0.7f, heapNum);
}

void SimEnemy::Update()
{
	if (heapNum == -1)
	{
		eraseManager = true;
		return;
	}

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
	if (id == 0)
	{
		angle.y = -LibMath::Vecto2ToAngle({ 1,0 }, true);
	
	}
	else if (id == 1)
	{
		angle.y = -LibMath::Vecto2ToAngle({ -0.5,0.5 }, true);
		
	}
	else
	{
		angle.y = -LibMath::Vecto2ToAngle({ 0,-1 }, true);
	
	}


	if (attackAfterTimer == 60 * 2)
	{
		//全員生きてるときだけ移動
		if (!GetDeadFlag()
			&& !other0->GetDeadFlag()
			&& !other1->GetDeadFlag()) 
		{
			//座標更新
			position = position + velocity * moveSpeed;
			setPosition(position);
		}
	}
	else
	{
		attackAfterTimer--;
		if (attackAfterTimer < 0)
			attackAfterTimer = 60 * 2;


		velocity = 0;
	}

	if (partDeadTimer < 60 * 0.5)
	{
		partDeadTimer--;
		if (partDeadTimer < 0)
		{
			Library::PlaySoundEveryLoad("Resources/Sound/SE/SimSE/EnemyDrop.wav");
			partDeadTimer = 60 * 0.5;
			hp = 1;
		}
	}

	//全員のHPが0で全員死ぬ
	if (other0->hp <= 0 && other1->hp <= 0 && hp <= 0)
	{
		eraseManager = true;
		other0->eraseManager = true;
		other1->eraseManager = true;
	}

	//やられたらアニメーション
	if (hp <= 0)
	{
		DeadAnimation();
	}
	else
	{
		angle.z = 0.0f;
	}

	//アニメーション
	//アニメーション更新
	UpdateMoveAnimationData(velocity);

	//ボーンをセット
	//右足 1 左足 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);

	modelData.SetAngle(angle, heapNum);
}

void SimEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	//if (hp > 0)
		modelData.Draw(heapNum);

}

void SimEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
	if (arrayNum == 3)
	{
		//ブロックとの衝突判定
		if (typeid(*object) == typeid(Block))
		{
			//personalPos -= velocity * moveSpeed;

			switch (sphereData[arrayNum].boxHitDistance)
			{
			case BOX_HIT_DIRECTION_RIGHT:
				position.x += OBJSIZE * moveSpeed;
				setPosition(position);
				break;
			case BOX_HIT_DIRECTION_LEFT:
				position.x -= OBJSIZE * moveSpeed;
				setPosition(position);
				break;
			case BOX_HIT_DIRECTION_FRONT:
				position.z -= OBJSIZE * moveSpeed;
				setPosition(position);
				break;
			case BOX_HIT_DIRECTION_BACK:
				position.z += OBJSIZE * moveSpeed;
				setPosition(position);
				break;
			}
		}
	}
	else
	{

		if (hp <= 0)
			return;

		//プレイヤーとの衝突判定
		if (typeid(*object) == typeid(Player))
		{			
			if (attackAfterTimer >= 60 * 2)
			{
				attackAfterTimer--;
				other0->attackAfterTimer--;
				other1->attackAfterTimer--;

				//ここにプレイヤーの体力を減らす処理
				pPlayer->DamageFromEnemy();
			}
		}

		//プレイヤーの弾との衝突判定
		if (typeid(*object) == typeid(PlayerBullet))
		{
			hp--;
			if (hp <= 0)
			{
				partDeadTimer--;

				//ここにスコアを与える処理

				//eraseManager = true;
			}
			if (hp == 0)
			{
				Library::PlaySoundEveryLoad("Resources/Sound/SE/CommonSE/EnemyLostSE.wav");
			}
		}
	}

}

void SimEnemy::SetOther(int num, std::shared_ptr<SimEnemy> p)
{
	if (num != 0 && num != 1)
		return;

	if (num == 0)
	{
		other0 = p;
	}
	else if (num == 1)
	{
		other1 = p;
	}
}

void SimEnemy::LoadResource()
{
	
	//modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 161,203,116,255 }, CREATE_NUMBER);
	modelData.LoadModel
	(
		"Resources/Model/SimEnemy/SimEnemy_Bone.obj",
		true,
		CREATE_NUMBER,
		0
	);
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

	float mag = 1.5f;

	//IDによって位置を変える
	if (id == 0)
	{
		personalPos = position + Vector3(std::cos(LibMath::AngleConversion(0, 0)) * mag, 0, std::sin(LibMath::AngleConversion(0, 0)) * mag);
		sphereData[0].position = personalPos;
		modelData.SetPosition(personalPos, heapNum);
	}
	else if (id == 1)
	{
		personalPos = position + Vector3(std::cos(LibMath::AngleConversion(0, 120)) * mag, 0, std::sin(LibMath::AngleConversion(0, 120)) * mag);
		sphereData[1].position = personalPos;
		modelData.SetPosition(personalPos, heapNum);
	}
	else
	{
		personalPos = position + Vector3(std::cos(LibMath::AngleConversion(0, 240)) * mag, 0, std::sin(LibMath::AngleConversion(0, 240)) * mag);
		sphereData[2].position = personalPos;
		modelData.SetPosition(personalPos, heapNum);
	}

	sphereData[3].position = position;

	//Library::setPosition(position, modelData, heapNum);
}


void SimEnemy::DeadAnimation()
{
	if (angle.z < 90)
	{
		angle.z += 2.0f;
	}
	else
		partDeadTimer--;
}


bool SimEnemy::GetDeadFlag()
{
	if (hp <= 0)
		return true;
	return false;
}

