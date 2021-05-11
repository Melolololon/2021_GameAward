#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include "SimEnemy.h"
#include "DefenceEnemy.h"
#include "HealEnemy.h"

#include"ObjectManager.h"
#include"XInputManager.h"

#include"LibMath.h"
#include"StageSelect.h"
#pragma region �I�u�W�F�N�g
#include"Block.h"


#pragma endregion

Sprite2D Play::arrowSprite;
Texture Play::arrowTexture;

float Play::targetDistance;
float Play::playerDistance;
int Play::targetNumber;
Vector3 Play::leftUpPosition;
Vector3 Play::rightDownPosition;
std::vector<Vector3> Play::blockPositions;
std::vector<Vector3> Play::blockScales;

Play::PlaySceneState Play::playSceneState;
Play::Play()
{
}


Play::~Play() {}

void Play::LoadResources()
{
	//���\�[�X�̓ǂݍ���
	/*Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");*/
	arrowSprite.CreateSprite();
	arrowTexture.LoadSpriteTexture("Resources/Texture/arrow.png");
}


void Play::Initialize()
{
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);
	

	//�G�ǉ�
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		std::shared_ptr<Enemy> enemy;
		Play::EnemyType enemyType = (Play::EnemyType)1;// (Play::EnemyType)Library::GetRandomNumber(1);
		if (enemyType == Play::EnemyType::ET_MoveEnemy){
			enemy = std::make_shared<MoveEnemy>();
		}
		else if (enemyType == EnemyType::ET_ShotEnemy) {
			enemy = std::make_shared<ShotEnemy>();
		}
		else if (enemyType == EnemyType::ET_FleeEnemy) {
			enemy = std::make_shared<FleeEnemy>();
		}
		/*
		else if (enemyType == EnemyType::ET_SimEnemy) {
			enemy = std::make_shared<SimEnemy>();
		}
		else if (enemyType == EnemyType::ET_DefenceEnemy) {
			enemy = std::make_shared<DefenceEnemy>();
		}
		else if (enemyType == EnemyType::ET_HealEnemy) {
			enemy = std::make_shared<HealEnemy>();
		}*/

		enemy->setPPlayer(player.get());
		//enemy->setPosition(Vector3(Library::GetRandomNumber(100) - 50, 0, Library::GetRandomNumber(100) - 50));
		enemy->setPosition(Vector3(50,0,20));
		ObjectManager::GetInstance()->AddObject(enemy);
		enemies.push_back(enemy);
	}

	playSceneState = PlaySceneState::PLAY_SCENE_SET_TARGET;

