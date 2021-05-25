#pragma once
#include"Sprite2D.h"
#include"Texture.h"


//フェードインアウトクラス
class Fade
{
public:
	enum FadeState
	{
		FADE_NOT,
		FADE_IN,
		FADE_OUT,
	};

private:


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

	void FadeStart() { fadeState = FadeState::FADE_OUT; }
	bool GetSceneChangeTimingFlag();
	bool GetFadeEndFlag();
	FadeState GetFadeState() { return fadeState; }

	void SetIsStopFlag(const bool flag) { isStop = flag; }
};

