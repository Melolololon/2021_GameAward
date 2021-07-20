#include "GameOver.h"
#include"Play.h"
#include"StageSelect.h"
#include"Fade.h"
#include"XInputManager.h"
#include"ObjectManager.h"
#include"LibMath.h"

Sprite2D GameOver::gameOverSpr;
Texture GameOver::gameOverTex;
Sprite2D GameOver::yesSpr;
Texture GameOver::yesTex;
Sprite2D GameOver::noSpr;
Texture GameOver::noTex;

bool GameOver::selectEnd = false;
void GameOver::LoadResources() 
{
	gameOverSpr.CreateSprite();
	gameOverTex.LoadSpriteTexture("Resources/Texture/GameOver/GameOverBack.png");

	yesSpr.CreateSprite();
	yesSpr.SetPosition(Vector2(160,350));
	yesSpr.SetScale(0.6f);
	yesTex.LoadSpriteTexture("Resources/Texture/GameOver/Continue.png");
	noSpr.CreateSprite();
	noSpr.SetPosition(Vector2(590,350));
	noSpr.SetScale(0.6f);
	noTex.LoadSpriteTexture("Resources/Texture/GameOver/Cancel.png");
}

void GameOver::Initialize()
{
	selectEnd = false;
	player = std::make_shared<Player>(playerYesPos);
	ObjectManager::GetInstance()->AddObject(player);

	targetObject[0] = std::make_shared<TargetObject>(Vector3(-27, 0, -22), 1);
	targetObject[1] = std::make_shared<TargetObject>(Vector3(27, 0, -22), 1);
	ObjectManager::GetInstance()->AddObject(targetObject[0]);
	ObjectManager::GetInstance()->AddObject(targetObject[1]);

#pragma region ƒJƒƒ‰ˆÚ“®
	Vector3 cameraPosition = Vector3(0, 65, -2);
	Vector3 cameraTarget = Vector3(0, 0, 0);

	Vector3 upVector = LibMath::OtherVector(cameraPosition, cameraTarget);
	upVector = LibMath::RotateVector3(upVector, { 1,0,0 }, 90);
	Library::SetCamera(cameraPosition, cameraTarget, upVector);

#pragma endregion
}

void GameOver::Update()
{
	ObjectManager::GetInstance()->Update();

	if (XInputManager::GetPadConnectedFlag(1))
	{
		float stickAngle = XInputManager::LeftStickAngle(1);

		if (stickAngle >= 0
			&& stickAngle <= 30
			|| stickAngle >= 330) 
		{
			returnStageSelect = true;
			player->SetModelMoveVector(playerNoPos);
		}
	}

	if (XInputManager::GetPadConnectedFlag(1))
	{
		float stickAngle = XInputManager::LeftStickAngle(1);

		if (stickAngle >= 150
			&& stickAngle <= 180)
		{
			returnStageSelect = false;
			player->SetModelMoveVector(playerYesPos);
			
		}
	}

	if (XInputManager::GetPadConnectedFlag(1)
		&& (XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)
			|| XInputManager::ButtonTrigger(XInputManager::XINPUT_A_BUTTON, 1)))
	{
		selectEnd = true;
		Fade::GetInstance()->FadeStart();

	}

	if (Fade::GetInstance()->GetSceneChangeTimingFlag()) isEnd = true;
}

void GameOver::Draw()
{
	gameOverSpr.Draw(&gameOverTex);
	yesSpr.Draw(&yesTex);
	noSpr.Draw(&noTex);

	ObjectManager::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
}

void GameOver::Finitialize()
{
	ObjectManager::GetInstance()->AllEraseObject();

	if (returnStageSelect) 
	{
		Library::StopLoadSound("Play", false);
	}
}

Scene* GameOver::GetNextScene()
{
	if (returnStageSelect) 
	{
		return new StageSelect();
	}
	return new Play();
}

bool GameOver::GetSelectEndFlag()
{
	if (selectEnd)
		return true;
	return false;
}
