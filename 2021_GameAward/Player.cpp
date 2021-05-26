#include "Player.h"
#include"LibMath.h"

#include"ValueLoader.h"
#include"XInputManager.h"
#include"ObjectManager.h"
#include"SceneManager.h"

#include"PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"

#include"EnemyBullet.h"

#include"Play.h"
#include"StageSelect.h"
#include"GameOver.h"

#include<fstream>


//�t�@�C������ǂ݂Ƃ���static�ɓ�����邩�m���߂�

ObjModel Player::modelData;
const int Player::CREATE_NUMBER = 2;
std::vector<Vector3> Player::initialBonePos;
std::vector<Vector3> Player::initialBonePosMulScale;
int Player::boneNum;
HeapIndexManager Player::playerModelHeapIndexManager(CREATE_NUMBER);


Sprite2D Player::hpAnimationSprite;
Texture Player::hpAnimationTexture;
Sprite2D Player::hpCrossSpr;
Texture Player::hpCrossTex;
Sprite2D Player::hpNumSpr;
Texture Player::hpNumTex;

Player::Player()
{
	Initialize();
}


Player::Player(const Vector3& pos)
{
	modelMoveVector = pos;
	Initialize();

	std::reverse(boneMovePos.begin(), boneMovePos.end());
	//�Z�b�g
	for (int i = 0; i < boneNum; i++) 
	{
		modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
	}
	position = bonePos[bonePos.size() / 2];
	modelData.SetPosition(modelMoveVector, heapNum);
}


Player::~Player()
{
	playerModelHeapIndexManager.DrawEndCallFunction(heapNum);
}

void Player::LoadResource()
{
	//���f��
	modelData.LoadModel("Resources/Model/Player/snake.obj", true, CREATE_NUMBER, 0);

	initialBonePos = modelData.GetBonePosition();
	boneNum = static_cast<int>(initialBonePos.size());

	
	//�X�v���C�g
	hpAnimationSprite.CreateSprite();
	hpAnimationTexture.LoadSpriteTexture("Resources/Texture/hpAnimation.png");
	hpAnimationSprite.SetPosition(Vector2(0, 0));

	hpCrossSpr.CreateSprite();
	hpCrossSpr.SetPosition(Vector2(70, 0));
	hpCrossSpr.SetScale(Vector2(0.4, 0.4));
	hpCrossTex.LoadSpriteTexture("Resources/Texture/numCross.png");

	hpNumSpr.CreateSprite();
	hpNumTex.LoadSpriteTexture("Resources/Texture/TimeNumber.png");
	hpNumSpr.SetPosition(Vector2(140, 10));

	////�X�v���C�g
	//targetLockSprite.CreateSprite({10,10});
	//targetLockTexture.LoadSpriteTexture("Resources/Texture/lock.png");
	//targetLockSprite.SetBillboardFlag(true,true,true);
	//targetLockSprite.SetAngle({90,0,0});
}


void Player::Initialize()
{
	heapNum = playerModelHeapIndexManager.GetHeapIndex();

#pragma region �ǂݎ��
	std::unordered_map<std::string, int>iMap;
	std::unordered_map<std::string, float>fMap;
	std::unordered_map<std::string, Vector3>v3Map;
	ValueLoader* valieLoader = ValueLoader::getInstance();

	valieLoader->loadValue("Resources/Data/player.msd", iMap, fMap, v3Map);
#pragma endregion


	position = { 0.0f,0.0f,0.0f };
	velocity = { 1.0f,0.0f,0.0f };
	//initSpeed = fMap["speed"];
	initSpeed = 0.25f;
	speed = initSpeed;

	//selectStage = false;
#pragma region �p�����[�^�[


	//hp = iMap["hp"];
	hp = 5;
	isMuteki = false;
	mutekiTimer = 0;
	mutekiTime = 60 * 3;
#pragma endregion

	scale = { 1,1,1 };

	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))
	{
		speedMag = 25.0f;
		speed = initSpeed * speedMag;
		scale = { 20,20,20 };
	}

	modelData.SetScale(scale, heapNum);

	initialBonePosMulScale = initialBonePos;

	boneMovePos.resize(boneNum, 0.0f);
	bonePos.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
	{
		//�X�P�[�����|����
		initialBonePosMulScale[i] *= scale;

		//�Z�b�g
		modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
	}


	

