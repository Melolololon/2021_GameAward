#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include "SimEnemy.h"
#include "DefenceEnemy.h"
#include "HealEnemy.h"
#include "Game.h"

#include"ObjectManager.h"
#include"XInputManager.h"
#include"Fade.h"

#include"LibMath.h"
#include"StageSelect.h"

#include"GameClear.h"
#include"GameOver.h"
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

#pragma region �X�v���C�g

Sprite3D Play::targetLockSprite;
Texture Play::targetLockTexture;

Sprite2D Play::timerSprite[6];
Texture Play::timerTexture;


Sprite2D Play::targetAnimationSprite;
Texture Play::targetAnimationTexture;

#pragma endregion


Play::PlaySceneState Play::playSceneState;
Play::Play()
{
}


Play::~Play() {}

void Play::LoadResources()
{
	/*Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");*/
	arrowSprite.CreateSprite();
	arrowTexture.LoadSpriteTexture("Resources/Texture/arrow.png");

	//�X�v���C�g
	targetLockSprite.CreateSprite({ 10,10 });
	targetLockTexture.LoadSpriteTexture("Resources/Texture/lock.png");
	targetLockSprite.SetBillboardFlag(true, true, true);

	for (int i = 0; i < _countof(timerSprite); i++)
	{
		timerSprite[i].CreateSprite();
	}
	timerTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

	


	targetAnimationSprite.CreateSprite();
	targetAnimationTexture.LoadSpriteTexture("Resources/Texture/targetAnimation.png");
	targetAnimationSprite.SetPosition(Vector2(0,120));
}


void Play::Initialize()
{
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);

	//�G�ǉ�
	for (int i = 0; i < ENEMY_COUNT; i++)
	{

		Play::EnemyType enemyType = (Play::EnemyType)Library::GetRandomNumber(4);
		if (enemyType == Play::EnemyType::ET_MoveEnemy){

			std::shared_ptr<MoveEnemy> enemy = std::make_shared<MoveEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPosition(Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z));
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_ShotEnemy) {
			std::shared_ptr<ShotEnemy> enemy = std::make_shared<ShotEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPosition(Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z));
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_FleeEnemy) {
			std::shared_ptr<FleeEnemy> enemy = std::make_shared<FleeEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPosition(Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z));
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_SimEnemy) {
			std::shared_ptr<SimEnemy> enemy[3];
			Vector3 initPos = Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z);

			for (int j = 0; j < 3; j++)
			{
				enemy[j] = std::make_shared<SimEnemy>();
				enemy[j]->setPPlayer(player.get());
				enemy[j]->setPosition(initPos);
				enemy[j]->Initialize();

				enemy[j]->SetID(j);
			}

			for (int j = 0; j < 3; j++)
			{
				int setNum = j - 1;
				if (setNum < 0) setNum = 2;

				enemy[j]->SetOther(0, enemy[setNum]);

				setNum = j + 1;
				if (setNum > 2) setNum = 0;

				enemy[j]->SetOther(1, enemy[setNum]);

				ObjectManager::GetInstance()->AddObject(enemy[j]);
			}
		}
		/*
		else if (enemyType == EnemyType::ET_DefenceEnemy) {
			enemy = std::make_shared<DefenceEnemy>();
		}
		else if (enemyType == EnemyType::ET_HealEnemy) {
			enemy = std::make_shared<HealEnemy>();
		}*/

	}

	playSceneState = PlaySceneState::PLAY_SCENE_SET_TARGET;

#pragma region �J����
	addCameraPosition = { 0,60,-30 };
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

#pragma region �^�C�}�[


	gameTime.SetMaxTime(INT_MAX);
	gameTime.SetNowTime(-60 * 3);


	sceneEndTimer.SetMaxTime(SCENE_END_TIME);

	
	targetAnimationTimer.SetMaxTime(TARGET_ANIMATION_ONE_FREAM_TIME * 4);
	targetAnimationTimer.SetStopFlag(false);


#pragma endregion

	Fade::GetInstance()->SetIsStopFlag(true);
}

void Play::Update()
{
	

#pragma region �|�[�Y
	if (XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_START_BUTTON ,1))
		isPause = isPause == false ? true : false;

	FreamTimer::SetAllTimerStopFlag(isPause);
	
	if (isPause) 
		return;
	
#pragma endregion

	player->UpdateHpAnimation();

	//�N���A�̃X���[
	if (!slowTimer.GetMultipleTimeFlag(2))
		return;

	ObjectManager::GetInstance()->Update();

#pragma region �J�����ړ�
	Vector3 pHeapPos = player->GetHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += addCameraPosition;
	cameraTarget = pHeapPos;

	Vector3 upVector = LibMath::OtherVector(cameraPosition, cameraTarget);
	upVector = LibMath::RotateVector3(upVector, { 1,0,0 }, 90);
	Library::SetCamera(cameraPosition, cameraTarget, upVector);

#pragma endregion

#pragma region ���


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
		if (abs(playerToTargetVector.x) >= 75.0f ||
			abs(playerToTargetVector.z) >= 55.0f)
			drawArrow = true;
	}

	arrowSprite.SetAngle(arrowAngle);
	arrowSprite.SetPosition(arrowPosition);
#pragma endregion

