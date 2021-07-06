#include "Enemy.h"
#include "Block.h"
#include "PlayerBullet.h"
#include "SceneManager.h"
#include "Play.h"

#include"ObjectManager.h"
#include"LibMath.h"

#include"DecrementTimeNumber.h"
#include"EnemyParticle.h"
#include"TargetObject.h"

const float Enemy::OBJSIZE = 1.0f;
const float Enemy::FREAM_MOVE_ANGLE = 6.0f;
const float Enemy::MAX_ANGLE = FREAM_MOVE_ANGLE / 2 * (MOVE_ANIMATION_MAX / 2);
const float Enemy::MIN_ANGLE = -FREAM_MOVE_ANGLE / 2 * (MOVE_ANIMATION_MAX / 2);

void Enemy::CalcToPlayerRouteVectors()
{
	std::vector<std::vector<AStarNode>>& nodes = Play::GetAStarNodes();

	if (nodes.size() == 0)return;

	Vector2 myPos = Vector2(position.x, position.z);
	Vector2 playerPos = Vector2(pPlayer->GetHeadPosition().x, pPlayer->GetHeadPosition().z);
	LibMath::GetAStarCalcResult
	(
		myPos,
		playerPos,
		nodes,
		toPlayerRouteVectors
	);
}

Enemy::Enemy()
{
	leftFootAngle.z = MAX_ANGLE;
	rightFootAngle.z = -MIN_ANGLE;
	moveAnimationTime.SetMaxTime(MOVE_ANIMATION_MAX);
	moveAnimationTime.SetStopFlag(false);

	flashingTimer.SetMaxTime(FLASHING_TIME);
}

void Enemy::setHeapNum()
{
}

void Enemy::UpdateMoveAnimationData(const Vector3& vel)
{
	if (hp <= 0)return;

	//静止してるときに動き始めたらMAX_MINで初期化
	if(!isMoveAnimation
		&& vel.x != 0
		&& vel.y != 0
		&& vel.z != 0)
	{
		leftFootAngle.z = MAX_ANGLE;
		rightFootAngle.z = MIN_ANGLE;
	}


	if (moveAnimationTime.GetTime() < MOVE_ANIMATION_MAX / 2)
	{
		leftFootAngle.z -= FREAM_MOVE_ANGLE;
		rightFootAngle.z += FREAM_MOVE_ANGLE;

	}
	else
	{
		leftFootAngle.z += FREAM_MOVE_ANGLE;
		rightFootAngle.z -= FREAM_MOVE_ANGLE;
	}

	//動いてなかったら静止
	if (vel.x == 0
		&& vel.y == 0
		&& vel.z == 0)
	{
		//棒立ちにさせるため0代入
		leftFootAngle.z = 0;
		rightFootAngle.z = 0;

		moveAnimationTime.ResetTime();
		isMoveAnimation = false;
	}
	else
		isMoveAnimation = true;
	
	leftFootAngle.z = leftFootAngle.z > MAX_ANGLE ? MAX_ANGLE : leftFootAngle.z;
	leftFootAngle.z = leftFootAngle.z < MIN_ANGLE ? MIN_ANGLE : leftFootAngle.z;
	rightFootAngle.z = rightFootAngle.z > MAX_ANGLE ? MAX_ANGLE : rightFootAngle.z;
	rightFootAngle.z = rightFootAngle.z < MIN_ANGLE ? MIN_ANGLE : rightFootAngle.z;

}

void Enemy::UpdateDeadAnimationData()
{
	if (angle.z < 90)
	{
		angle.z += 2.0f;
		position.y -= 0.025f;
	}
	else
	{
		pGameTime->SetNowTime(pGameTime->GetTime() - decreaseTime * 60);
		if (pGameTime->GetTime() < 0) pGameTime->SetNowTime(0);
		eraseManager = true;

		Vector3 parPos = position + Vector3(0, 0, -1);
		ObjectManager::GetInstance()->AddObject(std::make_shared<EnemyParticle>(parPos));

		Vector3 numPos = position + Vector3(0, 3, 0);
		ObjectManager::GetInstance()->AddObject(std::make_shared<DecrementTimeNumber>(numPos, decreaseTime));
	}

}