#pragma region �Ђ˂�
	rotateFlag = false;
	twistAngles.resize(boneNum);
	tienTimer = 0;
	//tienTime = iMap["tienTime"];
	tienTime = 4;
	//rotateSpeed = fMap["rotateSpeed"];//1�t���[���ɉ�]����p�x
	rotateSpeed = 3.0f;
	//pushRotateAngle = fMap["pushRotateAngle"];
	pushRotateAngle = 360.0f;
#pragma endregion

#pragma region �ړ�
	velRot = 0.0f;
	previousRot = velRot;
	moveRotateAngle.resize(boneNum, 0.0f);
	boneVelocity.resize(boneNum, 0.0f);

	targetRotatePlayer = false;
	lockTargetNum = -1;
#pragma endregion

#pragma region �e�̔���
	//shotTime = iMap["shotTimer"];
	shotTime = 60 * 1;
	shotTimer = 0;
#pragma endregion

#pragma region ����
	collisionFlag.sphere = true;

	sphereData.resize(boneNum);
	sphereData[0].r = 0.6f * scale.x;
	for (int i = 1; i < boneNum; i++)
	{
		sphereData[i].position = bonePos[i];
			
			/*if(i <= boneNum - 2)
				sphereData[i].r = 0.8f * scale.x;
			else*/
				sphereData[i].r = 1.5f * scale.x;
	}

#pragma endregion


	modelData.SetPosition(modelMoveVector, heapNum);
	//�p�x�Z�b�g
	for (int i = 0; i < boneNum; i++) 
		modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);

	//�X�v���C�g�֌W
	hpAnimationTimer.SetMaxTime(HP_ANIMATION_ONE_FREAM_TIME * 8);
	hpAnimationTimer.SetStopFlag(false);

}

void Player::PlayMove()
{

	lockTarget = false;

	//�߂��I�u�W�F�N�g�̋��������߂�
	float nearTergetDis = 9999999.0f;
	float targetDistance = 0.0f;
	Vector3 nearTergetPosition = 0;

	auto targetPosSize = targetPos.size();
	int nearTargetNum = -1;
	for (int i = 0; i < targetPosSize; i++)
	{
		targetDistance = LibMath::CalcDistance3D
		(
			bonePos[0],
			targetPos[i]
		);
		if (nearTergetDis > targetDistance)
		{
			nearTergetPosition = targetPos[i];
			nearTergetDis = targetDistance;
			nearTargetNum = i;
		}
	}
	float maxDistance = 40.0f;
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))maxDistance *= 17.0f;

	//�����]�t���O
	bool distanceFlag = nearTergetDis <= maxDistance;
	
	bool padTargetObjectRot =
		XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonState(XInputManager::XInputButton::XINPUT_LB_BUTTON, 1)
		&& distanceFlag;

	bool keyTargetObjectRot = DirectInput::KeyState(DIK_X)
		&& distanceFlag;

	if (keyTargetObjectRot
		|| padTargetObjectRot)
	{
		lockTarget = true;
		lockTargetNum = nearTargetNum;

		//�K�ɋ߂Â�����
		float minDistance = 10.0f;
		if (typeid(*currentScene) == typeid(StageSelect))minDistance *= 30.0f;

		if (nearTergetDis >= minDistance
			&& !targetRotatePlayer)
		{
			targetRotatePlayer = false;

			Vector3 playerToNeraTarget = nearTergetPosition - bonePos[0];
			float playerToNeraTargetAngle = LibMath::Vecto2ToAngle
			(
				{ playerToNeraTarget.x,playerToNeraTarget.z },
				true
			);
			velRot = playerToNeraTargetAngle;

		}
		//��鏈��
		else
		{
			if (!targetRotatePlayer)
			{
				targetRotatePlayer = true;
				//�����ŁA�^�[�Q�b�g����v���C���[�ւ̃x�N�g���̊p�x�����߁AvelRot�ɓ����
				Vector3 targetToPlayerToNera = bonePos[0] - nearTergetPosition;
				float targetToPlayerToNeraAngle = LibMath::Vecto2ToAngle
				(
					{ targetToPlayerToNera.x,targetToPlayerToNera.z },
					true
				);
				velRot = targetToPlayerToNeraAngle - 90.0f;
			}
			if (typeid(*currentScene) == typeid(StageSelect))
			{
				/*if (selectStage)
				{
					float subRotNum = 2.5f;
					velRot -= subRotNum;
					speed = initSpeed * stageSelectSpeedMag * subRotNum;
				}
				else*/
				velRot -= initSpeed * 5;
			}
			else
				velRot -= initSpeed * 6;
		}

	}
	//�ړ�
	else
	{
		targetRotatePlayer = false;

		if (XInputManager::GetPadConnectedFlag(1))
		{
			if (XInputManager::LeftStickDown(40, 1) ||
				XInputManager::LeftStickUp(40, 1) ||
				XInputManager::LeftStickLeft(40, 1) ||
				XInputManager::LeftStickRight(40, 1))
				velRot = XInputManager::LeftStickAngle(1);
		}
		else
			velRot = DirectInput::ArrowKeyAngle();
	}


}

