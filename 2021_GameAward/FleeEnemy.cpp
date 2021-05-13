#include "FleeEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"

PrimitiveModel FleeEnemy::modelData;
int FleeEnemy::createCount;
const int FleeEnemy::CREATE_NUMBER = 50;
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
	hp = 1;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

}

void FleeEnemy::Update()
{
	if (isGameStart() == false)
	{
		setPosition(position);
		return;
	}

	//�v���C���[�ւ̕����x�N�g�������߂�
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//���K��
	velocity = Vector3Normalize(velocity);

	//���Ԋu�ȏ�Ȃ���W�X�V
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && escapeTimer == 300)
	{
		position = position + velocity * moveSpeed;
		setPosition(position);
	}
	//��������薢����������
	else
	{
		
		//������
		if (escapeTimer < 300 - 60 * 2)
		{
			if (attackAfterTimer == 60 * 2)
			{
				//���W�X�V
				position = position - velocity * moveSpeed * 3;
				setPosition(position);
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

		}

		escapeTimer--;
		
	}
	if (escapeTimer < 0) escapeTimer = 300;


}

void FleeEnemy::Draw()
{
	/*Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);*/
	modelData.Draw(heapNum);

}

void FleeEnemy::LoadResource()
{
	std::string mtl;

	//modelData.key = "fleeenemy";
	//Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	//Library::createHeapData2({ 220,144,201,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, { 220,144,201,255 }, CREATE_NUMBER);
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