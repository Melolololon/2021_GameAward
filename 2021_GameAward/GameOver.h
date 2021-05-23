#pragma once
#include <Scene.h>
#include"Sprite2D.h"
#include"Texture.h"
#include<memory>
#include"Player.h"
class GameOver :
    public Scene
{
private:
	static bool selectEnd;
	bool returnStageSelect = false;
	static Sprite2D gameOverSpr;
	static Texture gameOverTex;
	std::shared_ptr<Player>player;
public:
	GameOver() {}
	~GameOver() {}

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();
	static bool GetSelectEndFlag();
};

