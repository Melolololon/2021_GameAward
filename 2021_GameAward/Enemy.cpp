#include "Enemy.h"
#include "Block.h"
#include "PlayerBullet.h"
#include "SceneManager.h"
#include "Play.h"

const float Enemy::OBJSIZE = 1.0f;
const float Enemy::FREAM_MOVE_ANGLE = 3.0f;
Enemy::Enemy()
{
	leftFootAngle.z = FREAM_MOVE_ANGLE;
	rightFootAngle.z = -FREAM_MOVE_ANGLE;
	moveAnimationTime.SetMaxTime(MOVE_ANIMATION_MAX);
	moveAnimationTime.SetStopFlag(false);
}

void Enemy::setHeapNum()
{
}

void Enemy::UpdateAnimationData()
{
	if (hp <= 0)return;

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
	
	const float MAX_ANGLE = FREAM_MOVE_ANGLE * (MOVE_ANIMATION_MAX / 2);
	const float MIN_ANGLE = -FREAM_MOVE_ANGLE * (MOVE_ANIMATION_MAX / 2);
	leftFootAngle.z = leftFootAngle.z > MAX_ANGLE ? MAX_ANGLE : leftFootAngle.z;
	leftFootAngle.z = leftFootAngle.z < MIN_ANGLE ? MIN_ANGLE : leftFootAngle.z;
	rightFootAngle.z = rightFootAngle.z > MAX_ANGLE ? MAX_ANGLE : rightFootAngle.z;
	rightFootAngle.z = rightFootAngle.z < MIN_ANGLE ? MIN_ANGLE : rightFootAngle.z;

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
	//�u���b�N�Ƃ̏Փ˔���
	if (typeid(*object) == typeid(Block))
	{
		position -= velocity * moveSpeed;

		setPosition(position);

	}

	//�v���C���[�Ƃ̏Փ˔���
	if (typeid(*object) == typeid(Player))
	{
		if (attackAfterTimer >= 60 * 2)
		{
			attackAfterTimer--;
			//�����Ƀv���C���[�̗̑͂����炷����
			pPlayer->DamageFromEnemy();
		}
	}

	//�v���C���[�̒e�Ƃ̏Փ˔���
	if (typeid(*object) == typeid(PlayerBullet))
	{
		hp--;
		if (hp <= 0)
		{
			//�����ɃX�R�A��^���鏈��

			eraseManager = true;
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
	//�����O�͓����Ȃ��悤��
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


