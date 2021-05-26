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
	//�eID�p
	sphereData[0].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 0)) * mag, 0, std::sin(LibMath::AngleConversion(0, 0)) * mag);
	sphereData[0].r = OBJSIZE / 2;
	sphereData[1].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 120)) * mag, 0, std::sin(LibMath::AngleConversion(0, 120)) * mag);
	sphereData[1].r = OBJSIZE / 2;
	sphereData[2].position = position + Vector3(std::cos(LibMath::AngleConversion(0, 240)) * mag, 0, std::sin(LibMath::AngleConversion(0, 240)) * mag);
	sphereData[2].r = OBJSIZE / 2;
	//���S�@�ǔ����p
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

	

	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->GetHeadPosition().x - position.x, 0, pPlayer->GetHeadPosition().z - position.z };
	//���K��
	velocity = Vector3Normalize(velocity);

	//�v���C���[�̕����������鏈��
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
		//�S�������Ă�Ƃ������ړ�
		if (!GetDeadFlag()
			&& !other0->GetDeadFlag()
			&& !other1->GetDeadFlag()) 
		{
			//���W�X�V
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

	//�S����HP��0�őS������
	if (other0->hp <= 0 && other1->hp <= 0 && hp <= 0)
	{
		eraseManager = true;
		other0->eraseManager = true;
		other1->eraseManager = true;
	}

	//���ꂽ��A�j���[�V����
	if (hp <= 0)
	{
		DeadAnimation();
	}
	else
	{
		angle.z = 0.0f;
	}

	//�A�j���[�V����
	//�A�j���[�V�����X�V
	UpdateMoveAnimationData(velocity);

	//�{�[�����Z�b�g
	//�E�� 1 ���� 2
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
		//�u���b�N�Ƃ̏Փ˔���
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

		//�v���C���[�Ƃ̏Փ˔���
		if (typeid(*object) == typeid(Player))
		{			
			if (attackAfterTimer >= 60 * 2)
			{
				attackAfterTimer--;
				other0->attackAfterTimer--;
				other1->attackAfterTimer--;

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
				partDeadTimer--;

				//�����ɃX�R�A��^���鏈��

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

	//ID�ɂ���Ĉʒu��ς���
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

