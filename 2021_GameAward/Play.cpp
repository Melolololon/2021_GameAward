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

#pragma region オブジェクト
#include"Block.h"


#pragma endregion


float Play::targetDistance;
float Play::playerDistance;
int Play::targetNumber;
Vector3 Play::leftUpPosition;
Vector3 Play::rightDownPosition;
std::vector<Vector3> Play::blockPositions;
std::vector<Vector3> Play::blockScales;
int Play::stageNum = 0;

#pragma region スプライト

Sprite2D Play::tutorialMessageSpr;
Texture Play::tutorialMessageTex[4];
Sprite2D Play::tutorialSkipSpr;
Texture Play::tutorialSkipTex; 
Sprite2D Play::tutorialNextSpr;
Texture Play::tutorialNextTex;

Sprite2D Play::arrowSprite;
Texture Play::arrowTexture;

Sprite3D Play::targetLockSprite;
Texture Play::targetLockTexture;

Sprite2D Play::targetNumSprite;
Sprite2D Play::targetCrossSprite;
Texture Play::targetCrossTexture;

Sprite2D Play::timerSprite[6];
Texture Play::timerTexture;

Sprite2D Play::startTimeSpr;
Sprite2D Play::startSpr;
Texture Play::startTex;


Sprite2D Play::targetAnimationSprite;
Texture Play::targetAnimationTexture;

Sprite2D Play::pauseSpr;
Texture Play::pauseTex;


#pragma endregion
bool Play::isPause = false;
Play::TutorialState Play::tutorialState = TutorialState::TUTORIAL_STATE_MOVE;

Play::PlaySceneState Play::playSceneState;

std::vector<std::vector<AStarNode>>Play::aStarNodes;


#ifdef _DEBUG
PrimitiveModel Play::nodeModel;

#endif // _DEBUG

Play::Play()
{
}


Play::~Play() {}

void Play::LoadResources()
{
#ifdef _DEBUG
	nodeModel.CreateBox(Vector3(1,1,1), Color(0, 0, 0, 255), 4000);

#endif // _DEBUG

	tutorialMessageSpr.CreateSprite();
	tutorialMessageSpr.SetPosition(Vector2(40, 480));
	for(int i = 0; i < 4;i++)
	{
		tutorialMessageTex[i].LoadSpriteTexture("Resources/Texture/Tutorial/tutorial_" + std::to_string(i + 1) + ".png");
	}

	tutorialSkipSpr.CreateSprite();
	tutorialSkipSpr.SetPosition(Vector2(878, 610));
	tutorialSkipSpr.SetScale(Vector2(0.8f,0.8f ));
	tutorialSkipTex.LoadSpriteTexture("Resources/Texture/Tutorial/tutorialSkip.png");

	tutorialNextSpr.CreateSprite();	
	tutorialNextSpr.SetPosition(Vector2(860, 520));
	tutorialNextSpr.SetScale(Vector2(0.8f, 0.8f));
	tutorialNextTex.LoadSpriteTexture("Resources/Texture/Tutorial/tutorialNext.png");

	/*Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");*/
	arrowSprite.CreateSprite();
	arrowTexture.LoadSpriteTexture("Resources/Texture/arrow.png");

	targetNumSprite.CreateSprite();
	targetNumSprite.SetPosition(Vector2(140, 120));

	targetCrossSprite.CreateSprite();
	targetCrossSprite.SetPosition(Vector2(70, 120));
	targetCrossSprite.SetScale(Vector2(0.4, 0.4));
	targetCrossTexture.LoadSpriteTexture("Resources/Texture/numCross.png");

	targetLockSprite.CreateSprite({ 10,10 });
	targetLockTexture.LoadSpriteTexture("Resources/Texture/lock.png");
	targetLockSprite.SetBillboardFlag(true, true, true);

	for (int i = 0; i < _countof(timerSprite); i++)
	{
		timerSprite[i].CreateSprite();
	}
	timerTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

	startTimeSpr.CreateSprite();
	startTimeSpr.SetPosition(Vector2(600, 300));
	startTimeSpr.SetScale(Vector2(4, 4));
	startSpr.CreateSprite();
	//startTex.LoadSpriteTexture();
	
	targetAnimationSprite.CreateSprite();
	targetAnimationTexture.LoadSpriteTexture("Resources/Texture/targetAnimation.png");
	targetAnimationSprite.SetPosition(Vector2(0,120));

	pauseSpr.CreateSprite();
	pauseSpr.SetPosition(Vector2(280, 100));
	pauseSpr.SetScale(Vector2(1.2f, 1.2f));

	pauseTex.LoadSpriteTexture("Resources/Texture/pause.png");

	Library::LoadSound("Resources/Sound/BGM/Map1BGM.wav", "Play", true);
}



