#include "FleeEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "LibMath.h"
#include "PlayerBullet.h"
#include"DecrementTimeNumber.h"
#include"TargetObject.h"

#include"ObjectManager.h"
#include"Random.h"

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
	hp = 3;
	moveSpeed = 0.15f;
	shiver = Vector3(0, 0, 0);

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.8f, heapNum);


	modelData.SetBoneAngle(0, 0, heapNum);
	modelData.SetBoneAngle(0, 1, heapNum);
	modelData.SetAngle(0, heapNum);

	////�v���C���[�ւ̕����x�N�g�������߂�
	//velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//LockPlayer();
}

void FleeEnemy::Update()
{
	CalcToPlayerRouteVectors();

	if (heapNum == -1)
	{
		eraseManager = true;
		return;
	}

	if (isGameStart() == false)
	{
		setPosition(position);

		////�v���C���[�ւ̕����x�N�g�������߂�
		velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
		LockPlayer();
		velocity = 0;
		modelData.SetAngle(angle, heapNum);
		return;
	}

	//���ꂽ��A�j���[�V����&return
	if (hp <= 0)
	{
		UpdateDeadAnimationData();
		modelData.SetAngle(angle, heapNum);
		return;
	}

	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//���K��
	velocity = Vector3Normalize(velocity);


	//�v���C���[�̕����������鏈��
	LockPlayer();

	//���Ԋu�ȏ�Ȃ���W�X�V
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		moveSpeed = 0.15f;
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	//��������薢����������
	else
	{
		
		//������
		if (escapeTimer < 300 - 60 * 2)
		{
			if(escapeTimer == 300 - 60 * 2 - 1
				&& attackAfterTimer == 60 * 2)
			{
				Library::PlaySoundEveryLoad("Resources/Sound/SE/FleeSE/EnemyRun.wav");
			}

			if (attackAfterTimer == 60 * 2)
			{
				//���W�X�V
				velocity *= -1;
				moveSpeed = 0.15f * 3;
				position = position + velocity * moveSpeed;
				setPosition(position);


				angle.y = -LibMath::Vecto2ToAngle({ velocity.x,velocity.z }, true);
			
			}
			else
			{
				attackAfterTimer--;
				if (attackAfterTimer < 0)
					attackAfterTimer = 60 * 2;
			}
		}
		//�Ƃ܂�
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


	//�A�j���[�V����
	//�A�j���[�V�����X�V
	UpdateMoveAnimationData(velocity);

	//�{�[�����Z�b�g
	//�E�� 1 ���� 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);


	modelData.SetAngle(angle, heapNum);


	//�_��
	FlashingTimerMaxCheck();
}

void FleeEnemy::Draw()
{
	/*Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);*/
	if (flashingTimer.GetMultipleTimeFlag(2) 
		|| hp <= 0)
	{
		modelData.Draw(heapNum);
	}

}

void FleeEnemy::Hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
	//�u���b�N�Ƃ̏Փ˔���
	if (typeid(*object) == typeid(Block)
		|| typeid(*object) == typeid(TargetObject))
	{
		if (escapeTimer < 300 - 60 * 2 || escapeTimer == 300)
		{
			switch (sphereData[arrayNum].boxHitDistance)
			{
			case BOX_HIT_DIRECTION_RIGHT:
				
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
		}
		else
		{
			position -= shiver;
		}

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
		flashingTimer.SetStopFlag(false);
		flashingTimer.ResetTime();

		if (hp <= 0)
		{
			//�����ɃX�R�A��^���鏈��

			//eraseManager = true;
		}
		if (hp == 0)
		{
			/*Vector3 numPos = position + Vector3(0, 3, 0);
			ObjectManager::GetInstance()->AddObject(std::make_shared<DecrementTimeNumber>(numPos, decreaseTime));*/
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