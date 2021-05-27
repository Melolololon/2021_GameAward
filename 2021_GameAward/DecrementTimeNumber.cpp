#include "DecrementTimeNumber.h"
#include"Game.h" 
Texture DecrementTimeNumber::numTex;

const Vector3 DecrementTimeNumber::NUMBER_SIZE = 4;

DecrementTimeNumber::DecrementTimeNumber(const Vector3& pos, const int decTime)
{
	Initialize();
	position = pos;
	this->decTime = decTime;

	Vector3 addNumVector = Vector3(NUMBER_SIZE.x / 2, 0,0);

	for (int i = 0; i < _countof(decSpr); i++) 
	{
		decSpr[i].SetPosition(position + addNumVector);
		addNumVectors[i] = addNumVector;
		addNumVector.x -= NUMBER_SIZE.x / 2;
		addNumVector.y += 0.01f;

	}
}

DecrementTimeNumber::~DecrementTimeNumber()
{
}

void DecrementTimeNumber::Initialize()
{
	for (int i = 0; i < _countof(decSpr); i++) 
	{
		decSpr[i].CreateSprite(NUMBER_SIZE);
		decSpr[i].SetBillboardFlag(true, true, true);
	}
	velocity = Vector3(0, 1, 0);
	speed = Vector3(0, 0.1f, 0);

	sortNumber = 2;
}

void DecrementTimeNumber::Update()
{
	position += velocity * speed;
	for (int i = 0; i < _countof(decSpr); i++)
	{
		decSpr[i].SetPosition(position + addNumVectors[i]);
	}

	if (subAlpha >= 100.0f)eraseManager = true;
}

void DecrementTimeNumber::Draw()
{

	std::string drawStr = std::to_string(-decTime);
	int keta = drawStr.size();
	for (int i = 0; i < keta; i++)
	{
		decSpr[i].SetSubColor(Color(0, 0, 0, Color::ToPar(subAlpha)));

		std::string str = drawStr.substr(keta - 1 - i, 1);
		if (str == "-")
		{
			decSpr[i].SelectDrawAreaDraw(Vector2(10 * 80, 0), Vector2(10 * 80 + 80, 80), &numTex);
		}
		else
		{
			int n = atoi(str.c_str());
			decSpr[i].SelectDrawAreaDraw(Vector2(n * 80, 0), Vector2(n * 80 + 80, 80), &numTex);
		}

	}

	subAlpha += 1.0f;
}

void DecrementTimeNumber::LoadResources()
{
	numTex.LoadSpriteTexture("Resources/Texture/TimeNumber.png");

}