void Play::Initialize()
{
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);
	pauseSnake = std::make_shared<Player>(0); 


	playSceneState = PlaySceneState::PLAY_SCENE_SET_TARGET;

#pragma region カメラ
	addCameraPosition = { 0,70,-35 };
	cameraPosition = addCameraPosition;
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region ブロックセット
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::GetInstance()->AddObject(std::make_shared<Block>(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region 矢印
	arrowPosition = 0.0f;
	arrowAngle = 0.0f;
	drawArrow = false;
#pragma endregion

#pragma region タイマー


	gameTime.SetMaxTime(INT_MAX);
	gameTime.SetNowTime(-60 * 3);


	sceneEndTimer.SetMaxTime(SCENE_END_TIME);


	targetAnimationTimer.SetMaxTime(TARGET_ANIMATION_ONE_FREAM_TIME * 4);
	targetAnimationTimer.SetStopFlag(false);


#pragma endregion

	Library::PlayLoadSound("Play");
	isPause = false;

	if (tutorialState != TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		playSceneState = PlaySceneState::PLAY_SCENE_START_PREVIOUS;
		gameTime.SetStopFlag(false);
		return;
	}
	else
	{
		Fade::GetInstance()->SetIsStopFlag(true);
	}




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


	SetEnemy();


#pragma region 経路関係

	//ノードセット
	Vector2 leftUpPositionV2 = Vector2(leftUpPosition.x, leftUpPosition.z);
	Vector2 rightDownPositionV2 = Vector2(rightDownPosition.x, rightDownPosition.z);;
	Vector2 nodeNum = Vector2(20, 20);
	LibMath::SetAStarNodePosition
	(
		leftUpPositionV2,
		rightDownPositionV2,
		nodeNum.x,
		nodeNum.y,
		aStarNodes,
		true
	);	

	//マスのサイズを求める
	Vector2 size = rightDownPositionV2 - leftUpPositionV2;
	size.x = abs(size.x);
	size.y = abs(size.y);
	if (nodeNum.x >= 1) size.x /= nodeNum.x - 1;
	if (nodeNum.y >= 1) size.y /= nodeNum.y - 1;




	//当たり判定
	std::vector<Vector2>blockPositionVec2(blockNum);
	std::vector<Vector2>blockSlaceVec2(blockNum);
	for(int i = 0; i < blockNum;i++)
	{
		blockPositionVec2[i].x = blockPositions[i].x;
		blockPositionVec2[i].y = blockPositions[i].z;
	
		blockSlaceVec2[i].x = blockScales[i].x;
		blockSlaceVec2[i].y = blockScales[i].z;
	}

	LibMath::SetAStarNodeHitObjectNodeFlag
	(
		blockPositionVec2,
		blockSlaceVec2,
		aStarNodes
	);




#ifdef _DEBUG
	for(int y = 0; y < aStarNodes.size();y++)
	{
		for (int x = 0; x < aStarNodes[0].size(); x++)
		{
			Vector3 pos = Vector3(aStarNodes[y][x].position.x, 0, aStarNodes[y][x].position.y);
			nodeModel.SetPosition(pos, x + y * aStarNodes[0].size());
			nodeModel.SetScale(Vector3(size.x - 0.2, 1, size.y - 0.2), x + y * aStarNodes[0].size());
			if (aStarNodes[y][x].hitObjectNode) 
			{
				nodeModel.AddColor(Color(255, 0, 0, 0), x + y * aStarNodes[0].size());
			}
		}
	}

#endif // _DEBUG

#pragma endregion

}

void Play::Update()
{
	if (Fade::GetInstance()->GetSceneChangeTimingFlag())
	{
		isEnd = true;
	}

#pragma region ポーズ
	FreamTimer::SetAllTimerStopFlag(isPause);

	//押したらポーズ
	if (XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_START_BUTTON, 1)
		&& Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT
		&& tutorialState == TutorialState::TUTORIAL_STATE_NOT_TUTORIAL
		&& playSceneState != Play::PLAY_SCENE_SET_TARGET
		&& playSceneState != Play::PLAY_SCENE_GAMECLEAR
		&& playSceneState != Play::PLAY_SCENE_GAMEOVER)
	{
		isPause = isPause == false ? true : false;
		backStageSelect = false;
	}

	if (isPause) 
	{
		pauseSnake->Update();

		//選択
		float stickAngle = 0.0f;
		if(XInputManager::GetPadConnectedFlag(1))
		{
			stickAngle = XInputManager::LeftStickAngle(1);
		}
		if(stickAngle >= 240
			&& stickAngle <= 300)
		{
			backStageSelect = true;
		}
		if (stickAngle >= 60
			&& stickAngle <= 120)
		{
			backStageSelect = false;
		}

		//ボタン
		if(XInputManager::GetPadConnectedFlag(1))
		{
			if(XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON,1)
				|| XInputManager::ButtonTrigger(XInputManager::XINPUT_A_BUTTON, 1))
			{
				if(backStageSelect)
				{
					Fade::GetInstance()->FadeStart();
				}
				else
				{
					isPause = false;
				}
			}
		}


		//蛇(カーソル)移動
		if(backStageSelect)
		{
			pauseSnake->SetModelMoveVector(cameraTarget + Vector3(-26, 30, -22));
		}
		else
		{
			pauseSnake->SetModelMoveVector(cameraTarget + Vector3(-29, 30, -9));
		}

		return;
	}
	
