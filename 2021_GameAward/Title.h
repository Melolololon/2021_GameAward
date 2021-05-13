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
	static std::unique_ptr<Sprite2D>pushButtonSprite;
	static std::unique_ptr<Texture>pushButtonTexture;
	static FreamTimer titleFreamTimer[7];
	static Vector2 titleSpritePosition[7];

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

