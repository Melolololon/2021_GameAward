#pragma once
#include"Scene.h"
#include"Block.h"
#include"Player.h"
#include<memory>

#include"FreamTimer.h"
class StageSelect :public Scene
{
public :
	enum StageSelectState
	{
		STAGE_SELECT_STATE_SELECT,
		STAGE_SELECT_STATE_SELECT_END,
		STAGE_SELECT_STATE_RETURN_TITLE,

	};


private:
	static int selectStageNum;
	static int maxStageNum;

	//シーン切り替え手も消えないようにするためにstatic
	//[ステージ][ブロック]
	static std::vector<std::vector<std::shared_ptr<Block>>>blocks;
	static std::vector<std::vector<Vector3>>blockPositions;
	static std::vector<std::vector<Vector3>>blockScales;
	static std::vector<float>targetDistance;
	static std::vector<float>playerDistance;
	static std::vector<int>targetNumbers;
	static std::vector<Vector3>leftUpPositions;
	static std::vector<Vector3>rightDownPositions;
	static std::vector<Vector3>mapMovePositions;
	static std::vector<float>worldCenterToStageVectorAngle;
	
	//ステージが回転してるかどうか
	enum class StageRotateState
	{
		STAGE_ROTATE_LEFT,
		STAGE_ROTATE_RIGHT,
		STAGE_ROTATE_NOT_ROTATE,
	};
	StageRotateState stageRotateState = StageRotateState::STAGE_ROTATE_NOT_ROTATE;

	const Vector3 MAP_MOVE_POSITION = { 0,0,-800 };

	static float mapRotateAngleMax;
	//回転量カウント
	float mapRotateAngleCount = 0.0f;
	//現在の回転角度
	//float mapAngle = 0.0f;
	const float MAP_ROTATE_SPEED = 3.5f;
	//回転軸となるベクトル
	static const Vector3 MAP_ROTATE_VECTOR;

	//std::shared_ptr<Player>player;
	Vector3 playerMoveVector = 0;
	//UINT playerRotateTimer;
	//static const UINT playerRotateTime;
	//UINT nextFromSelectionTimer;//選択から次のシーンまでの時間
	//static const UINT nextFromSelectionTime;


#pragma region スプライト


	static Sprite2D returnTitleSpr;
	static Texture returnTitleTex;
	static Sprite2D selectSpr;
	static Texture selectTex;


	static std::vector<Sprite3D> mapBackSpr;
	static Texture mapBackTex;
	static Sprite3D arrowSpr[2];
	static Texture arrowTex[2];
	static std::vector<Sprite3D> stageStringSpr;
	static std::vector<std::unique_ptr<Texture>> stageStringTex;
#pragma endregion


	//状態
	static StageSelectState stageSelectState;
	FreamTimer nextSceneTimer;



public:
	StageSelect();
	~StageSelect();

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();
	static void LoadResources();

	static int GetSelectStageNum() { return selectStageNum; }
	static StageSelectState GetStageSelectState() {return stageSelectState;}

	static void SetTutorialData();
};

