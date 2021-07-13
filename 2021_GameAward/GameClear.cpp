#include "GameClear.h"
#include"Fade.h"
#include"XInputManager.h"
#include"StageSelect.h"

#include"Game.h"

const int GameClear::S_RUNK_TIME[5] =
{
	12,
	12,
	30,
	20,
	15
};

const int GameClear::A_RUNK_TIME[5] =
{
	15,
	15,
	35,
	25,
	20
};
const int GameClear::B_RUNK_TIME[5] =
{
	20,
	20,
	40,
	30,
	25
};


int GameClear::stageNum = 0;
int GameClear::clearTime = 0;

Sprite2D GameClear::timeSprite[6];
Texture GameClear::timeTexture;

Sprite2D GameClear::rankSprite;
Texture GameClear::rankTexture;

Sprite2D GameClear::rankFreamSprite;
Texture GameClear::rankFreamTexture[5];

Vector2 GameClear::timeScale = Vector2(2, 2);

ObjModel GameClear::enemyModel;

void GameClear::LoadResources()
{
	enemyModel.LoadModel
	(
		"Resources/Model/MoveEnemy/MoveEnemy_Bone.obj",
		true,
		1,
		0
	);
	enemyModel.SetScale(0.1f,0);

	for (int i = 0; i < _countof(timeSprite); i++)
	{
		timeSprite[i].CreateSprite();
		timeSprite[i].SetScale(Vector2(2, 2));
	}
	timeTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

	rankSprite.CreateSprite();
	rankSprite.SetPosition(Vector2(730, 290));
	rankTexture.LoadSpriteTexture("Resources/Texture/rank.png");

	rankFreamSprite.CreateSprite();

	for (int i = 0; i < 5; i++)
	{
		rankFreamTexture[i].LoadSpriteTexture("Resources/Texture/rankFream_" + std::to_string(i + 1) + ".png");
	}
}

void GameClear::Initialize()
{
	rankFreamPosition = RANK_FREAM_STOP_POSITION + Vector2(0, -400);


	enemyPosition = ENEMY_STOP_POSITION + Vector3(0, 3.0f, 0);
	enemyModel.SetPosition(enemyPosition, 0);
}

void GameClear::Update()
{
	auto NextState = [&](const int changeTime, const ResultState& nextState)
	{
		nextStateTimer.SetStopFlag(false);
		nextStateTimer.SetMaxTime(changeTime);
		if (nextStateTimer.GetSameAsMaximumFlag())
		{
			nextStateTimer.SetStopFlag(true);
			resultState = nextState;
		}
	};

	switch (resultState)
	{
	case GameClear::ResultState::MOVE_RESULT:

		if (rankFreamPosition.y <= RANK_FREAM_STOP_POSITION.y) 
		{
			rankFreamPosition.y += RANK_FREAM_SPEED;
			rankFreamSprite.SetPosition(rankFreamPosition);
		}
		else
		{
			rankFreamSprite.SetPosition(RANK_FREAM_STOP_POSITION);
			NextState(40, ResultState::SET_ENEMY);
		}

		break;
	case GameClear::ResultState::SET_ENEMY:
		if (enemyPosition.y >= ENEMY_STOP_POSITION.y)
		{
			enemyPosition.y -= ENEMY_MOVE_SPEED;
			enemyModel.SetPosition(enemyPosition,0);
		}
		else
		{
			enemyModel.SetPosition(ENEMY_STOP_POSITION, 0);
			NextState(40, ResultState::ADD_TIME);
		}
		break;
	case GameClear::ResultState::ADD_ENEMY_VALUE:
		break;
	case GameClear::ResultState::ADD_TIME:
		//タイムの増加
		if (drawTime != clearTime)drawTime++;

		//ランク決定
		if (drawTime < S_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_S;
		}
		else if (drawTime < A_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_A;
		}
		else if (drawTime < B_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_B;
		}
		else
		{
			rank = StageRank::RANK_C;
		}
		break;
	default:
		break;
	}

	
	//敵回転
	enemyAngle += ENEMY_ROT_SPEED;
	enemyModel.SetAngle(Vector3(0, enemyAngle, 0),0);


	if (XInputManager::GetPadConnectedFlag(1)
		&& (XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)
			|| XInputManager::ButtonTrigger(XInputManager::XINPUT_A_BUTTON, 1)))
	{
		if (Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT)
		{
			Fade::GetInstance()->FadeStart();
		}
	}
	if (Fade::GetInstance()->GetSceneChangeTimingFlag())
	{
		isEnd = true;
	}
}

void GameClear::Draw()
{
	//フレーム
	rankFreamSprite.Draw(&rankFreamTexture[stageNum]);


	//数字
	std::string drawStr = std::to_string(drawTime);
	int keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		std::string str = drawStr.substr(keta - 1 - i, 1);
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40.0f * (timeScale.x * 1.2f) * (i + 1) - 120, 340);

		int n = atoi(str.c_str());
		timeSprite[i].SetPosition(pos);
		timeSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &timeTexture);
	}

	//ランク
	rankSprite.SelectDrawAreaDraw(Vector2(200 * (int)rank, 0), Vector2(200 * ((int)rank + 1), 200), &rankTexture);

	//敵
	enemyModel.Draw(0);

	Fade::GetInstance()->Draw();
}

void GameClear::Finitialize()
{
	Library::StopLoadSound("Play", false);
}

Scene* GameClear::GetNextScene()
{
	return new StageSelect();
}
