#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#pragma region �I�u�W�F�N�g
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
	enemy = new ShotEnemy();
	enemy->setPPlayer(player);
	enemy->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	ObjectManager::getInstance()->addObject(player);
	ObjectManager::getInstance()->addObject(enemy);

#pragma region �J����
	cameraPosition = { 0,50,-2 };
	cameraTarget = { 0,0,0 };
#pragma endregion

	//��
	Block::setGameStart(true);
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

#pragma region �J�����ړ�
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += {0, 50, -2};
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
