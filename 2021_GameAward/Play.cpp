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

#include"LibMath.h"
#include"StageSelect.h"
#pragma region オブジェクト
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

#pragma region スプライト

Sprite3D Play::targetLockSprite;
Texture Play::targetLockTexture;

Sprite2D Play::timerSprite[6];
Texture Play::timerTexture;

Sprite2D Play::hpAnimationSprite;
Texture Play::hpAnimationTexture;

#pragma endregion


Play::PlaySceneState Play::playSceneState;
Play::Play()
{
}


Play::~Play() {}

void Play::LoadResources()
{
	//リソースの読み込み
	/*Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");*/
	arrowSprite.CreateSprite();
	arrowTexture.LoadSpriteTexture("Resources/Texture/arrow.png");


	//スプライト
	targetLockSprite.CreateSprite({ 10,10 });
	targetLockTexture.LoadSpriteTexture("Resources/Texture/lock.png");
	targetLockSprite.SetBillboardFlag(true, true, true);

	for (int i = 0; i < _countof(timerSprite); i++)
	{
		timerSprite[i].CreateSprite();
	}
	timerTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

	hpAnimationSprite.CreateSprite();
	hpAnimationTexture.LoadSpriteTexture("Resources/Texture/hpAnimation.png");
	hpAnimationSprite.SetPosition(Vector2(0, 0));
}


void Play::Initialize()
{
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);


	//敵追加
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

