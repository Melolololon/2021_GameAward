#include "Player.h"
#include"LibMath.h"

#include"ValueLoader.h"
#include"XInputManager.h"
#include"ObjectManager.h"

#include"PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"

#include<fstream>



//�t�@�C������ǂ݂Ƃ���static�ɓ�����邩�m���߂�

ObjModel Player::modelData;
int Player::createCount;
const int Player::CREATE_NUMBER = 1;
std::vector<Vector3> Player::initialBonePos;


Player::Player()
{
	Initialize();
}

Player::~Player()
{
}

void Player::LoadResource()
{
	std::string mtl;

	modelData.LoadModel("Resources/Model/testSnake.obj", true, CREATE_NUMBER, 0);
	/*Library::loadOBJVertex("Resources/Model/testSnake.obj", true, true, &mtl, modelData);
	Library::loadOBJMaterial("Resources/Model/", mtl, CREATE_NUMBER, modelData);*/
	//initialBonePos = Library::getBonePosition(modelData);
	initialBonePos = modelData.GetBonePosition();
}



void Player::SetHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Player::Initialize()
{
	//�{�[����
	const int boneNum = static_cast<int>(initialBonePos.size());

	SetHeapNum();

#pragma region �ǂݎ��
	std::unordered_map<std::string, int>iMap;
	std::unordered_map<std::string, float>fMap;
	std::unordered_map<std::string, Vector3>v3Map;
	ValueLoader* valieLoader = ValueLoader::getInstance();

	valieLoader->loadValue("Resources/Data/player.msd", iMap, fMap, v3Map);
#pragma endregion


	position = { 0.0f,0.0f,0.0f };
	velocity = { 1.0f,0.0f,0.0f };
	speed = fMap["speed"];

#pragma region �p�����[�^�[


	hp = iMap["hp"];
	isMuteki = false;
	mutekiTimer = 0;
	mutekiTime = 60 * 3;
#pragma endregion

	boneMovePos.resize(boneNum, 0.0f);
	bonePos.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;

#pragma region �Ђ˂�
	rotateFlag = false;
	twistAngles.resize(boneNum);
	tienTimer = 0;
	tienTime = iMap["tienTime"];
	rotateSpeed = fMap["rotateSpeed"];//1�t���[���ɉ�]����p�x
	pushRotateAngle = fMap["pushRotateAngle"];
#pragma endregion

#pragma region �ړ�
	velRot = 0.0f;
	previousRot = 0.0f;
	moveRotateAngle.resize(boneNum, 0.0f);
	boneVelocity.resize(boneNum, 0.0f);
#pragma endregion

#pragma region �e�̔���
	//shotTime = iMap["shotTimer"];
	shotTime = 60 * 1;
	shotTimer = 0;
#pragma endregion


#pragma region ����
	collisionFlag.sphere = true;

	sphereData.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
	{
		sphereData[i].position = bonePos[i];
		sphereData[i].r = 0.4f;
	}

#pragma endregion


}
void Player::Update()
{
	//�{�[����
	const int boneNum = static_cast<int>(initialBonePos.size());



#pragma region �ړ�_�ړ����̉�]����

#pragma region �I�I�C�V�ړ�

	//if (DirectInput::keyState(DIK_LEFT))
	//	velRot += 5.0f;
	//if (DirectInput::keyState(DIK_RIGHT))
	//	velRot -= 5.0f;



	/*float radVelRot = LibMath::angleConversion(0, velRot);
	velocity = { cos(radVelRot) ,0,sin(radVelRot) };
	float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	velocity /= length;
*/

#pragma endregion

#pragma region �C�`�J���ړ�
	previousRot = velRot;

	if (XInputManager::GetPadConnectedFlag(1))
	{
		if (XInputManager::LeftStickDown(40, 1) ||
			XInputManager::LeftStickUp(40, 1) ||
			XInputManager::LeftStickLeft(40, 1) ||
			XInputManager::LeftStickRight(40, 1))
		{
			velRot = XInputManager::LeftStickAngle(1);
		}
	}
	else
		velRot = DirectInput::ArrowKeyAngle();


	if (velRot != -1)
	{
		float radVelRot = LibMath::AngleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;
	}
	else
		velRot = previousRot;

#pragma endregion

#pragma region Unity�ړ�
	//velRot = DirectInput::getMouseAngle();

	//float radVelRot = LibMath::angleConversion(0, velRot);
	//velocity = { cos(radVelRot) ,0,sin(radVelRot) };
	//float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	//velocity /= length;
#pragma endregion


	for (int i = 0; i < boneNum; i++)
	{
		if (i == 0)
		{
			//��]�p�x
			moveRotateAngle[i] = velRot;
			boneMovePos[i] += velocity * speed;

			//�����ʒu����ǂ̂��炢�����Ă��邩���Z�b�g
			//Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
			modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
			boneVelocity[i] = velocity;
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

			moveRotateAngle[i] = LibMath::AngleConversion(1, atan2(forwardVector.z, forwardVector.x));

			//moveRotateAngle[i] = moveRotateAngle[i] == 90.0f ? 0 : moveRotateAngle[i];


			float size = initialBonePos[i - 1].x - initialBonePos[i].x;
			//�߂Â��o�O�΍��if?
			if (length >= size)
			{
				boneMovePos[i] += forwardVector / length * speed;
				//Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
				modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
				boneVelocity[i] = forwardVector / length;
			}
		}

		//���W����
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;
	}


	//��O�h�~
	//�e�X�g�v���C�I���̃��Z�b�g�Ɏg�p



#pragma endregion

#pragma region �Ђ˂菈��

	if (!XInputManager::GetPadConnectedFlag(1))
	{
		if (DirectInput::KeyTrigger(DIK_SPACE))
			rotateFlag = true;
	}
	else
		if (XInputManager::ButtonTrigger(XInputManager::XINPUT_RB_BUTTON, 1))
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
	for (int i = 0; i < boneNum; i++)
		//Library::setOBJBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, modelData, 0);
		modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);
