#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Fade.h"

#pragma region シーン

#include"Title.h"
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
#include"DefenceEnemy.h"
#include"SimEnemy.h"
#include"HealEnemy.h"
#include"EnemyBullet.h"

#pragma endregion


Game::Game() {}

Game::~Game() {}

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}

void Game::Run()
{
	Initialize();
	while (1)
	{
		Library::LoopStartProcess();
		Update();
		Draw();
		Library::LoopEndProcess();
		if (Library::GetIsEnd())break;
	}

	Finitialize();
}

void Game::Initialize()
{
#pragma region ライブラリ初期化_設定
	Library::Initialize(WIN_WIDTH, WIN_HEIGHT, { 255,200,255,255 }, L"メビウスネーク");
	Library::SetCameraNearAndFar(0.1f, 2500.0f);
	Library::SetLightVector({0.1f,-0.75f,0.15f});

	Library::SetFramesPerSecond60(true);
#pragma endregion

#pragma region リソース読み込み
	Player::LoadResource();
	TargetObject::LoadResource();
	Block::LoadResource();
	PlayerBullet::LoadResource();
	EnemyBullet::LoadResource();
	MoveEnemy::LoadResource();
	ShotEnemy::LoadResource();
	FleeEnemy::LoadResource();
	DefenceEnemy::LoadResource();
	SimEnemy::LoadResource();
	HealEnemy::LoadResource();

	Title::LoadResources();
	StageSelect::LoadResources();
	Play::LoadResources();

	Fade::GetInstance()->LoadResources();
#pragma endregion

#pragma region マネージャー初期化_設定
	ObjectManager::GetInstance()->ReserveObjectArray(200);
	CollisionFlag f;
	f.sphere = true;
	f.box = true;
	ObjectManager::GetInstance()->SetCollisionFlag3D(f);

	SceneManager::GetInstace()->SetStartScene(new Title());
#pragma endregion

}


void Game::Finitialize()
{

	Library::Finalize();//絶対に最後に書く
}

void Game::Update()
{
	SceneManager::GetInstace()->Update();
	Fade::GetInstance()->Update();
}

void Game::Draw()
{
	SceneManager::GetInstace()->Draw();
	Fade::GetInstance()->Draw();
}