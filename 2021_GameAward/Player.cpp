#include "Player.h"
#include"LibMath.h"

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
	//�{�[����
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

	setHeapNum();

	position = {-15.0f,0,0};
	velocity = 1.0f;
	speed = 0.2f;

	collisionFlag.sphere = true;
	//sphereData.resize(initialBonePos.size());

	boneMovePos.resize(boneNum,0.0f);
	moveRotateAngle.resize(boneNum,0.0f);

	bonePos.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;
	
	//��]��region�̐��l���q�̊֐��ŃZ�b�g����\��
	loadParam();

#pragma region ��]
	rotateFlag = false;
	twistAngles.resize(boneNum);
	tienTimer = 0;
	tienTime = 4;
	rotateSpeed = 2;//1�t���[���ɉ�]����p�x
	pushRotateAngle = 360;
#pragma endregion

#pragma region �ړ�
	velRot = 0.0f;
#pragma endregion

}
float ang = 0.0f;
void Player::update()
{
	//�{�[����
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region �ړ�����
	if (DirectInput::keyState(DIK_Z)) 
	{

		if (DirectInput::keyState(DIK_LEFT))
			velRot += 5.0f;
		if (DirectInput::keyState(DIK_RIGHT))
			velRot -= 5.0f;

		float radVelRot = LibMath::angleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;

		for (UINT i = 0; i < boneNum; i++)
		{

			if (i == 0)
			{
				//��]�p�x
				moveRotateAngle[i] = velRot;
				boneMovePos[i] += velocity * speed;

				//�����ʒu����ǂ̂��炢�����Ă��邩���Z�b�g
				Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
			}
			else
			{
				//�O�̃{�[���ւ̃x�N�g��
				Vector3 forwardVector = bonePos[i - 1] - bonePos[i];

				float length = sqrt
				(
					forwardVector.x * forwardVector.x +
					forwardVector.y * forwardVector.y +
					forwardVector.z * forwardVector.z
				);
				boneMovePos[i] += forwardVector / length * speed;

				moveRotateAngle[i] = LibMath::angleConversion(1, atan2( forwardVector.z, forwardVector.x));
				moveRotateAngle[i] = moveRotateAngle[i] == 90.0f ? 0 : moveRotateAngle[i];


				float size = initialBonePos[i - 1].x - initialBonePos[i].x;
				//�߂Â��o�O�΍��if?
				//if (length >= size)
					Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
			}

			//���W����
			bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;


		}
	}
#pragma endregion

#pragma region ��]����

	if (DirectInput::keyTrigger(DIK_SPACE))
		rotateFlag = true;

	if (rotateFlag) 
	{
		for (UINT i = 0; i < boneNum; i++)
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

	//�p�x�Z�b�g
	for(UINT i = 0; i < boneNum;i++)
		Library::setOBJBoneAngle({ static_cast<float>(twistAngles[i]) ,-moveRotateAngle[i],0 }, i, modelData, 0);
	
	Library::setPosition(position, modelData, heapNum);
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