#pragma region �^�[�Q�b�g
	int playerTargetNum = player->GetLockTargetNum();
	if (playerTargetNum != -1)
	{
		Vector3 lockPos = targetObjects[playerTargetNum]->GetPosition()
			+ Vector3(0, 2.5, -1.5);
		targetLockSprite.SetPosition(lockPos);
	}
#pragma endregion



#pragma endregion

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
			if (t->GetSetEnd())continue;

			//�u���b�N�Ƃ��v���C���[�Əd�Ȃ�������J�E���g
			if (t->GetCreateHitObject())
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
				t->TrueSetEnd();

		}


#pragma region �K���W�ăZ�b�g
		for (auto& t : targetObjects)
		{
			//�ݒu�����ς͔�΂�
			if (t->GetSetEnd())continue;

			//���W�Z�b�g
			//�����͈͗�������ĕς���
			Vector3 targetPos =
			{
				Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->SetPosition(targetPos);

		}

#pragma endregion

		//�J�E���g����Ȃ�������Z�b�g�I��
		if (createMissCount == 0)
		{
			//���ɂ������Ă�
			playSceneState = PlaySceneState::PLAY_SCENE_START_PREVIOUS;
			Fade::GetInstance()->SetIsStopFlag(false);
			gameTime.SetStopFlag(false);
		}
	}
	else if (playSceneState == PlaySceneState::PLAY_SCENE_START_PREVIOUS)
	{
		if (gameTime.GetTime() >= 0)
		{
			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
		}
	}
	else//�K�����c��m�F(�N���A����p)
	{
		auto targetObjectsSize = targetObjects.size();

		std::vector<Vector3>targetObjectPos;
		targetObjectPos.reserve(targetObjectsSize);
		for (int i = 0; i < targetObjectsSize; i++)
		{
			//true�����������
			if (targetObjects[i]->GetEraseManager())
			{
				//�f�o�b�O�p
				if (i == targetObjectsSize - 1)
					int z = 0;

				targetObjects.erase(targetObjects.begin() + i);
				targetObjectsSize--;
				i--;
				int a = 0;
			}
			else//false��������ǉ�
				targetObjectPos.push_back(targetObjects[i]->GetPosition());
		}

		//�^�[�Q�b�g�̍��W���v���C���[�ɓn��
		player->SetTargetPosition(targetObjectPos);

	}

#pragma endregion



#pragma region �I������

	//�N���A
	if (targetObjects.size() == 0 
		&& playSceneState != PlaySceneState::PLAY_SCENE_GAMEOVER)
	{
		sceneEndTimer.SetStopFlag(false);
		slowTimer.SetStopFlag(false);
		gameTime.SetStopFlag(true);
		playSceneState = PlaySceneState::PLAY_SCENE_GAMECLEAR;
	}

	//�Q�[���I�[�o�[
	if (player->GetIsDead())
	{
		sceneEndTimer.SetStopFlag(false);
		//slowTimer.SetStopFlag(false);
		gameTime.SetStopFlag(true);
		playSceneState = PlaySceneState::PLAY_SCENE_GAMEOVER;
	}

	if (sceneEndTimer.GetSameAsMaximumFlag())
		Fade::GetInstance()->FadeStart();

	if(Fade::GetInstance()->GetSceneChangeTimingFlag())
		isEnd = true;
#pragma endregion

}

void Play::Draw()
{

	ObjectManager::GetInstance()->Draw();

	int playerLockTargetNum = player->GetLockTargetNum();
	if (playerLockTargetNum != -1)
		targetLockSprite.Draw(&targetLockTexture);


	//�^�[�Q�b�gHP
	for (auto& t : targetObjects)
		t->DrawHp();


#pragma region �Q�[���^�C�}�[
	int drawNum = gameTime.GetTime() / 60;
	if (drawNum < 0) drawNum *= -1;
	int keta = std::to_string(gameTime.GetTime() / 60).size();
	for (int i = 0; i < keta; i++)
	{
		int n = drawNum % 10;
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40 * (i + 1), 0);
		timerSprite[i].SetPosition(pos);
		timerSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &timerTexture);
		drawNum /= 10;
	}
#pragma endregion


	if (drawArrow)
		arrowSprite.Draw(&arrowTexture);

	//�v���C���[HP
	int playerHP = player->GetHp();

	//player��HP��`��
	player->DrawHp();
	
	//�K
	
	//�K�A�j���[�V����
	if (targetAnimationTimer.GetMultipleTimeFlag(TARGET_ANIMATION_ONE_FREAM_TIME)
		&& !isPause)
		targetAnimationNum++;
	if (targetAnimationTimer.GetSameAsMaximumFlag())
		targetAnimationNum = 0;

	const Vector2 targetAnimationTextureSize = targetAnimationTexture.GetTextureSize();
	targetAnimationSprite.SelectDrawAreaDraw
	(
		Vector2(targetAnimationTextureSize.x / 4 * targetAnimationNum, 0),
		Vector2(targetAnimationTextureSize.x / 4 * (targetAnimationNum + 1), targetAnimationTextureSize.y),
		&targetAnimationTexture
	);

	Fade::GetInstance()->Draw();
}


void Play::Finitialize()
{
	ObjectManager::GetInstance()->AllEraseObject();
}

Scene* Play::GetNextScene()
{
	if (playSceneState == PLAY_SCENE_GAMECLEAR)
		return new GameClear();
	else if (playSceneState == PLAY_SCENE_GAMEOVER)
		return new GameOver();
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