#pragma region �e�𔭎�

	auto shotBullet = [&](const UINT& arrayNum)
	{
		Vector3 normalizeForwordVector = Vector3Normalize(bonePos[arrayNum - 1] - bonePos[arrayNum]);
		Quaternion q = GetRotateQuaternion(normalizeForwordVector, { 0,1,0 }, -90 + twistAngles[arrayNum]);

		ObjectManager::GetInstance()->AddObject(std::make_shared<PlayerBullet>(bonePos[arrayNum], Vector3(q.x, 0, q.z)));
	};

	if (shotTimer >= shotTime)
		shotTimer = 0;
	if (twistAngles[0] == 0)
		shotTimer++;

	if (shotTimer >= shotTime)
	{
		for (int i = 1; i < boneNum - 1; i++)
		{
			shotBullet(i);
		}
	}

	for (int i = 1; i < boneNum - 1; i++)
	{
		if (twistAngles[i] >= 180 &&
			twistAngles[i] <= 180 + rotateSpeed)
			shotBullet(i);
	}


#pragma endregion

#pragma region ����
	for (int i = 0; i < boneNum; i++)
		sphereData[i].position = bonePos[i];

#pragma endregion

#pragma region �p�����[�^�[����

#pragma region ���C�t

#pragma endregion

#pragma region ���G����
	if (isMuteki)
	{
		mutekiTimer++;
		if (mutekiTimer >= mutekiTime)
		{
			mutekiTimer = 0;
			isMuteki = false;
		}
	}
#pragma endregion


#pragma endregion



}

void Player::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}

void Player::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	//���\�������Ԃł��ʂꂿ�Ⴄ����C�����邩�A���Ԃ��ł��Ȃ��悤�ɂ���
	if (typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(TargetObject))
	{
		switch (sphereData[arrayNum].boxHitDistance)
		{
		case BOX_HIT_DIRECTION_RIGHT:
			//�����ɓ��荞�ނ̖h�~����if
			//velocity���Œ��float�ɂ���΂߂荞�܂Ȃ����A�J�����������܂���̂ł������Ă�
			if (boneVelocity[arrayNum].x >= 0)
				boneMovePos[arrayNum].x += 1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			//�E�ɂԂ����Ă邩������������Ƃ�ɂ́A
			//x�̐i�s���Ȃ����Az�����ɓ����������Ȃ�
			//�擪��velocity����ɂ���ƁA�r���ŋt�s�����Ƃ���
			//���������肪�Ђǂ��Ȃ��Ă��܂�����O�̃{�[���̂��Q��
			//velocity����Ȃ��đ���ւ̃x�N�g�����g�p(velocity���Ƌt�ɐi��ł��܂��Ƃ���������)
			//�����s������Ăǂ����Ă��΍􂪂ł��Ȃ�������������A
			//���if�����Aif(typeid(*object) == typeid(Block) && arrayNum == 0)�ɕύX���Ă�������
			if (arrayNum != 0)
			{

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;
			}

			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (boneVelocity[arrayNum].x <= 0)
				boneMovePos[arrayNum].x += -1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			if (arrayNum != 0)
			{

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;
			}

			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (boneVelocity[arrayNum].z <= 0)
				boneMovePos[arrayNum].z += -1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
			{

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.z = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].x += previousBoneVector.x * speed.x;
			}

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (boneVelocity[arrayNum].z >= 0)
				boneMovePos[arrayNum].z += 1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
			{
				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.z = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].x += previousBoneVector.x * speed.x;
			}
			break;
		}

		bonePos[arrayNum] = initialBonePos[arrayNum] + boneMovePos[arrayNum] + position;

	}

	if (isMuteki)return;
	//if (typeid(*object) == typeid(Enemy))
	{
		isMuteki = true;

	}
}

void* Player::GetPtr()
{
	return this;
}
