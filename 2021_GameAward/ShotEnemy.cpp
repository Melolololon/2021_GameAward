#include "ShotEnemy.h"
void ShotEnemy::initialize()
{
	setHeapNum();
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
		shotWaitTimer--;
	}
	if (shotWaitTimer < 0) shotWaitTimer = 60;
	setPosition(position);

}

void ShotEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "shotenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
}
