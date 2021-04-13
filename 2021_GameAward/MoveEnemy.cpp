#include "MoveEnemy.h"

void MoveEnemy::initialize()
{
	setHeapNum();
}

void MoveEnemy::update()
{
	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = {pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//���K��
	velocity = vector3Normalize(velocity);

	//���W�X�V
	position = position + velocity * moveSpeed;
	Library::setPosition(position, modelData, heapNum);
}
