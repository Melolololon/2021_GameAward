#include "SimEnemy.h"
#include "ObjectManager.h"
#include "LibMath.h"
#include "Block.h"
#include "PlayerBullet.h"

PrimitiveModel SimEnemy::modelData;
int SimEnemy::createCount;
const int SimEnemy::CREATE_NUMBER = 50*3;

SimEnemy::SimEnemy()
{
	Initialize();
}

void SimEnemy::Initialize()
{
	setHeapNum();
	hp = 1;

	collisionFlag.sphere = true;

	sphereData.resize(4);

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
	sphereData[3].r = OBJSIZE / 2 * 1.5f;
}

void SimEnemy::Update()
{
	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//���K��
	velocity = Vector3Normalize(velocity);

	if (attackAfterTimer == 60 * 2)
	{
		//���W�X�V
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	else
	{
		attackAfterTimer--;
		if (attackAfterTimer < 0)
			attackAfterTimer = 60 * 2;
	}

	if (partDeadTimer < 60 * 0.5)
	{
		partDeadTimer--;
		if (partDeadTimer < 0)
		{
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
}

void SimEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	if (hp > 0)
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
	//	std::string mtl;
	//
	//	modelData.key = "Simenemy";
	//	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 161,203,116,255 }, CREATE_NUMBER);
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