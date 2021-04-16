#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#include"LibMath.h"

#pragma region �I�u�W�F�N�g
#include"Block.h"


#pragma endregion


float Play::targetDistance;
float Play::playerDistance;
int Play::targetNumber;
Vector3 Play::leftUpPosition;
Vector3 Play::rightDownPosition;
std::vector<Vector3> Play::blockPositions;
std::vector<Vector3> Play::blockScales;

Play::Play()
{
}

Play::~Play(){}

void Play::initialize()
{
	player = new Player();
	enemy = new ShotEnemy();
	enemy->setPPlayer(player);
	enemy->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	ObjectManager::getInstance()->addObject(player);
	ObjectManager::getInstance()->addObject(enemy);

	playSceneState = PlaySceneState::PLAY_SCENE_SETTARGET;

#pragma region �J����
	cameraPosition = { 0,50,-2 };
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region �u���b�N�Z�b�g
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::getInstance()->addObject(new Block(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region �K�Z�b�g
	//targetObjects.resize(targetNumber);
	//for (auto& t : targetObjects)
	//{

	//	//���W�Z�b�g
	//	//�����͈͗�������ĕς���
	//	Vector3 targetPos =
	//	{
	//		Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
	//		0,
	//		Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
	//	};

	//	t = new TargetObject(targetPos);
	//	ObjectManager::getInstance()->addObject(t);
	//}
#pragma endregion



	//��
	Block::setGameStart(true);

}

void Play::update()
{
	ObjectManager::getInstance()->update();

#pragma region �K����

//	//�K�Z�b�g
//	//���W�̒l���ς���ĂȂ��̂Ƀ��[�v���Ă��̂́A
//	//�������J��Ԃ��ăq�[�v�̔ԍ�����������ɁA������u����Ă��K�̃��f���̍��W���������̍��W�ŏ㏑�����ꂽ����
//	if (playSceneState == PlaySceneState::PLAY_SCENE_SETTARGET)
//	{
//		int createMissCount = 0;
//		for (auto& t : targetObjects)
//		{
//			//nullptr�͔�΂�
//			if (!t)continue;
//			//�ݒu�����ς͔�΂�
//			if (t->getSetEnd())continue;
//
//			//�u���b�N�Ƃ��v���C���[�Əd�Ȃ�������J�E���g
//			if (t->getCreateHitObject())
//			{
//				createMissCount++;
//				continue;
//			}
//
//			//��������
//			bool distanceCheck = true;//�����̊�𖞂��������ǂ���
//			float dis;
//			for (auto& t2 : targetObjects)
//			{
//				//�^�[�Q�b�g���m
//				if (!t2 || t == t2)continue;
//				dis = LibMath::calcDistance3D
//				(
//					t->getPosition(),
//					t2->getPosition()
//				);
//
//				//�Œ჉�C���������Ȃ�������J�E���g&������
//				if (dis <= targetDistance)
//				{
//					createMissCount++;
//					distanceCheck = false;
//					break;
//				}
//
//
//			}
//
//
//			//�v���C���[
//			dis = LibMath::calcDistance3D
//			(
//				t->getPosition(),
//				player->getPosition()
//			);
//			if (dis <= playerDistance)
//			{
//				createMissCount++;
//				distanceCheck = false;
//				break;
//			}
//
//			//�����̊�𖞂�������ݒu����
//			if (distanceCheck)
//				t->trueSetEnd();
//
//		}
//
//
//#pragma region �K���W�ăZ�b�g
//		for (auto& t : targetObjects)
//		{
//			//�ݒu�����ς͔�΂�
//			if (t->getSetEnd())continue;
//
//			//���W�Z�b�g
//			//�����͈͗�������ĕς���
//			Vector3 targetPos =
//			{
//				Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
//				0,
//				Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
//			};
//
//			t->setPosition(targetPos);
//
//		}
//
//#pragma endregion
//
//		//�J�E���g����Ȃ�������Z�b�g�I��
//		if (createMissCount == 0)
//		{
//			//���ɂ������Ă�
//			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
//			Block::setGameStart(true);
//		}
//	}
//	else//�K�����c��m�F(�N���A����p)
//	{
//		int deadCount = 0;
//		for (auto& t : targetObjects)
//		{
//			if (!t)
//			{
//				deadCount++;
//				continue;
//			}
//			//���ꂽ�����
//			if (t->getEraseManager())
//				t = nullptr;
//		}
//
//		//if(deadCount == targetObjects.size())
//			
//	}

#pragma endregion

#pragma region �J�����ړ�
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += {0, 50, -2};
	cameraTarget = pHeapPos;
	Library::setCamera(cameraPosition, cameraTarget, { 0,1,0 });

#pragma endregion

	ObjectManager::getInstance()->isDeadCheck();
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();



}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}

void Play::setStageData
(
	std::vector<Vector3>blockPos,
	std::vector<Vector3>blockScale,
	const float& targetDis,
	const float& playerDis,
	const int& targetNum,
	const Vector3& leftUpPos,
	const Vector3& rightDownPos
)
{
	blockPositions = blockPos;
	blockScales = blockScale;
	targetDistance = targetDis;
	playerDistance = playerDis;
	targetNumber = targetNum;
	leftUpPosition = leftUpPos;
	rightDownPosition = rightDownPos;

}