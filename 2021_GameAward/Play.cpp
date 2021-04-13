#include "Play.h"
#include "MoveEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#pragma region オブジェクト
#include"Block.h"


#pragma endregion


int Play::stageNum;

Play::Play()
{
	
}
Play::~Play(){}

void Play::initialize()
{
	player = new Player();
	enemy = new MoveEnemy();
	enemy->setPPlayer(player);
	ObjectManager::getInstance()->addObject(player);
	ObjectManager::getInstance()->addObject(enemy);

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



}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}