void Player::StageSelectMove()
{
	float velRotAngle = initSpeed * 5;
	if (StageSelect::GetStageSelectState() == StageSelect::STAGE_SELECT_STATE_SELECT_END)
	{
	 	const float mulAngleNum = 6.0f;
		velRotAngle *= mulAngleNum;
		speed = initSpeed * speedMag * mulAngleNum;
	}

	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360;
}

void Player::GameOverMove()
{
	float velRotAngle = initSpeed * 5;
	if (GameOver::GetSelectEndFlag())
	{
		const float mulAngleNum = 6.0f;
		velRotAngle *= mulAngleNum;
		speed = initSpeed *  mulAngleNum;
	}

	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360;
}

void Player::PauseMove()
{
	speedMag = 2.0f;
	speed = initSpeed * speedMag;
	scale = 0.3f;

	float velRotAngle = initSpeed * 14.0f;
	
	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360; 
	
	modelData.SetScale(scale, heapNum);
}

void Player::Update()
{
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	//�����O�͓����Ȃ��悤��
	if (typeid(*currentScene) == typeid(Play)
		&& !Play::GetIsPauseFlag())
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET)return;
		else if (Play::GetPlaySceneState() == Play::PLAY_SCENE_START_PREVIOUS)
		{
			//**�����ɃQ�[���J�n�O�̃v���C���[��~����������**//

			return;
		}
	}


#pragma region �p�����[�^�[����

#pragma region ���C�t
	if (hp <= 0) 
	{
		isDead = true;
	}
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

	if(isDead)
	{
		//�_�ŗp
		isMuteki = true;
		return;
	}

	lockTargetNum = -1;


#pragma region �ړ�_�ړ����̉�]����

	previousRot = velRot;

	if (typeid(*currentScene) == typeid(Play))
	{
		if (Play::GetIsPauseFlag())
		{
			PauseMove();
		}
		else 
		{
			PlayMove();
		}
	}
	if (typeid(*currentScene) == typeid(StageSelect)) 
	{
		StageSelectMove();
	}
	if (typeid(*currentScene) == typeid(GameOver)) 
	{
		GameOverMove();
	}
	

	if (velRot != -1)
	{
		float radVelRot = LibMath::AngleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;
	}
	else//���͂���ĂȂ�������O�t���[���̊p�x����
		velRot = previousRot;


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

			//���W����
			bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
		}
		else
		{
			////�O�̃{�[���ւ̃x�N�g��
			Vector3 forwardVector = bonePos[i - 1] - bonePos[i];
			////�ړ��ɂ��Y����̉�]�p�x����
			//moveRotateAngle[i] = LibMath::AngleConversion(1, atan2(forwardVector.z, forwardVector.x));
			moveRotateAngle[i] = LibMath::Vecto2ToAngle({ forwardVector.x,forwardVector.z }, true);


			float bonePosDistance = LibMath::CalcDistance3D(bonePos[i - 1], bonePos[i]);
			float defaultBoneDistance = LibMath::CalcDistance3D(initialBonePosMulScale[i - 1], initialBonePosMulScale[i]);

			//�f�t�H���g�����ȏ�̎��ɁAdefaultBoneDistance�𒴂��Ȃ��悤�Ɉړ�bonePos[i] = initialBonePosMulScale[i] 
			if (bonePosDistance >= defaultBoneDistance)
			{
				//�������߂�
				float disDifference = bonePosDistance - defaultBoneDistance;

				boneVelocity[i] = Vector3Normalize(bonePos[i - 1] - bonePos[i]);

				//�����̍������ړ�������
				//����ɂ��߂Â�������h����
				boneMovePos[i] = LibMath::FloatDistanceMoveVector3(boneMovePos[i], boneVelocity[i], disDifference);

				//�Z�b�g
				modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
				bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
			}

		}



	}
