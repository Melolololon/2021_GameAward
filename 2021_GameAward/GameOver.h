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
	Vector3 yesPos = Vector3(-38, 0, -6);
	Vector3 noPos = Vector3(3, 0, -6);
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

