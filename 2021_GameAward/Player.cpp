#include "Player.h"

#include<fstream>

//�t�@�C������ǂ݂Ƃ���static�ɓ�����邩�m���߂�

ModelData Player::modelData;
int Player::createCount;
const int Player::CREATE_NUMBER = 1;
std::vector<Vector3> Player::initialBonePos;

Player::Player()
{
}

Player::~Player()
{
}

void Player::loadModel()
{
	std::string mtl;

	modelData.key = "player";
	Library::loadOBJVertex("Resources/Model/testSnake.obj", true, true, &mtl, modelData);
	Library::loadOBJMaterial("Resources/Model/", mtl, 1, modelData);
	initialBonePos = Library::getBonePosition(modelData);
}


void Player::loadParam()
{
	std::ifstream playerParam;
	//playerParam.open("");



	//playerParam.close();
}


void Player::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Player::initialize()
{
	setHeapNum();

	position = 0.0f;
	velocity = 1.0f;
	speed = 0.5f;

	collisionFlag.sphere = true;
	//sphereData.resize(initialBonePos.size());

	boneMovePos.resize(initialBonePos.size(), 0);
	
	//��]��region�̐��l���q�̊֐��ŃZ�b�g����\��
	loadParam();

#pragma region ��]
	rotateFlag = false;
	twistAngles.resize(initialBonePos.size(), 0);
	tienTimer = 0;
	tienTime = 4;
	rotateSpeed = 7;//1�t���[���ɉ�]����p�x
	pushRotateAngle = 360;
#pragma endregion
}

void Player::update()
{
	//�{�[����
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region �ړ�����

#pragma endregion

#pragma region ��]����

	if (DirectInput::keyTrigger(DIK_SPACE))
		rotateFlag = true;

	if (rotateFlag) 
	{
		for (int i = 0; i < boneNum; i++)
		{
			//�ݒ��]�ʂ𒴂��ĂȂ����������
			if (twistAngles[i] < pushRotateAngle &&
				pushRotateAngle >= 0 ||
				twistAngles[i] > pushRotateAngle &&
				pushRotateAngle <= -1)
			{
				//�x���I�����ԂɂȂ������]
				if (tienTimer >= tienTime * i)
					twistAngles[i] += rotateSpeed;
			}
			//��������ݒ�ʂ������A��]���~�߂�
			else
				twistAngles[i] = pushRotateAngle;

			//�p�x�Z�b�g
			Library::setOBJBoneAngle({ static_cast<float>(twistAngles[i]), 0,0 }, i, modelData, 0);
		}

		//�S����]���I����������
		if (twistAngles[boneNum - 1] == pushRotateAngle)
		{
			//���Z�b�g
			rotateFlag = false;
			tienTimer = 0;
			for (auto& a : twistAngles)
				a = 0;
		}

		//��]���Ă���J�E���g
		tienTimer++;
	}

#pragma endregion

}

void Player::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);
}

void Player::hit
(
	const Object *const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

void* Player::getPtr()
{
	return this;
}
