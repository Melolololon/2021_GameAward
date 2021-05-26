#include "FleeEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "LibMath.h"
#include "PlayerBullet.h"
#include "Random.h"

ObjModel FleeEnemy::modelData;
int FleeEnemy::createCount;
const int FleeEnemy::CREATE_NUMBER = GENERATE_COUNT;
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
	moveSpeed = 0.15f;
	shiver = Vector3(0, 0, 0);

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.7f, heapNum);
}

void FleeEnemy::Update()
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
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		moveSpeed = 0.15f;
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	//距離が一定未満だったら
	else
	{
		
		//逃げる
		if (escapeTimer < 300 - 60 * 2)
		{
			if(escapeTimer == 300 - 60 * 2 - 1)
			{
				Library::PlaySoundEveryLoad("Resources/Sound/SE/FleeSE/EnemyRun.wav");
			}

			if (attackAfterTimer == 60 * 2)
			{
				//座標更新
				moveSpeed = 0.15f * -3;
				position = position + velocity * moveSpeed;
				setPosition(position);


				angle.y = -LibMath::Vecto2ToAngle({ -velocity.x,-velocity.z }, true);
			
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
			shiver = Vector3(Random::GetRandomNumberRangeSelectFloat(-20, 20) / 100, 0, Random::GetRandomNumberRangeSelectFloat(-20, 20) / 100);
			position += shiver;
			setPosition(position);
		}

		escapeTimer--;
		
	}
	if (escapeTimer < 0) escapeTimer = 300;


	//アニメーション
	//アニメーション更新
	UpdateMoveAnimationData(velocity);

	//ボーンをセット
	//右足 1 左足 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);


	modelData.SetAngle(angle, heapNum);
}

void FleeEnemy::Draw()
{
	/*Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);*/
	modelData.Draw(heapNum);

}

void FleeEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
	//ブロックとの衝突判定
	if (typeid(*object) == typeid(Block))
	{
		if (escapeTimer < 300 - 60 * 2 || escapeTimer == 300)
		{
			position -= velocity * moveSpeed;
		}
		else
		{
			position -= shiver;
		}

		setPosition(position);

	}

	//プレイヤーとの衝突判定
	if (typeid(*object) == typeid(Player))
	{
		if (attackAfterTimer >= 60 * 2)
		{
			attackAfterTimer--;
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
			//ここにスコアを与える処理

			//eraseManager = true;
		}
		if (hp == 0)
		{
			Library::PlaySoundEveryLoad("Resources/Sound/SE/CommonSE/EnemyLostSE.wav");
		}
	}
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