#include "MoveEnemy.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"
#include"LibMath.h"

int MoveEnemy::createCount;
const int MoveEnemy::CREATE_NUMBER = GENERATE_COUNT;
ObjModel MoveEnemy::modelData;
HeapIndexManager MoveEnemy::heapIndexManager(CREATE_NUMBER);

MoveEnemy::MoveEnemy()
{
	heapNum = heapIndexManager.GetHeapIndex();
	collisionFlag.sphere = true;
	sphereData.resize(1);
}

MoveEnemy::~MoveEnemy()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}

void MoveEnemy::Initialize()
{
	//setHeapNum();

	hp = 3;

	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;

	modelData.SetScale(0.7f, heapNum);

}

void MoveEnemy::Update()
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

		//�A�j���[�V�����p
		velocity = 0;
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

void MoveEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}


void MoveEnemy::LoadResource()
{

	//modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 180,153,108,255 }, CREATE_NUMBER);

	modelData.LoadModel
	(
		"Resources/Model/MoveEnemy/MoveEnemy_Bone.obj",
		true,
		CREATE_NUMBER,
		0
	);
}

void MoveEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void MoveEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}
