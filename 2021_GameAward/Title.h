#pragma once
#include"Scene.h"
#include"Sprite2D.h"
#include"Texture.h"
#include<memory>
#include"FreamTimer.h"
class Title:public Scene
{
private:
	static std::unique_ptr<Sprite2D>titleSprite[7];
	static std::unique_ptr<Texture>titleTexture;
	static const Vector2 TITLE_SCALE;
	static const int TITLE_CHAR_NUM = 7;

	static std::unique_ptr<Sprite2D>pushButtonSprite;
	static std::unique_ptr<Texture>pushButtonTexture;
	static FreamTimer titleFreamTimer[TITLE_CHAR_NUM];
	static FreamTimer titleTopStopTimer;
	static Vector2 titleSpritePosition[TITLE_CHAR_NUM];
	static const int TITLE_SPRITE_STOP_TIME_INTERVAL = 20;
	bool titleSlowMove[TITLE_CHAR_NUM];
	static const float TITLE_UP_DOWN_SPEED;

	static Sprite2D titleBackSpr;
	static Texture titleBackTex;

	static bool nextSceneTutorial;
public:
	Title();
	~Title();

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();
};

