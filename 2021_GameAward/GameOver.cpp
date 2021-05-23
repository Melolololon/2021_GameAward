#include "GameOver.h"
#include"Play.h"
#include"StageSelect.h"
#include"Fade.h"
#include"XInputManager.h"
#include"ObjectManager.h"
#include"LibMath.h"

Sprite2D GameOver::gameOverSpr;
Texture GameOver::gameOverTex;
bool GameOver::selectEnd = false;
void GameOver::LoadResources() 
{
	gameOverSpr.CreateSprite();
	gameOverTex.LoadSpriteTexture("Resources/Texture/GameOver.png");
}

void GameOver::Initialize()
{
	player = std::make_shared<Player>(Vector3(-38, 0, -6));
	ObjectManager::GetInstance()->AddObject(player);



#pragma region ƒJƒƒ‰ˆÚ“®
	Vector3 cameraPosition = Vector3(0, 50, -2);
	Vector3 cameraTarget = Vector3(0, 0, 0);

	Vector3 upVector = LibMath::OtherVector(cameraPosition, cameraTarget);
	upVector = LibMath::RotateVector3(upVector, { 1,0,0 }, 90);
	Library::SetCamera(cameraPosition, cameraTarget, upVector);

#pragma endregion
}

void GameOver::Update()
{
	ObjectManager::GetInstance()->Update();




	if (DirectInput::KeyTrigger(DIK_SPACE)
		|| XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)) 
	{
		selectEnd = true;
		Fade::GetInstance()->FadeStart();
	}

	if (Fade::GetInstance()->GetSceneChangeTimingFlag())
		isEnd = true;
}

void GameOver::Draw()
{
	gameOverSpr.Draw(&gameOverTex);
	ObjectManager::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
}

void GameOver::Finitialize()
{
}

Scene* GameOver::GetNextScene()
{
	if (returnStageSelect)
		return new StageSelect();
	return new Play();
}

bool GameOver::GetSelectEndFlag()
{
	if (selectEnd)
		return true;
	return false;
}
