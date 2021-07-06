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

	//�u���b�N�Ƃ̏Փ˔���
	if (typeid(*object) == typeid(Block)
		|| typeid(*object) == typeid(TargetObject))
	{
		switch (sphereData[arrayNum].boxHitDistance)
		{
		case BOX_HIT_DIRECTION_RIGHT:
			//�����ɓ��荞�ނ̖h�~����if
			//velocity���Œ��float�ɂ���΂߂荞�܂Ȃ����A�J�����������܂���̂ł������Ă�
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
			//�����ɓ��荞�ނ̖h�~����if
			//velocity���Œ��float�ɂ���΂߂荞�܂Ȃ����A�J�����������܂���̂ł������Ă�
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
		flashingTimer.SetStopFlag(false);
		flashingTimer.ResetTime();

		if (hp <= 0)
		{
			//�����ɃX�R�A��^���鏈��

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


void Enemy::FlashingTimerMaxCheck()
{
	if(flashingTimer.GetSameAsMaximumFlag())
	{
		flashingTimer.ResetTime();
		flashingTimer.SetStopFlag(true);
	}
}