#pragma endregion

	player->UpdateHpAnimation();

	//クリアのスロー
	if (!slowTimer.GetMultipleTimeFlag(2))
		return;

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

#pragma region 矢印


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
		if (abs(playerToTargetVector.x) >= 75.0f ||
			abs(playerToTargetVector.z) >= 55.0f)
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
	if (playSceneState == PlaySceneState::PLAY_SCENE_SET_TARGET
		&& tutorialState == TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		int createMissCount = 0;
		for (auto& t : targetObjects)
		{
			//nullptrは飛ばす
			if (!t)continue;
			//設置完了済は飛ばす
			if (t->GetSetEnd())continue;

			//ブロックとかプレイヤーと重なったたらカウント
			if (t->GetCreateHitObject())
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
				t->TrueSetEnd();

		}


#pragma region 祠座標再セット
		for (auto& t : targetObjects)
		{
			//設置完了済は飛ばす
			if (t->GetSetEnd())continue;

			//座標セット
			//ここ範囲乱数作って変える
			Vector3 targetPos =
			{
				Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->SetPosition(targetPos);

		}

#pragma endregion

		//カウントされなかったらセット終了
		if (createMissCount == 0)
		{
			//仮にこうしてる
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

				targetObjects.erase(targetObjects.begin() + i);
				targetObjectsSize--;
				i--;

				if (targetObjects.size() != 0) 
				{
					SetEnemy();
				}
			}
			else//falseだったら追加
			{
				targetObjectPos.push_back(targetObjects[i]->GetPosition());
			}
		}

		//ターゲットの座標をプレイヤーに渡す
		player->SetTargetPosition(targetObjectPos);

	}

