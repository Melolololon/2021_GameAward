#include "GameClear.h"
#include"Fade.h"
#include"XInputManager.h"
#include"StageSelect.h"
#include"Enemy.h"

#include"Game.h"
#include"Random.h"


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

const Vector2 GameClear::NumberData::NUMBER_MIN_SCALE = 2.0f;
const Vector2 GameClear::NumberData::NUMBER_MAX_SCALE = NUMBER_MIN_SCALE + 3.0f;
const float GameClear::NumberData::NUMBER_SCALLING_SPEED = 0.3f;
const int GameClear::NumberData::NUMBER_ADD_START_TIME = 60 * 0.5f;
Texture GameClear::NumberData::numberTexture;

int GameClear::stageNum = 0;
int GameClear::clearTime = 0;




Sprite2D GameClear::rankSprite;
Texture GameClear::rankTexture;

Sprite2D GameClear::resultFreamSprite;
Texture GameClear::resultFreamTexture;

Sprite2D GameClear::crossSprite;
Texture GameClear::crossTexture;

ObjModel GameClear::enemyModel;

Sprite2D GameClear::treeSprite[TREE_NUM_Y][TREE_NUM_X];
Texture GameClear::treeTexture[4];

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

	crossSprite.CreateSprite();
	crossTexture.LoadSpriteTexture("Resources/Texture/rank_X.png");

	NumberData::LoadTexture();

	rankSprite.CreateSprite();
	rankTexture.LoadSpriteTexture("Resources/Texture/rank.png");

	resultFreamSprite.CreateSprite();


	resultFreamTexture.LoadSpriteTexture("Resources/Texture/resultFream.png");


	//for(int y = 0; y < _countof(treeSprite);y++)
	//{
	//	for (int x = 0; x < _countof(treeSprite[0]); x++) 
	//	{
	//		treeSprite[y][x].CreateSprite();
	//		treeSprite[y][x].SetPosition(Vector2(180 * x - 150, 180 * y - 150));
	//	}
	//}
	//for(int i = 0; i < _countof(treeTexture);i++)
	//{
	//	treeTexture[i].LoadSpriteTexture("Resources/Texture/Tree" + std::to_string(i) + ".png");
	//}
}

void GameClear::Initialize()
{
	enemyModel.SetPosition(ENEMY_STOP_POSITION, 0);

	resultFreamPosition = RESULT_FREAM_STOP_POSITION + Vector2(0, -650);
	resultFreamSprite.SetPosition(0);

	timeNumberData.SetMaxNum(clearTime);
	enemyDeadCountData.SetMaxNum(Enemy::GetDeadCount());


	rankSprite.SetPosition(rankResultData.GetPosition());
	rankSprite.SetScale(0);

	crossSprite.SetPosition(crossResultData.GetPosition());
	crossSprite.SetScale(0);

	/*for (int y = 0; y < _countof(treeSprite); y++)
	{
		for (int x = 0; x < _countof(treeSprite[0]); x++)
		{
			treeTextureNumber[y][x] = Random::GetRandomNumber(4);
		}
	}*/
}

void GameClear::Update()
{
	//シーン切り替え処理
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

	/*	if (resultFreamPosition.y <= RESULT_FREAM_STOP_POSITION.y) 
		{
			resultFreamPosition.y += RESULT_FREAM_SPEED;
			resultFreamSprite.SetPosition(resultFreamPosition);
		}
		else
		{
			resultFreamSprite.SetPosition(RESULT_FREAM_STOP_POSITION);
			NextState(40, ResultState::SET_ENEMY);
		}*/
		NextState(40, ResultState::SET_ENEMY);
		break;
	case GameClear::ResultState::SET_ENEMY:
		//if (enemyPosition.y >= ENEMY_STOP_POSITION.y)
		//{
		//	enemyPosition.y -= ENEMY_MOVE_SPEED;
		//	enemyModel.SetPosition(enemyPosition,0);
		//}
		//else
		//{
		//	enemyModel.SetPosition(ENEMY_STOP_POSITION, 0);
		//	NextState(40, ResultState::ADD_ENEMY_VALUE);
		//}
		NextState(10, ResultState::ADD_ENEMY_VALUE);
		break;
	case GameClear::ResultState::ADD_ENEMY_VALUE:
		if (enemyDeadCountData.ProcessEnd()) NextState(40, ResultState::ADD_TIME);
		enemyDeadCountData.Update();

		crossResultData.Update();
		crossSprite.SetScale(crossResultData.GetScale());
		crossSprite.SetSubColor(Color(0, 0, 0, crossResultData.GetSubAlpha()));
		break;
	case GameClear::ResultState::ADD_TIME:
		
		if(timeNumberData.ProcessEnd()) NextState(40, ResultState::PROCESS_END);
		timeNumberData.Update();

		rankResultData.Update();
		rankSprite.SetScale(rankResultData.GetScale());
		rankSprite.SetSubColor(Color(0,0,0,rankResultData.GetSubAlpha()));


		//ランク決定
		if (timeNumberData.GetDrawTime() < S_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_S;
		}
		else if (timeNumberData.GetDrawTime() < A_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_A;
		}
		else if (timeNumberData.GetDrawTime() < B_RUNK_TIME[stageNum])
		{
			rank = StageRank::RANK_B;
		}
		else
		{
			rank = StageRank::RANK_C;
		}

		break;

	case GameClear::ResultState::PROCESS_END:
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
		break;
	}

	
	//敵回転
	enemyAngle += ENEMY_ROT_SPEED;
	enemyModel.SetAngle(Vector3(0, enemyAngle, 0),0);


	
}