#pragma endregion

	auto SetAngleAndPos = [&]()
	{
		//�p�x�Z�b�g
		for (int i = 0; i < boneNum; i++)
		{
			modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);
		}

		position = bonePos[bonePos.size() / 2];
		modelData.SetPosition(modelMoveVector, heapNum);
	};

	if(Play::GetIsPauseFlag())
	{
		SetAngleAndPos();
		return;
	}

#pragma region �Ђ˂菈��

	if (!XInputManager::GetPadConnectedFlag(1))
	{
		if (DirectInput::KeyTrigger(DIK_SPACE))
			rotateFlag = true;
	}
	else
		if (XInputManager::ButtonTrigger(XInputManager::XINPUT_RB_BUTTON, 1))
			rotateFlag = true;


	if (rotateFlag && typeid(*currentScene) == typeid(Play))
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

	SetAngleAndPos();

#pragma region �e�𔭎�

	auto shotBullet = [&](const UINT& arrayNum)
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_GAMECLEAR)return;

		Vector3 forwordVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
		Vector3 normalizeForwordVector = Vector3Normalize(forwordVector);
		Quaternion q = GetRotateQuaternion(normalizeForwordVector, { 0,1,0 }, -90 + twistAngles[arrayNum]);

		ObjectManager::GetInstance()->AddObject(std::make_shared<PlayerBullet>
			(
				//Vector3(bonePos[arrayNum].x, bonePos[arrayNum].y - 0.5f, bonePos[arrayNum].z),
				bonePos[arrayNum] + Vector3(q.x * 2.5 ,-0.2f, q.z * 2.5),
				Vector3(q.x, 0, q.z)
				));

		Library::PlaySoundEveryLoad("Resources/Sound/SE/PlayerSE/SneakBulletSe.wav");
	};

	//���V���b�g(����)
	/*if (shotTimer >= shotTime)
		shotTimer = 0;
	if (twistAngles[0] == 0)
		shotTimer++;

	if (shotTimer >= shotTime)
	{
		for (int i = 1; i < boneNum - 1; i++)
		{
			shotBullet(i);
		}
	}*/

	//�V�V���b�g(�蓮)
	bool keyShot = DirectInput::KeyTrigger(DIK_Z)
		&& !rotateFlag
		&& typeid(*currentScene) == typeid(Play);

	bool padShot = XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_X_BUTTON, 1) 
		&& !rotateFlag
		&& typeid(*currentScene) == typeid(Play);

	if (keyShot || padShot)
	{
		for (int i = 3; i < boneNum - 3; i++)
		{
			if (i % 2 == 0)continue;
			shotBullet(i);
		}
	}


	for (int i = 3; i < boneNum - 3; i++)
	{
		if (i % 2 == 0)continue;

		if (twistAngles[i] >= 180 &&
			twistAngles[i] < 180 + rotateSpeed)
			shotBullet(i);
	}


#pragma endregion

#pragma region ����
	for (int i = 0; i < boneNum; i++)
		sphereData[i].position = bonePos[i];

#pragma endregion

	//�X�v���C�g���W
	/*if (targetLock) 
	{
		Vector3 targetSprPos = targetPos[targetNum];
		targetLockSprite.SetPosition(targetSprPos);
	}*/


}

