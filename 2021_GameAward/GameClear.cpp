#include "GameClear.h"
#include"Fade.h"
void GameClear::LoadResources() 
{

}

void GameClear::Initialize()
{
	Fade::GetInstance()->SetIsStopFlag(true);
}

void GameClear::Update()
{

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
}