#pragma region カメラ
	addCameraPosition = { 0,60,-30 };
	cameraPosition = addCameraPosition;
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region ブロックセット
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::GetInstance()->AddObject(std::make_shared<Block>(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region 祠セット
	targetObjects.resize(targetNumber);
	for (auto& t : targetObjects)
	{

		//座標セット
		//ここ範囲乱数作って変える
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


#pragma region 矢印
	arrowPosition = 0.0f;
	arrowAngle = 0.0f;
	drawArrow = false;
#pragma endregion

#pragma region タイマー


	gameTime.SetMaxTime(INT_MAX);
	gameTime.SetNowTime(-60 * 4);
	gameTime.SetStopFlag(false);

	sceneEndTimer.SetMaxTime(SCENE_END_TIME);

	hpAnimationTimer.SetMaxTime(ANIMATION_ONE_FREAM_TIME * 8);
	hpAnimationTimer.SetStopFlag(false);
#pragma endregion
}

void Play::Update()
{
#pragma region ポーズ
	if (XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_START_BUTTON ,1))
		isPause = isPause == false ? true : false;

	FreamTimer::SetAllTimerStopFlag(isPause);
	
	if (isPause) 
		return;
	
#pragma endregion


	ObjectManager::GetInstance()->Update();

#pragma region カメラ移動
	Vector3 pHeapPos = player->GetHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += addCameraPosition;
	cameraTarget = pHeapPos;

	Vector3 upVector = LibMath::OtherVector(cameraPosition, cameraTarget);
	upVector = LibMath::RotateVector3(upVector, { 1,0,0 }, 90);
	Library::SetCamera(cameraPosition, cameraTarget, upVector);

#pragma endregion

#pragma region スプライト


#pragma region 祠を示す矢印
	//一番近い祠を求める
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
	//祠あったら計算
	if (minDistance < 9999999.0f)
	{
		//Vector2 arrowTexSize = Library::GetTextureSize(arrowTexture);
		Vector2 arrowTexSize = arrowTexture.GetTextureSize();

		//プレイヤーから一番近いターゲットのベクトルを正規化したもの
		Vector3 playerToTargetVector = nearTargetPos - playerHeadPos;
		Vector3 playerToTargetNVector = Vector3Normalize(playerToTargetVector);

		//クォータニオンで回す?
		//座標
		arrowPosition = { 1280.0f / 2.0f - arrowTexSize.x / 2, 720.0f / 2.0f - arrowTexSize.y / 2 };
		//祠の方に移動させる
		arrowPosition.x += playerToTargetNVector.x * (1280 / 2);
		arrowPosition.y -= playerToTargetNVector.z * (720 / 2);

		//画面内に収まるようにする
		if (arrowPosition.x >= 1280 - arrowTexSize.x)
			arrowPosition.x = 1280 - arrowTexSize.x;
		if (arrowPosition.x <= 0)
			arrowPosition.x = 0;
		if (arrowPosition.y >= 720 - arrowTexSize.y)
			arrowPosition.y = 720 - arrowTexSize.y;
		if (arrowPosition.y <= 0)
			arrowPosition.y = 0;

		//角度
		arrowAngle = LibMath::Vecto2ToAngle({ playerToTargetNVector.x,playerToTargetNVector.z }, false);

		//描画するかどうか
		drawArrow = false;
		if (abs(playerToTargetVector.x) >= 50.0f ||
			abs(playerToTargetVector.z) >= 30.0f)
			drawArrow = true;
	}

	arrowSprite.SetAngle(arrowAngle);
	arrowSprite.SetPosition(arrowPosition);
#pragma endregion

#pragma region ターゲット
	int playerTargetNum = player->GetLockTargetNum();
	if (playerTargetNum != -1)
	{
		Vector3 lockPos = targetObjects[playerTargetNum]->GetPosition()
			+ Vector3(0, 2.5, -1.5);
		targetLockSprite.SetPosition(lockPos);
	}
#pragma endregion



#pragma endregion

#pragma region 祠処理

	//	//祠セット
	//	//座標の値が変わってないのにワープしてたのは、
	//	//生成を繰り返してヒープの番号が被った時に、元から置かれてた祠のモデルの座標が消えるやつの座標で上書きされたから
	if (playSceneState == PlaySceneState::PLAY_SCENE_SET_TARGET)
	{
		int createMissCount = 0;
		for (auto& t : targetObjects)
		{
			//nullptrは飛ばす
			if (!t)continue;
			//設置完了済は飛ばす
			if (t->getSetEnd())continue;

			//ブロックとかプレイヤーと重なったたらカウント
			if (t->getCreateHitObject())
			{
				createMissCount++;
				continue;
			}

			//距離測定
			bool distanceCheck = true;//距離の基準を満たしたかどうか
			float dis;
			for (auto& t2 : targetObjects)
			{
				//ターゲット同士
				if (!t2 || t == t2)continue;
				dis = LibMath::CalcDistance3D
				(
					t->GetPosition(),
					t2->GetPosition()
				);

				//最低ライン超えられなかったらカウント&即抜け
				if (dis <= targetDistance)
				{
					createMissCount++;
					distanceCheck = false;
					break;
				}


			}


			//プレイヤー
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

			//距離の基準を満たしたら設置完了
			if (distanceCheck)
				t->trueSetEnd();

		}


#pragma region 祠座標再セット
		for (auto& t : targetObjects)
		{
			//設置完了済は飛ばす
			if (t->getSetEnd())continue;

			//座標セット
			//ここ範囲乱数作って変える
			Vector3 targetPos =
			{
				Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->setPosition(targetPos);

		}

#pragma endregion

		//カウントされなかったらセット終了
		if (createMissCount == 0)
		{
			//仮にこうしてる
			playSceneState = PlaySceneState::PLAY_SCENE_START_PREVIOUS;
		}
	}
	else if (playSceneState == PlaySceneState::PLAY_SCENE_START_PREVIOUS)
	{
		if (gameTime.GetTime() >= 0)
		{
			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
		}
	}
	else//祠生き残り確認(クリア判定用)
	{
		auto targetObjectsSize = targetObjects.size();

		std::vector<Vector3>targetObjectPos;
		targetObjectPos.reserve(targetObjectsSize);
		for (int i = 0; i < targetObjectsSize; i++)
		{
			//trueだったら消す
			if (targetObjects[i]->GetEraseManager())
			{
				//デバッグ用
				if (i == targetObjectsSize - 1)
					int z = 0;

				targetObjects.erase(targetObjects.begin() + i);
				targetObjectsSize--;
				i--;
				int a = 0;
			}
			else//falseだったら追加
				targetObjectPos.push_back(targetObjects[i]->GetPosition());
		}

		//ターゲットの座標をプレイヤーに渡す
		player->SetTargetPosition(targetObjectPos);

	}

#pragma endregion


	//終了処理
	if (targetObjects.size() == 0)
	{
		sceneEndTimer.SetStopFlag(false);
		playSceneState = PlaySceneState::PLAY_SCENE_GAMECLEAR;
	}
	if (sceneEndTimer.GetSameAsMaximumFlag())
		isEnd = true;
}

void Play::Draw()
{
	ObjectManager::GetInstance()->Draw();

	int playerLockTargetNum = player->GetLockTargetNum();
	if (playerLockTargetNum != -1)
		targetLockSprite.Draw(&targetLockTexture);

#pragma region ゲームタイマー
	int drawNum = gameTime.GetTime() / 60;
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

	int playerHP = player->GetHp();
	if (playerHP > 0) 
	{
		if (hpAnimationTimer.GetMultipleTimeFlag(ANIMATION_ONE_FREAM_TIME))
			hpAnimationNum++;
		if (hpAnimationTimer.GetSameAsMaximumFlag())
			hpAnimationNum = 0;

		const Vector2 textureSize = hpAnimationTexture.GetTextureSize();
		hpAnimationSprite.SelectDrawAreaDraw
		(
			Vector2(textureSize.x / 8 * hpAnimationNum, 0),
			Vector2(textureSize.x / 8 * (hpAnimationNum + 1), textureSize.y),
			&hpAnimationTexture
		);
	}
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