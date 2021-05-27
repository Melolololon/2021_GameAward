#pragma once
#include <Scene.h>
#include"Sprite2D.h"
#include"Texture.h"
class GameClear :
	public Scene
{
public:
	static const int S_RUNK_TIME[5];
	static const int A_RUNK_TIME[5];
	static const int B_RUNK_TIME[5];

private:
	enum StageRank
	{
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,
	};

	static int stageNum;
	StageRank rank = StageRank::RANK_C;
	static int time;
	static Sprite2D timeSprite[6];
	static Texture timeTexture;
	static Vector2 timeScale;

	static Sprite2D rankSprite;
	static Texture rankTexture;

	static Sprite2D rankFreamSprite;
	static Texture rankFreamTexture[5];


public:
	GameClear() {}
	~GameClear() {}

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();

	static void SetStageNum(const int num) { stageNum = num; }
	static void SetTime(const int num) { time = num; }
};

