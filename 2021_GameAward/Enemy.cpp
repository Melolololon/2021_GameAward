#include "Enemy.h"
#include "Block.h"
#include "PlayerBullet.h"
#include "SceneManager.h"
#include "Play.h"

#include"LibMath.h"

const float Enemy::OBJSIZE = 1.0f;
const float Enemy::FREAM_MOVE_ANGLE = 6.0f;
const float Enemy::MAX_ANGLE = FREAM_MOVE_ANGLE / 2 * (MOVE_ANIMATION_MAX / 2);
const float Enemy::MIN_ANGLE = -FREAM_MOVE_ANGLE / 2 * (MOVE_ANIMATION_MAX / 2);

Enemy::Enemy()
{
	leftFootAngle.z = MAX_ANGLE;
	rightFootAngle.z = -MIN_ANGLE;
	moveAnimationTime.SetMaxTime(MOVE_ANIMATION_MAX);
	moveAnimationTime.SetStopFlag(false);
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
	if (typeid(*object) == typeid(Block))
	{
		position -= velocity * moveSpeed;

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
		if (hp <= 0)
		{
			//ここにスコアを与える処理

			//eraseManager = true;
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


