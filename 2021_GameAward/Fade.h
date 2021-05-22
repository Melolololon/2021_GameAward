#pragma once
#include"Sprite2D.h"
#include"Texture.h"


//フェードインアウトクラス
class Fade
{
private:
	enum FadeState
	{
		FADE_NOT,
		FADE_IN,
		FADE_OUT,
	};

	Fade(){}
	~Fade(){}

	Sprite2D fadeSpr;
	Texture fadeTex;
	float fadeSubAlpha = 100.0f;
	static const float FADE_SPEED;
	bool isStop = false;

	FadeState fadeState = FadeState::FADE_NOT;
public:
	Fade(Fade& f) = delete;
	Fade& operator=(Fade& f) = delete;
	static Fade* GetInstance();

	void LoadResources();
	void Update();
	void Draw();

	void FadeStart() { fadeState = FadeState::FADE_IN; }
	bool GetSceneChangeTimingFlag();
	bool GetFadeEndFlag();

	void SetIsStopFlag(const bool flag) { isStop = flag; }
};