#pragma region �J����
	addCameraPosition = { 0,50,-2 };
	cameraPosition = addCameraPosition;
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region �u���b�N�Z�b�g
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::GetInstance()->AddObject(std::make_shared<Block>(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region �K�Z�b�g
	targetObjects.resize(targetNumber);
	for (auto& t : targetObjects)
	{

		//���W�Z�b�g
		//�����͈͗�������ĕς���
		Vector3 targetPos =
		{
			Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
			0,
			Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
		};

		t = std::make_shared<TargetObject>(targetPos);
		ObjectManager::GetInstance()->AddObject(t);
	}
#pragma endregion


#pragma region ���
	arrowPosition = 0.0f;
	arrowAngle = 0.0f;
	drawArrow = false;
#pragma endregion


}

void Play::Update()
{

	ObjectManager::GetInstance()->Update();
#pragma region �K����

	//	//�K�Z�b�g
	//	//���W�̒l���ς���ĂȂ��̂Ƀ��[�v���Ă��̂́A
	//	//�������J��Ԃ��ăq�[�v�̔ԍ�����������ɁA������u����Ă��K�̃��f���̍��W���������̍��W�ŏ㏑�����ꂽ����
	if (playSceneState == PlaySceneState::PLAY_SCENE_SET_TARGET)
	{
		int createMissCount = 0;
		for (auto& t : targetObjects)
		{
			//nullptr�͔�΂�
			if (!t)continue;
			//�ݒu�����ς͔�΂�
			if (t->getSetEnd())continue;

			//�u���b�N�Ƃ��v���C���[�Əd�Ȃ�������J�E���g
			if (t->getCreateHitObject())
			{
				createMissCount++;
				continue;
			}

			//��������
			bool distanceCheck = true;//�����̊�𖞂��������ǂ���
			float dis;
			for (auto& t2 : targetObjects)
			{
				//�^�[�Q�b�g���m
				if (!t2 || t == t2)continue;
				dis = LibMath::CalcDistance3D
				(
					t->GetPosition(),
					t2->GetPosition()
				);

				//�Œ჉�C���������Ȃ�������J�E���g&������
				if (dis <= targetDistance)
				{
					createMissCount++;
					distanceCheck = false;
					break;
				}


			}


			//�v���C���[
			dis = LibMath::CalcDistance3D
			(
				t->GetPosition(),
				player->GetPosition()
			);
			if (dis <= playerDistance)
			{
				createMissCount++;
				distanceCheck = false;
				break;
			}

			//�����̊�𖞂�������ݒu����
			if (distanceCheck)
				t->trueSetEnd();

		}


#pragma region �K���W�ăZ�b�g
		for (auto& t : targetObjects)
		{
			//�ݒu�����ς͔�΂�
			if (t->getSetEnd())continue;

			//���W�Z�b�g
			//�����͈͗�������ĕς���
			Vector3 targetPos =
			{
				Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->setPosition(targetPos);

		}

#pragma endregion

		//�J�E���g����Ȃ�������Z�b�g�I��
		if (createMissCount == 0) 
		{
			//���ɂ������Ă�
			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
		}
	}
	else//�K�����c��m�F(�N���A����p)
	{
		int deadCount = 0;
		for (auto& t : targetObjects)
		{
			//����ăI�u�W�F�N�g�}�l�[�W���[����폜����Ă���J�E���g
			if (t->GetEraseManager())
				deadCount++;
		}

		//�^�[�Q�b�g�̍��W���v���C���[�ɓn��
		auto targetObjectsSize = targetObjects.size();
		std::vector<Vector3>targetObjectPos;
		targetObjectPos.reserve(30);
		for (int i = 0; i < targetObjectsSize; i++) 
		{
			if (!targetObjects[i]->GetEraseManager())
				targetObjectPos.push_back(targetObjects[i]->GetPosition());
		}
		player->SetTargetPosition(targetObjectPos);

		//�I������
		/*if(deadCount == targetObjects.size())
		{
		}*/
	}

#pragma endregion

#pragma region �J�����ړ�
	Vector3 pHeapPos = player->GetHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += addCameraPosition;
	cameraTarget = pHeapPos;
	Library::SetCamera(cameraPosition, cameraTarget, { 0,0,1 });

#pragma endregion


#pragma region �K���������
	//��ԋ߂��K�����߂�
	float minDistance = 9999999.0f;
	Vector3 playerHeadPos = player->GetHeadPosition();
	Vector3 nearTargetPos = 0;
	for (auto& t : targetObjects)
	{
		if (!t->GetEraseManager())
		{
			float dis = LibMath::CalcDistance3D
			(
				playerHeadPos,
				t->GetPosition()
			);
			if (dis < minDistance)
			{
				minDistance = dis;
				nearTargetPos = t->GetPosition();
			}
		}
	}
	//�K��������v�Z
	if (minDistance < 9999999.0f)
	{
		//Vector2 arrowTexSize = Library::GetTextureSize(arrowTexture);
		Vector2 arrowTexSize = arrowTexture.GetTextureSize();

		//�v���C���[�����ԋ߂��^�[�Q�b�g�̃x�N�g���𐳋K����������
		Vector3 playerToTargetVector = nearTargetPos - playerHeadPos;
		Vector3 playerToTargetNVector = Vector3Normalize(playerToTargetVector);

		//�N�H�[�^�j�I���ŉ�?
		//���W
		arrowPosition = { 1280.0f / 2.0f - arrowTexSize.x / 2, 720.0f / 2.0f - arrowTexSize.y / 2 };
		//�K�̕��Ɉړ�������
		arrowPosition.x += playerToTargetNVector.x * (1280 / 2);
		arrowPosition.y -= playerToTargetNVector.z * (720 / 2);

		//��ʓ��Ɏ��܂�悤�ɂ���
		if (arrowPosition.x >= 1280 - arrowTexSize.x)
			arrowPosition.x = 1280 - arrowTexSize.x;
		if (arrowPosition.x <= 0)
			arrowPosition.x = 0;
		if (arrowPosition.y >= 720 - arrowTexSize.y)
			arrowPosition.y = 720 - arrowTexSize.y;
		if (arrowPosition.y <= 0)
			arrowPosition.y = 0;

		//�p�x
		arrowAngle = LibMath::Vecto2ToAngle({ playerToTargetNVector.x,playerToTargetNVector.z }, false);

		//�`�悷�邩�ǂ���
		drawArrow = false;
		if (abs(playerToTargetVector.x) >= 50.0f ||
			abs(playerToTargetVector.z) >= 30.0f)
			drawArrow = true;
	}
#pragma endregion
}

void Play::Draw()
{
	ObjectManager::GetInstance()->Draw();


	//Library::setSpriteAngle(arrowAngle, arrowSprite);
	arrowSprite.SetAngle(arrowAngle);
	arrowSprite.SetPosition(arrowPosition);
	if (drawArrow)
		//Library::drawSprite(arrowPosition, arrowSprite, arrowTexture);
		arrowSprite.Draw(&arrowTexture);
}

void Play::Finitialize()
{
	ObjectManager::GetInstance()->AllEraseObject();
}

Scene* Play::GetNextScene()
{
	return new StageSelect();
}


void Play::SetStageData
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