#pragma endregion

	//チュートリアル
	Tutorial();

#pragma region 終了処理

	//クリア
	if (targetObjects.size() == 0 
		&& playSceneState != PlaySceneState::PLAY_SCENE_GAMEOVER
		&& tutorialState == TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		sceneEndTimer.SetStopFlag(false);
		slowTimer.SetStopFlag(false);
		gameTime.SetStopFlag(true);
		playSceneState = PlaySceneState::PLAY_SCENE_GAMECLEAR;
	}

	//ゲームオーバー
	if (player->GetIsDead()
		&& playSceneState != PlaySceneState::PLAY_SCENE_GAMECLEAR)
	{
		sceneEndTimer.SetStopFlag(false);
		//slowTimer.SetStopFlag(false);
		gameTime.SetStopFlag(true);
		playSceneState = PlaySceneState::PLAY_SCENE_GAMEOVER;
	}

	if (sceneEndTimer.GetSameAsMaximumFlag()) 
	{
		Fade::GetInstance()->FadeStart();
	}

	
#pragma endregion

}

void Play::Tutorial()
{
	if (playSceneState != PlaySceneState::PLAY_SCENE_PLAY
		|| tutorialState == TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)return;

	bool skipTimeOver = gameTime.GetTime() >= 60 * 1;
	//スキップ処理
	if(XInputManager::GetPadConnectedFlag(1)
		&& skipTimeOver)
	{
		if(XInputManager::ButtonTrigger(XInputManager::XINPUT_B_BUTTON,1))
		{
			Fade::GetInstance()->FadeStart();
		}
	}

	//次へ進める処理
	auto GetNextTutorialScene = [&skipTimeOver]()
	{
		if (XInputManager::GetPadConnectedFlag(1)
			&& skipTimeOver)
		{
			return XInputManager::ButtonTrigger(XInputManager::XINPUT_A_BUTTON, 1);
		}
		return false;
	};

	switch (tutorialState)
	{
	case Play::TUTORIAL_STATE_MOVE:
		
		if(GetNextTutorialScene())
		{
			tutorialState = TUTORIAL_STATE_SHOT;

			//敵追加
			//ObjectManager::GetInstance()->AddObject(tutorialEnemy);
		}

		break;

	case Play::TUTORIAL_STATE_SHOT:
		if (GetNextTutorialScene())
		{
			tutorialState = TUTORIAL_STATE_LOCK;

			//祠
			targetObjects.push_back(std::make_shared<TargetObject>(Vector3(0, 0, 0)));
			ObjectManager::GetInstance()->AddObject(targetObjects[0]);
		}
		break;

	case Play::TUTORIAL_STATE_LOCK:
		if (GetNextTutorialScene())
		{
			tutorialState = TUTORIAL_STATE_TWIST;
		}
		break;

	case Play::TUTORIAL_STATE_TWIST:
		if (GetNextTutorialScene())
		{
			Fade::GetInstance()->FadeStart();
		}
		break;
	}

	//if (DirectInput::KeyTrigger(DIK_A))
	//	isEnd = true;
	
}

