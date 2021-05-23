#include "GameClear.h"
#include"Fade.h"
#include"XInputManager.h"
#include"StageSelect.h"

UINT GameClear::score = 0;

void GameClear::LoadResources() 
{

}

void GameClear::Initialize()
{
	Fade::GetInstance()->SetIsStopFlag(true);
}

void GameClear::Update()
{
	if (DirectInput::KeyTrigger(DIK_Z)
		|| XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)) 
	{
		isEnd = true;
		Fade::GetInstance()->SetIsStopFlag(false);
	}
}

void GameClear::Draw()
{
	Fade::GetInstance()->Draw();

}

void GameClear::Finitialize()
{
}

Scene* GameClear::GetNextScene()
{
	return new StageSelect();
}
