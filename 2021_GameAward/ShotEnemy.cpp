#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"

ModelData ShotEnemy::modelData;
int ShotEnemy::createCount;
const int ShotEnemy::CREATE_NUMBER = 1;

ShotEnemy::ShotEnemy()
{
	initialize();
}

void ShotEnemy::initialize()
{
	setHeapNum();
	hp = 2;
}

void ShotEnemy::update()
{
	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//���K��
	velocity = vector3Normalize(velocity);

	//���Ԋu�ȏ�Ȃ���W�X�V
	if (sqrt((pPlayer->getHeadPosition().x - position.x) * (pPlayer->getHeadPosition().x - position.x) +
		(pPlayer->getHeadPosition().y - position.y) * (pPlayer->getHeadPosition().y - position.y) +
		(pPlayer->getHeadPosition().z - position.z) * (pPlayer->getHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		position = position + velocity * moveSpeed;
	}
	//��������薢�����������~�E�e������
	else
	{
		//�����ɒe��������
		if (shotWaitTimer == 60)
			ObjectManager::getInstance()->addObject(std::make_shared<EnemyBullet>(position, velocity));

		shotWaitTimer--;

	}
	if (shotWaitTimer < 0) shotWaitTimer = 60;
	setPosition(position);

}

void ShotEnemy::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);

}

void ShotEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "shotenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
}

void ShotEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void ShotEnemy::setPosition(Vector3 pos)
{
	position = pos;
	Library::setPosition(position, modelData, heapNum);
}