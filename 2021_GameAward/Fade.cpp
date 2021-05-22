#include "Fade.h"

Fade* Fade::GetInstance()
{
	static Fade s;
	return &s;
}

void Fade::LoadResources()
{
	fadeSpr.CreateSprite();
	fadeSpr.SetScale(Vector2(1280, 720));
	fadeSpr.SetSubColor({ 0,0,0,255 });
	fadeTex.LoadSpriteTexture("Resources/Texture/fadeTex.png");
}

void Fade::Update()
{
}

void Fade::Draw()
{
	fadeSpr.Draw(&fadeTex);
}
