#include "Play.h"
#include"ObjectManager.h"

#pragma region オブジェクト
#include"Player.h"

#pragma endregion


Play::Play(){}
Play::~Play(){}

ModelData m;
void Play::initialize()
{
	ObjectManager::getInstance()->addObject(new Player());

	m.key = "fierd";
	Library::createBoard({ 1000,1000 }, m);
	Library::createHeapData2({ 0,255,0,255 }, 1, m);
	Library::setAngle({ 90,0,0 }, m, 0);
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	//Library::drawGraphic(m, 0);
}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}
