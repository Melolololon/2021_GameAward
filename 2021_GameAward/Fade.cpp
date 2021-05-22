#include "Fade.h"

const float Fade::FADE_SPEED = 2.0f;

Fade* Fade::GetInstance()
{
	static Fade s;
	return &s;
}

void Fade::LoadResources()
{
	fadeSpr.CreateSprite();
	fadeSpr.SetScale(Vector2(1280, 720));
	fadeTex.LoadSpriteTexture("Resources/Texture/fadeTex.png");
}

void Fade::Update()
{
	if (isStop)
		return;
	
	switch (fadeState)
	{
	case Fade::FADE_OUT:

		fadeSubAlpha -= FADE_SPEED;
		if (fadeSubAlpha <= 0.0f) 
		{
			fadeSubAlpha = 0.0f;
			fadeState = FadeState::FADE_IN;
		}

		break;

	case Fade::FADE_IN:
		fadeSubAlpha += FADE_SPEED;
		if (fadeSubAlpha >= 100.0f) 
		{
			fadeSubAlpha = 100.0f;
			fadeState = FadeState::FADE_NOT;
		}
		break;
	}

	fadeSpr.SetSubColor({ 0,0,0,Color::ToPar(fadeSubAlpha) });
}

void Fade::Draw()
{
	fadeSpr.Draw(&fadeTex);
}

bool Fade::GetSceneChangeTimingFlag()
{
	if (fadeSubAlpha <= 0.0f)
		return true;
	return false;
}

bool Fade::GetFadeEndFlag()
{
	if (fadeSubAlpha >= 100.0f)
		return true;
	return false;
}