void Play::Draw()
{

#ifdef _DEBUG
	if (tutorialState == Play::TutorialState::TUTORIAL_STATE_NOT_TUTORIAL) {
		for (int i = 0; i < aStarNodes.size() * aStarNodes[0].size(); i++)
		{

			nodeModel.Draw(i);
		}
	}

#endif // _DEBUG


	const float numTextureSizeY = timerTexture.GetTextureSize().y;
	auto DrawNumber = [&numTextureSizeY](Sprite2D& numSprite, const int number)
	{
		numSprite.SelectDrawAreaDraw
		(
			Vector2(numTextureSizeY * number, 0),
			Vector2(numTextureSizeY * (number + 1), numTextureSizeY),
			&timerTexture
		);
	};


	ObjectManager::GetInstance()->Draw();

	int playerLockTargetNum = player->GetLockTargetNum();
	if (playerLockTargetNum != -1)
		targetLockSprite.Draw(&targetLockTexture);


	//ターゲットHP
	for (auto& t : targetObjects)
		t->DrawHp();



#pragma region ゲームタイマー
	int drawNum = gameTime.GetTime() / 60;
	bool isMinus = gameTime.GetTime() < 0;
	if (isMinus) drawNum--;

	if (drawNum < 0)drawNum = 0;

	std::string drawStr = std::to_string(drawNum);
	int keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		std::string str = drawStr.substr(keta - 1 - i, 1);
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40 * (i + 1), 0);
		if (str == "-")
		{
			timerSprite[i].SetPosition(pos);
			timerSprite[i].SelectDrawAreaDraw(Vector2(10 * 80, 0), Vector2(10 * 80 + 80, 80), &timerTexture);
		}
		else
		{
			int n = atoi(str.c_str());
			timerSprite[i].SetPosition(pos);
			timerSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &timerTexture);
		}

		drawNum /= 10;
	}
#pragma endregion

	//321スタート

	if(gameTime.GetTime() < 60 * -2)
	{
		DrawNumber(startTimeSpr, 3);
	}
	else if(gameTime.GetTime() < 60 * -1)
	{
		DrawNumber(startTimeSpr, 2);
	}
	else if (gameTime.GetTime() < 60 * 0)
	{
		DrawNumber(startTimeSpr, 1);
	}
	
	//矢印
	if (drawArrow) 
	{
		arrowSprite.Draw(&arrowTexture);
	}

	//プレイヤーHP
	int playerHP = player->GetHp();

	//playerのHPを描画
	player->DrawHp();
	
	//祠
	
	//祠アニメーション
	if (targetAnimationTimer.GetMultipleTimeFlag(TARGET_ANIMATION_ONE_FREAM_TIME)
		&& !isPause)
		targetAnimationNum++;
	if (targetAnimationTimer.GetSameAsMaximumFlag())
		targetAnimationNum = 0;

	//祠掛ける
	targetCrossSprite.Draw(&targetCrossTexture);

	//数字
	const int targetNum = targetObjects.size();
	DrawNumber(targetNumSprite, targetNum);
	
	const Vector2 targetAnimationTextureSize = targetAnimationTexture.GetTextureSize();
	targetAnimationSprite.SelectDrawAreaDraw
	(
		Vector2(targetAnimationTextureSize.x / 4 * targetAnimationNum, 0),
		Vector2(targetAnimationTextureSize.x / 4 * (targetAnimationNum + 1), targetAnimationTextureSize.y),
		&targetAnimationTexture
	);


	if(isPause)
	{
		pauseSpr.Draw(&pauseTex);
		pauseSnake->Draw();
	}

	//チュートリアル
	if(playSceneState == PlaySceneState::PLAY_SCENE_PLAY
		&& tutorialState != TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		tutorialMessageSpr.Draw(&tutorialMessageTex[tutorialState - 1]);
		tutorialSkipSpr.Draw(&tutorialSkipTex);
		tutorialNextSpr.Draw(&tutorialNextTex);
	}

	Fade::GetInstance()->Draw();
}
 

void Play::Finitialize()
{
	if (backStageSelect
		|| tutorialState != TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		Library::StopLoadSound("Play", false);

	}

	if (tutorialState != TutorialState::TUTORIAL_STATE_NOT_TUTORIAL)
	{
		tutorialState = TutorialState::TUTORIAL_STATE_NOT_TUTORIAL;
	}

	FreamTimer::SetAllTimerStopFlag(false);

	ObjectManager::GetInstance()->AllEraseObject();
}

Scene* Play::GetNextScene()
{
	if (playSceneState == PLAY_SCENE_GAMECLEAR) 
	{
		int drawNum = gameTime.GetTime() / 60;
		bool isMinus = gameTime.GetTime() < 0;
		if (isMinus) drawNum--;

		GameClear::SetStageNum(stageNum);
		GameClear::SetTime(drawNum);

		return new GameClear();
	}
	else if (playSceneState == PLAY_SCENE_GAMEOVER) 
	{
		return new GameOver();
	}

	return new StageSelect();
}

