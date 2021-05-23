#pragma once
#include <Scene.h>
class GameClear :
    public Scene
{
private:
	static UINT score;

public:
	GameClear(){}
	~GameClear(){}

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();

	static void SetScore(const UINT num) { score = num; }
};

