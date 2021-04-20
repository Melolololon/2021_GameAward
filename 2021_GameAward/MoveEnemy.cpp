#include "MoveEnemy.h"

void MoveEnemy::initialize()
{
	setHeapNum();
	hp = 3;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.5f;
	
}

void MoveEnemy::update()
{
	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = {pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//���K��
	velocity = vector3Normalize(velocity);

	//���W�X�V
	position = position + velocity * moveSpeed;
	setPosition(position);
}

void MoveEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "moveenemy";
	Library::create3DBox(Vector3{ 1,1,1 }, modelData);
	Library::createHeapData2({ 180,153,108,255 }, CREATE_NUMBER, modelData);
}

