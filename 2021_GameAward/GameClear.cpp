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
const float GameClear::NumberData::NUMBER_ADD_START_TIME = 60 * 0.5f;
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

	timeNumberData.SetMaxNum(40);
}

void GameClear::Update()
{
	//ÉVÅ[ÉìêÿÇËë÷Ç¶èàóù
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
		
		if(timeNumberData.AddEnd()) NextState(40, ResultState::PROCESS_END);
		else timeNumberData.Update();

		//ÉâÉìÉNåàíË
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

	
	//ìGâÒì]
	enemyAngle += ENEMY_ROT_SPEED;
	enemyModel.SetAngle(Vector3(0, enemyAngle, 0),0);


	
}

void GameClear::Draw()
{
	//ÉtÉåÅ[ÉÄ
	rankFreamSprite.Draw(&rankFreamTexture[stageNum]);



	//É^ÉCÉÄ
	timeNumberData.Draw();

	//ì|ÇµÇΩêî
	/*drawStr = std::to_string(Enemy::GetDeadCount());
	keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		std::string str = drawStr.substr(keta - 1 - i, 1);
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40.0f * (timeScale.x * 1.2f) * (i + 1) - 120, 320);

		int n = atoi(str.c_str());
		enemyDeadCountSprite[i].SetPosition(pos);
		enemyDeadCountSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &numberTexture);

	}*/


	//ÉâÉìÉN
	rankSprite.SelectDrawAreaDraw(Vector2(200 * (int)rank, 0), Vector2(200 * ((int)rank + 1), 200), &rankTexture);

	//ìG
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
	scale(NUMBER_MAX_SCALE),
	subAlpha(100.0f),
	drawTime(0),
	position(pos)
{
	for (int i = 0; i < NUMBER_DIGIT; i++)
	{
		numberSprite[i].CreateSprite();
	}
	numberSprite[0].SetScale(scale);
	for (int i = 1; i < NUMBER_DIGIT; i++)
	{
		numberSprite[i].SetScale(NUMBER_MIN_SCALE);
	}
	numberSprite[0].SetSubColor(Color(0, 0, 0, Color::ToPar(subAlpha)));

}

void GameClear::NumberData::Update()
{
	//èkè¨
	if (scale.x >= NUMBER_MIN_SCALE.x)
	{
		scale -= NUMBER_SCALLING_SPEED;
		numberSprite[0].SetScale(scale);

		float alphaDecNum = 100 / ((NUMBER_MAX_SCALE.x - NUMBER_MIN_SCALE.x) / NUMBER_SCALLING_SPEED);
		subAlpha -= alphaDecNum;
		numberSprite[0].SetSubColor(Color(0, 0, 0, Color::ToPar(subAlpha)));
	}
	else
	{
		numberSprite[0].SetScale(NUMBER_MIN_SCALE);
		numberSprite[0].SetSubColor(Color(0, 0, 0, 0));

		addStartTimer.SetStopFlag(false);
		if (addStartTimer.GetTime() >= NUMBER_ADD_START_TIME)
		{
			addStartTimer.SetStopFlag(true);
			//ëùâ¡
			if (drawTime != clearTime)drawTime++;
			 
		}
	}
}

void GameClear::NumberData::Draw()
{
	std::string drawStr = std::to_string(drawTime);
	int keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		std::string str = drawStr.substr(keta - 1 - i, 1);
		Vector2 pos = Vector2(Game::WIN_WIDTH / 2 + 20 * keta - 40.0f * (scale.x * 1.2f) * (i + 1) - position.x, position.y);

		int n = atoi(str.c_str());
		numberSprite[i].SetPosition(pos);
		numberSprite[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &numberTexture);

	}

}

void GameClear::NumberData::LoadTexture()
{

	numberTexture.LoadSpriteTexture("Resources/Texture/TimeNumber.png");


}