void GameClear::Draw()
{
	//木
	//for (int y = 0; y < _countof(treeSprite); y++)
	//{
	//	for (int x = 0; x < _countof(treeSprite[0]); x++)
	//	{
	//		treeSprite[y][x].Draw(&treeTexture[treeTextureNumber[y][x]]);
	//	}
	//}


	//フレーム
	resultFreamSprite.Draw(&resultFreamTexture);



	//タイム
	timeNumberData.Draw();
	//倒した数
	enemyDeadCountData.Draw();
	//掛ける
	//crossSprite.Draw(&crossTexture);

	//ランク
	float RANK_TEXTURE_SIZE_Y = rankTexture.GetTextureSize().y;
	rankSprite.SelectDrawAreaDraw
	(
		Vector2(RANK_TEXTURE_SIZE_Y * (int)rank, 0), 
		Vector2(RANK_TEXTURE_SIZE_Y * ((int)rank + 1), RANK_TEXTURE_SIZE_Y), 
		&rankTexture
	);

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


GameClear::NumberData::NumberData(const Vector2& pos) :
	resultData(pos, NUMBER_MIN_SCALE, NUMBER_MAX_SCALE, NUMBER_SCALLING_SPEED)
	, drawNum(0)
{
	for (int i = 0; i < NUMBER_DIGIT; i++)
	{
		numberSprite[i].CreateSprite();
		numberSprite[i].SetScale(0);
	}
}

void GameClear::NumberData::Update()
{
	resultData.Update();

	for (int i = 0; i < NUMBER_DIGIT; i++)
	{
		numberSprite[i].SetScale(resultData.GetScale());
	}
	numberSprite[0].SetSubColor(Color(0, 0, 0, Color::ToPar(resultData.GetSubAlpha())));

	if (resultData.GetProcessEnd())
	{
		if (addStartTimer.GetTime() == NUMBER_ADD_START_TIME)
		{
			addStartTimer.SetStopFlag(true);

			if (!ProcessEnd()) drawNum++;
		}
		else
		{
			addStartTimer.SetStopFlag(false);
		}
	}
}

bool GameClear::NumberData::ProcessEnd()
{
	if (addStartTimer.GetTime() == NUMBER_ADD_START_TIME
		&& maxNum == drawNum) return true;
	return false;
}

void GameClear::NumberData::Draw()
{
	std::string drawStr = std::to_string(drawNum);
	int keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		
		std::string str = drawStr.substr(keta - 1 - i, 1);
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40.0f * (NUMBER_MIN_SCALE.x * 1.2f) * (i + 1) + resultData.GetPosition().x - 20 * keta, resultData.GetPosition().y);

		int n = atoi(str.c_str());
		numberSprite[i].SetPosition(pos);
		numberSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &numberTexture);

	}

}

void GameClear::NumberData::LoadTexture()
{
	numberTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

}

void GameClear::ResultData::Update()
{
	if (scale.x > minScale.x)
	{
		scale -= scalingSpeed;
		

		float alphaDecNum = 100 / ((maxScale.x - minScale.x) / scalingSpeed);
		subAlpha -= alphaDecNum;
		
	}
	else
	{
		scale = minScale;
		subAlpha = 0.0f;
		processEnd = true;
	}
}
