#pragma once
#include <Scene.h>
class GameClear :
    public Scene
{

public:
	GameClear(){}
	~GameClear(){}

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();
};