void Player::Draw()
{
	if (isMuteki == false || (isMuteki == true && mutekiTimer % 2 == 0))
	modelData.Draw(heapNum);

	/*if (targetLock)
		targetLockSprite.Draw(&targetLockTexture);*/

	

}

void Player::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (isDead)
		return;

	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))return;

	//���\�������Ԃł��ʂꂿ�Ⴄ����C�����邩�A���Ԃ��ł��Ȃ��悤�ɂ���
	if (typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(TargetObject))
	{

		auto MoveBone = [&]()
		{
			float bonePosDistance = LibMath::CalcDistance3D(bonePos[arrayNum - 1], bonePos[arrayNum]);
			float defaultBoneDistance = LibMath::CalcDistance3D(initialBonePosMulScale[arrayNum - 1], initialBonePosMulScale[arrayNum]);

			//�f�t�H���g�����ȏ�̎��ɁAdefaultBoneDistance�𒴂��Ȃ��悤�Ɉړ�
			if (bonePosDistance >= defaultBoneDistance)
			{
				//�������߂�
				float disDifference = bonePosDistance - defaultBoneDistance;

				boneVelocity[arrayNum] = Vector3Normalize(bonePos[arrayNum - 1] - bonePos[arrayNum]);

				//�����̍������ړ�������
				//����ɂ��߂Â�������h����
				boneMovePos[arrayNum] = LibMath::FloatDistanceMoveVector3
				(
					boneMovePos[arrayNum],
					boneVelocity[arrayNum],
					disDifference
				);

				//�Z�b�g
				modelData.SetBoneMoveVector(boneMovePos[arrayNum], arrayNum, heapNum);
				bonePos[arrayNum] = initialBonePosMulScale[arrayNum] + boneMovePos[arrayNum] + modelMoveVector;
			}
		};

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

				/*Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;*/

				MoveBone();
			}

			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (boneVelocity[arrayNum].x <= 0)
				boneMovePos[arrayNum].x += -1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			if (arrayNum != 0)
				MoveBone();

			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (boneVelocity[arrayNum].z <= 0)
				boneMovePos[arrayNum].z += -1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
				MoveBone();

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (boneVelocity[arrayNum].z >= 0)
				boneMovePos[arrayNum].z += 1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
				MoveBone();
			break;
		}

		bonePos[arrayNum] = initialBonePosMulScale[arrayNum] + boneMovePos[arrayNum] + modelMoveVector;

	}


	if (typeid(*object) == typeid(EnemyBullet))
		DamageFromEnemy();

	//if (isMuteki)return;
	////if (typeid(*object) == typeid(Enemy))
	//{
	//	isMuteki = true;

	//}
}

void Player::UpdateHpAnimation()
{
	//HP�A�j���[�V����
	if (hpAnimationTimer.GetMultipleTimeFlag(HP_ANIMATION_ONE_FREAM_TIME))
		hpAnimationNum++;
	if (hpAnimationTimer.GetSameAsMaximumFlag())
		hpAnimationNum = 0;

}

void Player::DrawHp()
{	
	
	//�ւ̃A�j���[�V����
	const Vector2 hpAnimationTextureSize = hpAnimationTexture.GetTextureSize();
	hpAnimationSprite.SelectDrawAreaDraw
	(
		Vector2(hpAnimationTextureSize.x / 8 * hpAnimationNum, 0),
		Vector2(hpAnimationTextureSize.x / 8 * (hpAnimationNum + 1), hpAnimationTextureSize.y),
		&hpAnimationTexture
	);

	//������
	hpCrossSpr.Draw(&hpCrossTex);

	//����
	const float numSize = hpNumTex.GetTextureSize().y;
	hpNumSpr.SelectDrawAreaDraw(Vector2(numSize * hp, 0), Vector2(numSize * (hp + 1), numSize), &hpNumTex);
}


void Player::SetModelMoveVector(const Vector3& vector) 
{
	modelMoveVector = vector; 
	modelData.SetPosition(modelMoveVector,heapNum);
	for (int i = 0; i < boneNum; i++) 
	{
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
		sphereData[i].position = bonePos[i];
	}
}


void Player::DamageFromEnemy()
{
	if (isMuteki)
		return;
	hp--;
	isMuteki = true;
}


