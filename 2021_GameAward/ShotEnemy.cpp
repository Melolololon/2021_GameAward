#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"

#include"LibMath.h"

ObjModel ShotEnemy::modelData;
int ShotEnemy::createCount;
const int ShotEnemy::CREATE_NUMBER = GENERATE_COUNT;
HeapIndexManager ShotEnemy::heapIndexManager(CREATE_NUMBER);


ShotEnemy::ShotEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

ShotEnemy::~ShotEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void ShotEnemy::Initialize()
{
	hp = 2;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.5f, heapNum);
}

void ShotEnemy::Update()
{
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
	float angleY = LibMath::Vecto2ToAngle({ velocity.x,velocity.z }, true);
	modelData.SetAngle({ 0,-angleY,0 }, heapNum);

	//���Ԋu�ȏ�Ȃ���W�X�V
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		if (attackAfterTimer == 60 * 2)
		{
			position = position + velocity * moveSpeed;
			setPosition(position);
		}
		else
		{
			attackAfterTimer--;
			if (attackAfterTimer < 0)
				attackAfterTimer = 60 * 2;
		}
	}
	//��������薢�����������~�E�e������
	else
	{
		//�����ɒe��������
		if (shotWaitTimer == 60)
			ObjectManager::GetInstance()->AddObject(std::make_shared<EnemyBullet>(position, velocity));

		shotWaitTimer--;

		if (attackAfterTimer != 60 * 2)
		{
			attackAfterTimer--;
			if (attackAfterTimer < 0)
				attackAfterTimer = 60 * 2;
		}

		//�A�j���[�V�����̂��߂�0���
		velocity = 0;

	}

	if (shotWaitTimer < 0) shotWaitTimer = 60;
	


	//�A�j���[�V����
	//�A�j���[�V�����X�V
	UpdateAnimationData(velocity);

	//�{�[�����Z�b�g
	//�E�� 1 ���� 2
	modelData.SetBoneAngle(rightFootAngle, 0, heapNum);
	modelData.SetBoneAngle(leftFootAngle, 1, heapNum);
}

void ShotEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void ShotEnemy::LoadResource()
{
	//modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 200,112,28,255 }, CREATE_NUMBER);

	modelData.LoadModel
	(
		"Resources/Model/ShotEnemy/ShotEnemy_Bone.obj",
		true,
		CREATE_NUMBER,
		0
	);
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
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}