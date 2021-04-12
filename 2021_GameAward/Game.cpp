#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"

#include"Play.h"

#pragma region Object
#include"Player.h"
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
		Library::roopStartProcess();
		update();
		draw();
		Library::roopEndProcess();
		if (Library::getIsEnd())break;
	}

	end();
}

void Game::initialize()
{
#pragma region ライブラリ初期化_設定
	Library::initialize(1280, 720, { 0,0,0,255 }, L"メビウスネーク");
	Library::setCameraNearAndFar(0.1f, 100.0f);
	
	Library::setFramesPerSecond60(true);
#pragma endregion


#pragma region モデル読み込み
	Player::loadModel();
	Enemy::loadModel();

#pragma endregion

#pragma region マネージャー初期化_設定
	ObjectManager::getInstance()->initialize();
	ObjectManager::getInstance()->reserveObjectArray(200);
	CollisionFlag f;
	f.sphere = true;
	ObjectManager::getInstance()->setCollisionFlag3D(f);

	SceneManager::getInstace()->initialize();
	SceneManager::getInstace()->addScene("Play", new Play());
#pragma endregion


}


void Game::end()
{
#pragma region マネージャー終了
	ObjectManager::getInstance()->end();
	SceneManager::getInstace()->end();
#pragma endregion


	Library::end();//絶対に最後に書く
}

void Game::update()
{
	SceneManager::getInstace()->update();

}

void Game::draw()
{
	SceneManager::getInstace()->draw();
}