void Enemy::LockPlayer()
{
	angle.y = -LibMath::Vecto2ToAngle({ velocity.x,velocity.z }, true);
}

void Enemy::Initialize()
{
	setHeapNum();
}

void Enemy::Update()
{

}

void Enemy::Draw()
{
}

void Enemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{

	//ブロックとの衝突判定
	if (typeid(*object) == typeid(Block)
		|| typeid(*object) == typeid(TargetObject))
	{
		switch (sphereData[arrayNum].boxHitDistance)
		{
		case BOX_HIT_DIRECTION_RIGHT:
			//内側に入り込むの防止するif
			//velocityを固定のfloatにすればめり込まないが、カメラが動きまくるのでこうしてる
			if (velocity.x >= 0)
				position.x += 1.0f * moveSpeed;
			else
				position.x += -velocity.x * moveSpeed;


			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (velocity.x <= 0)
				position.x += -1.0f * moveSpeed;
			else
				position.x += -velocity.x * moveSpeed;


			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (velocity.z <= 0)
				position.z += -1.0f * moveSpeed;
			else
				position.z += -velocity.z * moveSpeed;

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (velocity.z >= 0)
				position.z += 1.0f * moveSpeed;
			else
				position.z += -velocity.z * moveSpeed;

			break;
		}


		switch (sphereData[arrayNum].boxHitDistance)
		{
		case BOX_HIT_DIRECTION_RIGHT:
			//内側に入り込むの防止するif
			//velocityを固定のfloatにすればめり込まないが、カメラが動きまくるのでこうしてる
			if (velocity.x >= 0)
				position.x += velocity.x * moveSpeed;
			else
				position.x += -velocity.x * moveSpeed;


			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (velocity.x <= 0)
				position.x += velocity.x * moveSpeed;
			else
				position.x += -velocity.x * moveSpeed;


			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (velocity.z <= 0)
				position.z += velocity.z * moveSpeed;
			else
				position.z += -velocity.z * moveSpeed;

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (velocity.z >= 0)
				position.z += velocity.z * moveSpeed;
			else
				position.z += -velocity.z * moveSpeed;

			break;
		}
		//position -= velocity * moveSpeed;

		setPosition(position);

	}

	//プレイヤーとの衝突判定
	if (typeid(*object) == typeid(Player))
	{
		if (attackAfterTimer >= 60 * 2)
		{

			Library::PlaySoundEveryLoad("Resources/Sound/SE/MoveSE/EnemyAttack.wav");
			attackAfterTimer--;
			//ここにプレイヤーの体力を減らす処理
			pPlayer->DamageFromEnemy();
		}
	}

	//プレイヤーの弾との衝突判定
	if (typeid(*object) == typeid(PlayerBullet))
	{
		hp--;
		flashingTimer.SetStopFlag(false);
		flashingTimer.ResetTime();

		if (hp <= 0)
		{
			//ここにスコアを与える処理

			//eraseManager = true;
		}
		if(hp == 0)
		{

			Library::PlaySoundEveryLoad("Resources/Sound/SE/CommonSE/EnemyLostSE.wav");
		}
	}
}

void Enemy::loadModel()
{
}

void Enemy::setPosition(Vector3 pos)
{
}

bool Enemy::isGameStart()
{
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	//準備前は動かないように
	if (typeid(*currentScene) == typeid(Play))
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET ||
			Play::GetPlaySceneState() == Play::PLAY_SCENE_START_PREVIOUS)
		{
			return false;
		}
	}

	return true;
}


void Enemy::FlashingTimerMaxCheck()
{
	if(flashingTimer.GetSameAsMaximumFlag())
	{
		flashingTimer.ResetTime();
		flashingTimer.SetStopFlag(true);
	}
}
