#pragma once
#include <Scene.h>
#include"Sprite2D.h"
#include"Texture.h"
#include"FreamTimer.h"
#include"ObjModel.h"
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
		PROCESS_END,//処理終了
	};

	class ResultData
	{
	private:
		Vector2 position = 0;
		Vector2 scale = 0;
		float subAlpha = 100.0f;
		Vector2 minScale;
		Vector2 maxScale;
		const float scalingSpeed = 0.0f;
		bool processEnd = false;
	public:
		ResultData(const Vector2& pos, const Vector2 minScale, const Vector2 maxScale,const float scalingSpeed): 
			position(pos)
			, scale(maxScale)
			, minScale(minScale)
			, maxScale(maxScale)
			, scalingSpeed(scalingSpeed)
		{}
		void Update();

		Vector2 GetPosition() { return position; }
		Vector2 GetScale() { return scale; }
		float GetSubAlpha() { return subAlpha; }
		bool GetProcessEnd() { return processEnd; }
	};

	class NumberData
	{
	public:
		static const int NUMBER_DIGIT = 6;
		static const Vector2 NUMBER_MIN_SCALE;
		static const Vector2 NUMBER_MAX_SCALE;
		static const float NUMBER_SCALLING_SPEED;
		static const int NUMBER_ADD_START_TIME;

		NumberData(const Vector2& pos);

		static void LoadTexture();
		void Update();
		bool ProcessEnd();
		int GetDrawTime() { return drawNum; }
		void Draw();
		void SetMaxNum(const int num) { maxNum = num; }
	private:

		Sprite2D numberSprite[NUMBER_DIGIT];
		static Texture numberTexture;

		ResultData resultData;

		FreamTimer addStartTimer;
		int drawNum = 0;
		int maxNum = 0;
		bool processEnd = false;
	};

	ResultState resultState = ResultState::MOVE_RESULT;
	FreamTimer nextStateTimer;

	static int stageNum;
	StageRank rank = StageRank::RANK_C;
	static int clearTime;


	//現在描画されているタイム
	NumberData timeNumberData;

	//倒した数
	NumberData enemyDeadCountData;
	//倒した数の掛ける
	static Sprite2D crossSprite;
	static Texture crossTexture;
	ResultData crossResultData;

	//結果の枠
	static Sprite2D resultFreamSprite;
	static Texture resultFreamTexture;
	const Vector2 RESULT_FREAM_STOP_POSITION = Vector2(220, 80);
	Vector2 resultFreamPosition = 0;
	const float RESULT_FREAM_SPEED = 3.0f;

	//ランク
	static Sprite2D rankSprite;
	static Texture rankTexture;
	ResultData rankResultData;

	

	static ObjModel enemyModel;
	float enemyAngle = 0.0f;
	const float ENEMY_ROT_SPEED = 1.0f;
	const Vector3 ENEMY_STOP_POSITION = Vector3(-2.45f, 0.45f, 0);
	Vector3 enemyPosition = 0;
	const float ENEMY_MOVE_SPEED = 0.025f;

	//木
	static const int TREE_NUM_X = 8;
	static const int TREE_NUM_Y = 6;
	static Sprite2D treeSprite[TREE_NUM_Y][TREE_NUM_X];
	static Texture treeTexture[4];
	int treeTextureNumber[TREE_NUM_Y][TREE_NUM_X];
public:
	//timeNumberDataなどの数値の座標は、実際の座標じゃなくて補正値なので注意

	GameClear() :
		timeNumberData(Vector2(-30, 430))
		, enemyDeadCountData(Vector2(-30, 260))
		, rankResultData(Vector2(680, 290), 1.0f, 3.0f, 0.3f)
		, crossResultData(Vector2(395, 290), 1.0f, 3.0f, 0.3f) {}
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

