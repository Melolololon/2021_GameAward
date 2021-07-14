#include "GameClear.h"
#include"Fade.h"
#include"XInputManager.h"
#include"StageSelect.h"
#include"Enemy.h"

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

const Vector2 GameClear::NumberData::NUMBER_MIN_SCALE = 2.0f;
const Vector2 GameClear::NumberData::NUMBER_MAX_SCALE = NUMBER_MIN_SCALE + 3.0f;
const float GameClear::NumberData::NUMBER_SCALLING_SPEED = 0.3f;
const int GameClear::NumberData::NUMBER_ADD_START_TIME = 60 * 0.5f;
Texture GameClear::NumberData::numberTexture;

int GameClear::stageNum = 0;
int GameClear::clearTime = 0;




Sprite2D GameClear::rankSprite;
Texture GameClear::rankTexture;

Sprite2D GameClear::rankFreamSprite;
Texture GameClear::rankFreamTexture[5];

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

	NumberData::LoadTexture();

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
	enemyPosition = ENEMY_STOP_POSITION + Vector3(0, 3.0f, 0);
	enemyModel.SetPosition(enemyPosition, 0);


	rankFreamPosition = RANK_FREAM_STOP_POSITION + Vector2(0, -400);

	timeNumberData.SetMaxNum(clearTime);
	enemyDeadCountData.SetMaxNum(Enemy::GetDeadCount());


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
			NextState(40, ResultState::ADD_ENEMY_VALUE);
		}
		break;
	case GameClear::ResultState::ADD_ENEMY_VALUE:
		if (enemyDeadCountData.ProcessEnd()) NextState(40, ResultState::ADD_TIME);
		enemyDeadCountData.Update();

		break;
	case GameClear::ResultState::ADD_TIME:
		
		if(timeNumberData.ProcessEnd()) NextState(40, ResultState::PROCESS_END);
		timeNumberData.Update();

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
	//フレーム
	rankFreamSprite.Draw(&rankFreamTexture[stageNum]);



	//タイム
	timeNumberData.Draw();
	//倒した数
	enemyDeadCountData.Draw();


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
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40.0f * (resultData.GetScale().x * 1.2f) * (i + 1) - resultData.GetPosition().x, resultData.GetPosition().y);

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
