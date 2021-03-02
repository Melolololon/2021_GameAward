#include "Play.h"
#include"ObjectManager.h"

Play::Play(){}
Play::~Play(){}

void Play::initialize()
{
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();
}

void Play::draw()
{
}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}
