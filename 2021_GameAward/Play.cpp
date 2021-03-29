#include "Play.h"
#include"ObjectManager.h"

#pragma region オブジェクト


#pragma endregion


Play::Play(){}
Play::~Play(){}

ModelData m;
void Play::initialize()
{
	player = new Player();
	ObjectManager::getInstance()->addObject(player);

	m.key = "fierd";
	Library::createBoard({ 1000,1000 }, m);
	Library::createHeapData2({ 0,255,0,255 }, 1, m);
	Library::setAngle({ 90,0,0 }, m, 0);

#pragma region カメラ
	cameraPosition = { 0,30,-2 };
	cameraTarget = { 0,0,0 };
#pragma endregion

}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

#pragma region カメラ移動
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += {0, 30, -2};
	cameraTarget = pHeapPos;
	Library::setCamera(cameraPosition, cameraTarget, { 0,1,0 });

#pragma endregion
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
