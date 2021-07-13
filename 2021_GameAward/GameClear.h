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
		MOVE_RESULT,//è„Ç©ÇÁç~ÇËÇƒÇ≠ÇÈ
		SET_ENEMY,//ìGÇÃèàóù(è„Ç©ÇÁóéÇ∆Ç∑Ç©î{ó¶Ç0Ç©ÇÁëùÇ‚Ç∑Ç©)
		ADD_ENEMY_VALUE,//ìGÇÃílÇâ¡éZ
		ADD_TIME,//É^ÉCÉÄÇÃâ¡éZÅAÉâÉìÉNÉAÉbÉvèàóù
		PROCESS_END,//èàóùèIóπ
	};


	class NumberData	
	{
	public:
		static const int NUMBER_DIGIT = 6;
		static const Vector2 NUMBER_MIN_SCALE;
		static const Vector2 NUMBER_MAX_SCALE;
		static const float NUMBER_SCALLING_SPEED;
		static const float NUMBER_ADD_START_TIME;

		NumberData(const Vector2& pos);

		static void LoadTexture();
		void Update();
		bool AddEnd() { return drawTime >= maxNum; }
		int GetDrawTime() { return drawTime; }
		void Draw();
		void SetMaxNum(const int num) { maxNum = num; }
	private:

		Sprite2D numberSprite[NUMBER_DIGIT];
		static Texture numberTexture;

		Vector2 scale = 0;
		FreamTimer addStartTimer;
		float subAlpha = 100.0f;
		int drawTime = 0;
		Vector2 position = 0;
		int maxNum = 0;
	};

	ResultState resultState = ResultState::MOVE_RESULT;
	FreamTimer nextStateTimer;

	static int stageNum;
	StageRank rank = StageRank::RANK_C;
	static int clearTime;

	//åªç›ï`âÊÇ≥ÇÍÇƒÇ¢ÇÈÉ^ÉCÉÄ
	NumberData timeNumberData;




	static Sprite2D rankSprite;
	static Texture rankTexture;

	static Sprite2D rankFreamSprite;
	static Texture rankFreamTexture[5];
	const Vector2 RANK_FREAM_STOP_POSITION = Vector2(220, 80);
	Vector2 rankFreamPosition = 0;
	const float RANK_FREAM_SPEED = 3.0f;

	static ObjModel enemyModel;
	float enemyAngle = 0.0f;
	const float ENEMY_ROT_SPEED = 1.0f;
	const Vector3 ENEMY_STOP_POSITION = Vector3(-2.8f, 0.4f, 0);
	Vector3 enemyPosition = 0;
	const float ENEMY_MOVE_SPEED = 0.025f;



public:
	GameClear():timeNumberData(Vector2(120, 430)){}
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

