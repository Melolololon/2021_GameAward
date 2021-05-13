#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"

#pragma region �V�[��

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
#pragma region ���C�u����������_�ݒ�
	Library::Initialize(1280, 720, { 0,0,0,255 }, L"���r�E�X�l�[�N");
	Library::SetCameraNearAndFar(0.1f, 1500.0f);
	Library::SetLightVector({0,-0.75,0.25});

	Library::SetFramesPerSecond60(true);
#pragma endregion

#pragma region ���\�[�X�ǂݍ���
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
#pragma endregion

#pragma region �}�l�[�W���[������_�ݒ�
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

	Library::Finalize();//��΂ɍŌ�ɏ���
}

void Game::Update()
{

	SceneManager::GetInstace()->Update();

}

void Game::Draw()
{
	SceneManager::GetInstace()->Draw();
}