#include "Title.h"
#include"StageSelect.h"

Title::Title(){}
Title::~Title(){}

void Title::LoadResources()
{
}


void Title::Initialize()
{
}

void Title::Update()
{
	if(DirectInput::KeyTrigger(DIK_SPACE))
		isEnd = false;

}

void Title::Draw()
{
}

void Title::Finitialize()
{
}

Scene* Title::GetNextScene()
{
	return new StageSelect();
}
