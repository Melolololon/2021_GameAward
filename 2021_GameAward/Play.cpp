#include "Play.h"
#include"ObjectManager.h"

#pragma region オブジェクト
#include"Player.h"

#pragma endregion


Play::Play(){}
Play::~Play(){}

void Play::initialize()
{
	ObjectManager::getInstance()->addObject(new Player());
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();
}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}