void Play::SetEnemy()
{
	Vector3 playerPosition = player->GetPosition();

	//敵追加
	for (int i = 0; i < ENEMY_NUMBER[stageNum]; i++)
	{
		int range = 2;
		if (stageNum == 1)
		{
			range = 3;
		}
		else if (stageNum >= 2)
		{
			range = 4;
		}
		Play::EnemyType enemyType = (Play::EnemyType)Library::GetRandomNumber(range);
		if (enemyType == Play::EnemyType::ET_MoveEnemy) {

			std::shared_ptr<MoveEnemy> enemy = std::make_shared<MoveEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPGameTime(&gameTime);
			Vector3 random;
			bool b1 = false;
			const float PLAYERCOLLISIONRANGE = 50;
			do {
				random = Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z);
				Vector3 enemyLeftUp = random - Vector3(0.5, 0, 0.5);
				Vector3 enemyRightDown = random + Vector3(0.5, 0, 0.5);

				//プレイヤーに近すぎないように
				b1 = isCollision(Vector3(-PLAYERCOLLISIONRANGE+ playerPosition.x, 0, -PLAYERCOLLISIONRANGE + playerPosition.z), Vector3(PLAYERCOLLISIONRANGE + playerPosition.x, 0, PLAYERCOLLISIONRANGE + playerPosition.z), enemyLeftUp, enemyRightDown);
				if (b1) {
					continue;
				}

				for (int j = 0; j < blockPositions.size(); j++)
				{
					Vector3 blockLeftUp = blockPositions[j] - blockScales[j] / 2;
					Vector3 blockRightDown = blockPositions[j] + blockScales[j] / 2;
					//ブロックと重なってるかどうか
					b1 = isCollision(blockLeftUp, blockRightDown, enemyLeftUp, enemyRightDown);
					if (b1) break;
				}

			} while (b1);

			enemy->setPosition(random);
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_ShotEnemy) {
			std::shared_ptr<ShotEnemy> enemy = std::make_shared<ShotEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPGameTime(&gameTime);
			Vector3 random;
			bool b1 = false;
			const float PLAYERCOLLISIONRANGE = 50;
			do {
				random = Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z);
				Vector3 enemyLeftUp = random - Vector3(0.5, 0, 0.5);
				Vector3 enemyRightDown = random + Vector3(0.5, 0, 0.5);

				//プレイヤーに近すぎないように
				b1 = isCollision(Vector3(-PLAYERCOLLISIONRANGE + playerPosition.x, 0, -PLAYERCOLLISIONRANGE + playerPosition.z), Vector3(PLAYERCOLLISIONRANGE + playerPosition.x, 0, PLAYERCOLLISIONRANGE + playerPosition.z), enemyLeftUp, enemyRightDown);
				if (b1) {
					continue;
				}

				for (int j = 0; j < blockPositions.size(); j++)
				{
					Vector3 blockLeftUp = blockPositions[j] - blockScales[j] / 2;
					Vector3 blockRightDown = blockPositions[j] + blockScales[j] / 2;
					b1 = isCollision(blockLeftUp, blockRightDown, enemyLeftUp, enemyRightDown);
					if (b1) break;
				}
			} while (b1);
			enemy->setPosition(random);
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_FleeEnemy) {
			std::shared_ptr<FleeEnemy> enemy = std::make_shared<FleeEnemy>();
			enemy->setPPlayer(player.get());
			enemy->setPGameTime(&gameTime);
			Vector3 random;
			bool b1 = false;
			const float PLAYERCOLLISIONRANGE = 50;
			do {
				random = Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z);
				Vector3 enemyLeftUp = random - Vector3(0.5, 0, 0.5);
				Vector3 enemyRightDown = random + Vector3(0.5, 0, 0.5);

				//プレイヤーに近すぎないように
				b1 = isCollision(Vector3(-PLAYERCOLLISIONRANGE + playerPosition.x, 0, -PLAYERCOLLISIONRANGE + playerPosition.z), Vector3(PLAYERCOLLISIONRANGE + playerPosition.x, 0, PLAYERCOLLISIONRANGE + playerPosition.z), enemyLeftUp, enemyRightDown);
				if (b1) {
					continue;
				}

				for (int j = 0; j < blockPositions.size(); j++)
				{
					Vector3 blockLeftUp = blockPositions[j] - blockScales[j] / 2;
					Vector3 blockRightDown = blockPositions[j] + blockScales[j] / 2;
					b1 = isCollision(blockLeftUp, blockRightDown, enemyLeftUp, enemyRightDown);
					if (b1) break;
				}
			} while (b1);
			enemy->setPosition(random);
			enemy->Initialize();
			ObjectManager::GetInstance()->AddObject(enemy);
		}
		else if (enemyType == EnemyType::ET_SimEnemy) {
			std::shared_ptr<SimEnemy> enemy[3];

			Vector3 random;
			bool b1 = false;
			const float PLAYERCOLLISIONRANGE = 50;
			do {
				random = Vector3(Library::GetRandomNumber(rightDownPosition.x - leftUpPosition.x) + leftUpPosition.x, 0, Library::GetRandomNumber(leftUpPosition.z - rightDownPosition.z) + rightDownPosition.z);
				Vector3 enemyLeftUp = random - Vector3(0.5, 0, 0.5);
				Vector3 enemyRightDown = random + Vector3(0.5, 0, 0.5);

				//プレイヤーに近すぎないように
				b1 = isCollision(Vector3(-PLAYERCOLLISIONRANGE + playerPosition.x, 0, -PLAYERCOLLISIONRANGE + playerPosition.z), Vector3(PLAYERCOLLISIONRANGE + playerPosition.x, 0, PLAYERCOLLISIONRANGE + playerPosition.z), enemyLeftUp, enemyRightDown);
				if (b1) {
					continue;
				}

				for (int j = 0; j < blockPositions.size(); j++)
				{
					Vector3 blockLeftUp = blockPositions[j] - blockScales[j] / 2;
					Vector3 blockRightDown = blockPositions[j] + blockScales[j] / 2;
					b1 = isCollision(blockLeftUp, blockRightDown, enemyLeftUp, enemyRightDown);
					if (b1) break;
				}
			} while (b1);

			for (int j = 0; j < 3; j++)
			{
				enemy[j] = std::make_shared<SimEnemy>();
				enemy[j]->setPPlayer(player.get());
				enemy[j]->setPGameTime(&gameTime);
				enemy[j]->setPosition(random);
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


}

void Play::SetStageData
(
	const int stage,
	std::vector<Vector3>blockPos,
	std::vector<Vector3>blockScale,
	const float& targetDis,
	const float& playerDis,
	const int& targetNum,
	const Vector3& leftUpPos,
	const Vector3& rightDownPos
)
{
	stageNum = stage;
	blockPositions = blockPos;
	blockScales = blockScale;
	targetDistance = targetDis;
	playerDistance = playerDis;
	targetNumber = targetNum;
	leftUpPosition = leftUpPos;
	rightDownPosition = rightDownPos;

}

bool Play::isCollision(const Vector3& blockLeftUp, const Vector3& blockRightDown, const Vector3& enemyLeftUp, const Vector3& enemyRightDown)
{
	if (blockLeftUp.x > enemyRightDown.x) return false;
	if (blockRightDown.x < enemyLeftUp.x) return false;
	if (blockLeftUp.y > enemyRightDown.y) return false;
	if (blockRightDown.y < enemyLeftUp.y) return false;
	if (blockLeftUp.z > enemyRightDown.z) return false;
	if (blockRightDown.z < enemyLeftUp.z) return false;

	return true;

}
