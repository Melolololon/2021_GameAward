#pragma once
#include"Scene.h"
class Title:public Scene
{
private:
	
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

