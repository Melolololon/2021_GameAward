#include "FleeEnemy.h"

ModelData FleeEnemy::modelData;
int FleeEnemy::createCount;
const int FleeEnemy::CREATE_NUMBER = 1;

FleeEnemy::FleeEnemy()
{
	initialize();
}

void FleeEnemy::initialize()
{
	setHeapNum();
	hp = 1;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

}

void FleeEnemy::update()
{
	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//���K��
	velocity = vector3Normalize(velocity);

	//���Ԋu�ȏ�Ȃ���W�X�V
	if (sqrt((pPlayer->getHeadPosition().x - position.x) * (pPlayer->getHeadPosition().x - position.x) +
		(pPlayer->getHeadPosition().y - position.y) * (pPlayer->getHeadPosition().y - position.y) +
		(pPlayer->getHeadPosition().z - position.z) * (pPlayer->getHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		position = position + velocity * moveSpeed;
	}
	//��������薢�����������~�E�e������
	else
	{
		//������
		if (escapeTimer < 300 - 60 * 2)
		{
			position = position - velocity * moveSpeed * 2;
		}
		//�Ƃ܂�
		else
		{

		}

		escapeTimer--;

	}
	if (escapeTimer < 0) escapeTimer = 300;

	setPosition(position);

}

void FleeEnemy::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);

}

void FleeEnemy::hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void FleeEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "fleeenemy";
	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	Library::createHeapData2({ 220,144,201,255 }, CREATE_NUMBER, modelData);
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
	Library::setPosition(position, modelData, heapNum);
}