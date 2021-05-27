#pragma once
#include"Object.h"
#include"Sprite3D.h"
#include"Texture.h"

class DecrementTimeNumber:public Object
{

private:
	Sprite3D decSpr[3];
	static Texture numTex;
	float subAlpha = 0.0f;

	int decTime = 0;
	Vector3 addNumVectors[3];
	static const Vector3 NUMBER_SIZE;

public:
	DecrementTimeNumber(const Vector3& pos,const int decTime);
	~DecrementTimeNumber();
	void Initialize()override;
	void Update()override;
	void Draw()override;

	static void LoadResources();

};

