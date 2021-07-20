#pragma once
#include <Scene.h>
#include"Sprite2D.h"
#include"Texture.h"
#include<memory>
#include"Player.h"
#include"TargetObject.h"
class GameOver :
    public Scene
{
private:
	static bool selectEnd;
	bool returnStageSelect = false;
	static Sprite2D gameOverSpr;
	static Texture gameOverTex;

	
	static Sprite2D yesSpr;
	static Texture yesTex;
	static Sprite2D noSpr;
	static Texture noTex;

	std::shared_ptr<Player>player;
	Vector3 playerYesPos = Vector3(-45, 0, -12);
	Vector3 playerNoPos = Vector3(11, 0, -12);

	std::shared_ptr<TargetObject> targetObject[2];
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

