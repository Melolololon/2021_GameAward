#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"

#pragma region �V�[��

#include"StageSelect.h"
#include"Play.h"
#pragma endregion

#pragma region Object
#include"Player.h"
#include"PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"
#include"MoveEnemy.h"
#include"ShotEnemy.h"
#include"FleeEnemy.h"
#include"EnemyBullet.h"
#pragma endregion


Game::Game() {}

Game::~Game() {}

Game* Game::getInstance()
{
	static Game game;
	return &game;
}

void Game::run()
{
	initialize();
	while (1)
	{
		Library::loopStartProcess();
		update();
		draw();
		Library::loopEndProcess();
		if (Library::getIsEnd())break;
	}

	end();
}

void Game::initialize()
{
#pragma region ���C�u����������_�ݒ�
	Library::initialize(1280, 720, { 0,0,0,255 }, L"���r�E�X�l�[�N");
	Library::setCameraNearAndFar(0.1f, 400.0f);
	
	Library::setFramesPerSecond60(true);
#pragma endregion


#pragma region ���\�[�X�ǂݍ���
	Player::loadModel();
	TargetObject::loadModel();
	Block::loadModel();
	PlayerBullet::loadModel();
	EnemyBullet::loadModel();
	MoveEnemy::loadModel();
	ShotEnemy::loadModel();
	FleeEnemy::loadModel();

	StageSelect::loadResources();
	Play::loadResources();
#pragma endregion

#pragma region �}�l�[�W���[������_�ݒ�
	ObjectManager::getInstance()->initialize();
	ObjectManager::getInstance()->reserveObjectArray(200);
	CollisionFlag f;
	f.sphere = true;
	ObjectManager::getInstance()->setCollisionFlag3D(f);

	SceneManager::getInstace()->initialize(new StageSelect());
#pragma endregion

}


void Game::end()
{
#pragma region �}�l�[�W���[�I��
	ObjectManager::getInstance()->end();
	SceneManager::getInstace()->end();
#pragma endregion


	Library::end();//��΂ɍŌ�ɏ���
}

void Game::update()
{
	SceneManager::getInstace()->update();

}

void Game::draw()
{
	SceneManager::getInstace()->draw();
}