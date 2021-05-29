#include "Title.h"
#include"StageSelect.h"
#include"XInputManager.h"
#include"Fade.h"
#include"Play.h"

std::unique_ptr<Sprite2D>Title::titleSprite[7];
std::unique_ptr<Texture>Title::titleTexture;
std::unique_ptr<Sprite2D>Title::pushButtonSprite;
std::unique_ptr<Texture>Title::pushButtonTexture;
FreamTimer Title::titleFreamTimer[7];
Vector2 Title::titleSpritePosition[7];

Sprite2D Title::titleBackSpr;
Texture Title::titleBackTex;

Title::Title(){}
Title::~Title(){}

void Title::LoadResources()
{
	for (int i = 0; i < _countof(titleSprite); i++) 
	{
		titleSprite[i] = std::make_unique<Sprite2D>();
		titleSprite[i]->CreateSprite();
		titleSprite[i]->SetScale({ 3,3 });
	}
	titleTexture = std::make_unique<Texture>();
	titleTexture->LoadSpriteTexture("Resources/Texture/MS_Title/MS_Tex.png");


	pushButtonSprite = std::make_unique<Sprite2D>();
	pushButtonSprite->CreateSprite();
	pushButtonSprite->SetPosition({ 390,180 });
	pushButtonTexture = std::make_unique<Texture>();
	pushButtonTexture->LoadSpriteTexture("Resources/Texture/MS_Title/press_X_button.png");

	titleBackSpr.CreateSprite();
	titleBackTex.LoadSpriteTexture("Resources/Texture/MS_Title/titleBackTex.png");

	Library::LoadSound("Resources/Sound/BGM/SnakeTitle.wav", "Title", true);


}

void Title::Initialize()
{
		
	const float titleTextureSize = titleTexture->GetTextureSize().y;
	for (int i = 0; i < _countof(titleSpritePosition); i++)
	{
		titleSlowMove[i] = false;
		titleSpritePosition[i] = { 1280 + i * titleTextureSize * 3 ,0 };
	}

	//BGMÄ¶
	Library::PlayLoadSound("Title");
}

void Title::Update()
{

	const float titleTextureSize = titleTexture->GetTextureSize().y;
	float titleSpeed = 10.0f;
	for (int i = 0; i < _countof(titleSprite); i++) 
	{

		float sinY = 0.0f;
		//sinY = sin(titleFreamTimer[i].GetTime() * 0.05f) * 40.0f;

		float titleHoseiPosY = 100;
		float stopPosX = 230 + titleTextureSize * 3 * i;
		if (titleSpritePosition[i].x <= 1280 && 
			titleSpritePosition[i].x >= stopPosX)
		{
			sinY = sin(titleFreamTimer[i].GetTime() * 0.1f) * 60.0f;
			titleFreamTimer[i].SetStopFlag(false);
		}
		if (titleSpritePosition[i].x > stopPosX)
			titleSpritePosition[i].x -= titleSpeed;
		 

		if (titleSpritePosition[i].y >= -titleSpeed + titleHoseiPosY
			&& titleSpritePosition[i].y <= titleSpeed + titleHoseiPosY
			&& !titleSlowMove[i]
			&& titleSpritePosition[i].x <= stopPosX)
		{
			//titleSpritePosition[i].y = titleHoseiPosY;
			//titleFreamTimer[i].SetStopFlag(true);
			if (i == 0) 
			{
				
				titleSlowMove[i] = true;
				titleFreamTimer[i].ResetTime();
			}
			else
			{
				if (titleSlowMove[i - 1]
					&& !titleSlowMove[i])
				{
					titleSlowMove[i] = true;
					titleFreamTimer[i].ResetTime();
				}
			}
		}

		

		if(titleSlowMove[i])
	    	sinY = sin(titleFreamTimer[i].GetTime() * 0.05f) * 40.0f;

		titleSpritePosition[i].y = titleHoseiPosY + sinY;
		titleSprite[i]->SetPosition(titleSpritePosition[i]);

	}

	bool padStart = (XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_X_BUTTON, 1)
		|| XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_START_BUTTON, 1))
		&& XInputManager::GetPadConnectedFlag(1);
	if (DirectInput::KeyTrigger(DIK_Z)
		|| padStart)
	{
		if (Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT) 
		{
			Fade::GetInstance()->FadeStart();
			Library::PlaySoundEveryLoad("Resources/Sound/SE/SystemSE/SneakTitleSe.wav");
		}
	}

	if (Fade::GetInstance()->GetSceneChangeTimingFlag())
		isEnd = true;

}

void Title::Draw()
{
	titleBackSpr.Draw(&titleBackTex);

	const float titleTextureSize = titleTexture->GetTextureSize().y;
	for (int i = 0; i < _countof(titleSprite); i++)
		titleSprite[i]->SelectDrawAreaDraw
		(
			{ titleTextureSize * i,0 }, 
			{ titleTextureSize * (i + 1),titleTextureSize }, 
			titleTexture.get()
		);
	
	pushButtonSprite->Draw(pushButtonTexture.get());

	Fade::GetInstance()->Draw();
}

void Title::Finitialize()
{
	Library::StopLoadSound("Title", false);

	StageSelect::SetTutorialData();
}

Scene* Title::GetNextScene()
{
	return new Play();
}
