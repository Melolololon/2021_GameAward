#include "Title.h"
#include"StageSelect.h"
#include"XInputManager.h"
#include"Fade.h"
#include"Play.h"
#include"StageSelect.h"
#include"GameOver.h"

std::unique_ptr<Sprite2D>Title::titleSprite[TITLE_CHAR_NUM];
std::unique_ptr<Texture>Title::titleTexture;

std::unique_ptr<Sprite2D>Title::pushButtonSprite;
std::unique_ptr<Texture>Title::pushButtonTexture;
FreamTimer Title::titleFreamTimer[TITLE_CHAR_NUM];
FreamTimer Title::titleTopStopTimer;
Vector2 Title::titleSpritePosition[TITLE_CHAR_NUM];

const float Title::TITLE_UP_DOWN_SPEED = 0.05f;

Sprite2D Title::titleBackSpr;
Texture Title::titleBackTex;

bool Title::nextSceneTutorial = true;

Title::Title(){}
Title::~Title(){}

void Title::LoadResources()
{
	for (int i = 0; i < _countof(titleSprite); i++) 
	{
		titleSprite[i] = std::make_unique<Sprite2D>();
		titleSprite[i]->CreateSprite();
		titleSprite[i]->SetScale(1.0f);
	}
	titleTexture = std::make_unique<Texture>();
	titleTexture->LoadSpriteTexture("Resources/Texture/MS_Title/title.png");


	pushButtonSprite = std::make_unique<Sprite2D>();
	pushButtonSprite->CreateSprite();
	pushButtonSprite->SetScale(0.8f);
	pushButtonSprite->SetPosition({ 140,380 });
	pushButtonTexture = std::make_unique<Texture>();
	pushButtonTexture->LoadSpriteTexture("Resources/Texture/MS_Title/titleStart.png");

	titleBackSpr.CreateSprite();
	titleBackTex.LoadSpriteTexture("Resources/Texture/MS_Title/titleBackTex.png");

	Library::LoadSound("Resources/Sound/BGM/SnakeTitle.wav", "Title", true);


}

void Title::Initialize()
{

	const float titleTextureSize = titleTexture->GetTextureSize().x / TITLE_CHAR_NUM;
	for (int i = 0; i < _countof(titleSpritePosition); i++)
	{
		titleSlowMove[i] = false;
		titleSpritePosition[i] = { 1280 + i * titleTextureSize ,0 };

	}

	pushButtonTimer.SetMaxTime(PUSH_BUTTON_SUB_ALPHA_MAX);
	pushButtonTimer.SetStopFlag(false);

	//BGMÄ¶
	Library::PlayLoadSound("Title");
}

void Title::Update()
{

	const float TITLE_TEX_SIZE_X = titleTexture->GetTextureSize().x / TITLE_CHAR_NUM;
	const float TITLE_SPEED = 10.0f;
	const float TITLE_STOP_POS_X = 130;
	
	bool stopPosTopChar = false;
	if (titleSpritePosition[0].x <= TITLE_STOP_POS_X)
	{
		stopPosTopChar = true;
		titleTopStopTimer.SetStopFlag(false);
	}
	
	for (int i = 0; i < _countof(titleSprite); i++)
	{
		float sinY = 0.0f;
		float titleHoseiPosY = 100;
		

		//‰æ–Ê“à‚É‚Í‚¢‚Á‚½‚çã‰ºˆÚ“®
		if (titleSpritePosition[i].x <= 1280 &&
			titleSpritePosition[i].x >= TITLE_STOP_POS_X)
		{
			sinY = sin(titleFreamTimer[i].GetTime() * TITLE_UP_DOWN_SPEED) * 130.0f;
			titleFreamTimer[i].SetStopFlag(false);
		}
	
		float sinCenterLinePosY = titleHoseiPosY;
		if (stopPosTopChar
			&& LibMath::Difference(titleHoseiPosY, titleSpritePosition[i].y,3.0f)
			&& !titleSlowMove[i])
		{
			if (i == 0) 
			{
				titleSlowMove[i] = true;
				titleFreamTimer[i].ResetTime();
			}
			else if(titleSlowMove[i - 1])
			{
				titleSlowMove[i] = true;
				titleFreamTimer[i].ResetTime();
			}
		}

		if (!stopPosTopChar) 
		{
			titleSpritePosition[i].x -= TITLE_SPEED;
		}


		if (titleSlowMove[i])
		{
			sinY = sin(titleFreamTimer[i].GetTime() * TITLE_UP_DOWN_SPEED) * 40.0f;
		}
		

		titleSpritePosition[i].y = titleHoseiPosY + sinY;
		titleSprite[i]->SetPosition(titleSpritePosition[i]);

	}

	const float PUSH_BUTTON_ALPHA_SPEED = 100.0 / (PUSH_BUTTON_SUB_ALPHA_MAX / 2);
	if(pushButtonTimer.GetTime() < PUSH_BUTTON_SUB_ALPHA_MAX / 2)
	{
		pushButtonSubAlpha += PUSH_BUTTON_ALPHA_SPEED;
	}
	else
	{
		pushButtonSubAlpha -= PUSH_BUTTON_ALPHA_SPEED;
	}
	pushButtonSprite->SetSubColor(Color(0, 0, 0, Color::ToPar(pushButtonSubAlpha)));


	bool padStart = (XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_X_BUTTON, 1)
		|| XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_A_BUTTON, 1)
		|| XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_START_BUTTON, 1))
		&& XInputManager::GetPadConnectedFlag(1);
	if (padStart)
	{
		if (Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT)
		{
			Fade::GetInstance()->FadeStart();
			Library::PlaySoundEveryLoad("Resources/Sound/SE/SystemSE/SneakTitleSe.wav");
		}
	}



	if (Fade::GetInstance()->GetSceneChangeTimingFlag()) isEnd = true;

}
void Title::Draw()
{
	titleBackSpr.Draw(&titleBackTex);

	const Vector2 titleTextureSize = Vector2(titleTexture->GetTextureSize().x / TITLE_CHAR_NUM, titleTexture->GetTextureSize().y);
	
	for (int i = 0; i < _countof(titleSprite); i++) 
	{
		titleSprite[i]->SelectDrawAreaDraw
		(
			Vector2(titleTextureSize.x * i, 0),
			Vector2(titleTextureSize.x * (i + 1), titleTextureSize.y),
			titleTexture.get()
		);
	}
	pushButtonSprite->Draw(pushButtonTexture.get());

	Fade::GetInstance()->Draw();
}

void Title::Finitialize()
{
	Library::StopLoadSound("Title", false);

	if (nextSceneTutorial) 
	{
		StageSelect::SetTutorialData();
	}

}

Scene* Title::GetNextScene()
{
	//ƒeƒXƒg
	return new GameOver();


	if (nextSceneTutorial) 
	{
		//nextSceneTutorial = false;
		Play::SetTutorialState(Play::TutorialState::TUTORIAL_STATE_MOVE);
		return new Play();
	}
	return new StageSelect();
}
