#include "Title.h"
#include"StageSelect.h"
#include"XInputManager.h"

std::unique_ptr<Sprite2D>Title::titleSprite[7];
std::unique_ptr<Texture>Title::titleTexture;
std::unique_ptr<Sprite2D>Title::pushButtonSprite;
std::unique_ptr<Texture>Title::pushButtonTexture;
FreamTimer Title::titleFreamTimer[7];
Vector2 Title::titleSpritePosition[7];
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
	pushButtonTexture = std::make_unique<Texture>();
	pushButtonTexture->LoadSpriteTexture("Resources/Texture/MS_Title/press_X_button.png");
}

void Title::Initialize()
{
	const float titleTextureSize = titleTexture->GetTextureSize().y;
	for(int i = 0; i < _countof(titleSpritePosition);i++)
		titleSpritePosition[i] = { 1280 + i * titleTextureSize * 3 ,100 };
}
void Title::Update()
{
	
	

	const float titleTextureSize = titleTexture->GetTextureSize().y;
	float titleSpeed = 10.0f;
	for (int i = 0; i < _countof(titleSprite); i++) 
	{
		/*titleSpritePosition[i].x -= titleSpeed;
		if (titleSpritePosition[i].x <= 1280)
			titleFreamTimer[i].SetStopFlag(false);
		if(titleSpritePosition[i] <= )
			titleFreamTimer[i].SetStopFlag(true);*/

		float sinY = sin(titleFreamTimer[i].GetTime() * i/ 10.0f) * 100.0f;
		titleSprite[i]->SetPosition({ -200 * titleTextureSize * 3, + sinY });

	}
	pushButtonSprite->SetPosition({ 500,400 });

	bool padStart = XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_X_BUTTON, 1)
		&& XInputManager::GetPadConnectedFlag(1);
	if(DirectInput::KeyTrigger(DIK_SPACE)
		|| padStart)
		isEnd = true;

}

void Title::Draw()
{
	const float titleTextureSize = titleTexture->GetTextureSize().y;
	for (int i = 0; i < _countof(titleSprite); i++)
		titleSprite[i]->SelectDrawAreaDraw
		(
			{ titleTextureSize * i,0 }, 
			{ titleTextureSize * (i + 1),titleTextureSize }, 
			titleTexture.get()
		);
	
	pushButtonSprite->Draw(pushButtonTexture.get());
}

void Title::Finitialize()
{
}

Scene* Title::GetNextScene()
{
	return new StageSelect();
}
