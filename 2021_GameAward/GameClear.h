#pragma once
#include <Scene.h>
#include"Sprite2D.h"
#include"Texture.h"
#include"FreamTimer.h"
class GameClear :
	public Scene
{
public:
	static const int S_RUNK_TIME[5];
	static const int A_RUNK_TIME[5];
	static const int B_RUNK_TIME[5];

private:
	enum class StageRank
	{
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,
	};

	enum class ResultState
	{
		MOVE_RESULT,//上から降りてくる
		SET_ENEMY,//敵の処理(上から落とすか倍率を0から増やすか)
		ADD_ENEMY_VALUE,//敵の値を加算
		ADD_TIME,//タイムの加算、ランクアップ処理
	};
	ResultState resultState = ResultState::MOVE_RESULT;
	FreamTimer nextStateTimer;

	static int stageNum;
	StageRank rank = StageRank::RANK_C;
	static int clearTime;
	//現在描画されているタイム
	int drawTime = 0;
	static Sprite2D timeSprite[6];
	static Texture timeTexture;
	static Vector2 timeScale;

	static Sprite2D rankSprite;
	static Texture rankTexture;

	static Sprite2D rankFreamSprite;
	static Texture rankFreamTexture[5];
	const Vector2 RANK_FREAM_STOP_POSITION = Vector2(220, 80);
	Vector2 rankFreamPosition = Vector2(RANK_FREAM_STOP_POSITION.x, -400);
	const float RANK_FREAM_SPEED = 3.0f;

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
	static void SetTime(const int num) { clearTime = num; }
};

