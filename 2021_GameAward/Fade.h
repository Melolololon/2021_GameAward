#pragma once
#include"Sprite2D.h"
#include"Texture.h"
//フェードインアウトクラス
class Fade
{
private:
	Fade(){}
	~Fade(){}

	Sprite2D fadeSpr;
	Texture fadeTex;

public:
	Fade(Fade& f) = delete;
	Fade& operator=(Fade& f) = delete;
	static Fade* GetInstance();


	void LoadResources();
	void Update();
	void Draw();
	
};

