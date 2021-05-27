#include "GameClear.h"
#include"Fade.h"
#include"XInputManager.h"
#include"StageSelect.h"

#include"Game.h"

const int GameClear::S_RUNK_TIME[5] =
{
	20,
	20,
	20,
	20,
	20
};

const int GameClear::A_RUNK_TIME[5] =
{ 
	30,
	30,
	30,
	30,
	30
};
const int GameClear::B_RUNK_TIME[5] =
{
	40,
	40,
	40,
	40,
	40
};


int GameClear::stageNum = 0;
int GameClear::time = 0;

Sprite2D GameClear::timeSprite[6];
Texture GameClear::timeTexture;

Sprite2D GameClear::rankSprite;
Texture GameClear::rankTexture;

Sprite2D GameClear::rankFreamSprite;
Texture GameClear::rankFreamTexture;

Vector2 GameClear::timeScale = Vector2(2, 2);

void GameClear::LoadResources() 
{
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
	rankFreamSprite.SetPosition(Vector2(220, 80));
	rankFreamTexture.LoadSpriteTexture("Resources/Texture/rankFream.png");
}

void GameClear::Initialize()
{

	//ランク決定
	if(time < S_RUNK_TIME[stageNum])
	{
		rank = StageRank::RANK_S;
	}
	else if(time < A_RUNK_TIME[stageNum])
	{
		rank = StageRank::RANK_A;
	}
	else if (time < B_RUNK_TIME[stageNum])
	{
		rank = StageRank::RANK_B;
	}
	
}

void GameClear::Update()
{
	if (DirectInput::KeyTrigger(DIK_Z)
		|| XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)) 
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
	rankFreamSprite.Draw(&rankFreamTexture);


	//数字
	std::string drawStr = std::to_string(time);
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
	rankSprite.SelectDrawAreaDraw(Vector2(200 * rank, 0), Vector2(200 * (rank + 1), 200), &rankTexture);

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
