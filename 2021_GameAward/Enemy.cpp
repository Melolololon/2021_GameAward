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

	//�Î~���Ă�Ƃ��ɓ����n�߂���MAX_MIN�ŏ�����
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

	//�����ĂȂ�������Î~
	if (vel.x == 0
		&& vel.y == 0
		&& vel.z == 0)
	{
		//�_�����ɂ����邽��0���
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

			Library::PlaySoundEveryLoad("Resources/Sound/SE/MoveSE/EnemyAttack.wav");
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


