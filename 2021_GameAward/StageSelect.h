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

	std::shared_ptr<Player>player;
	Vector3 playerMoveVector = 0;
	//UINT playerRotateTimer;
	//static const UINT playerRotateTime;
	//UINT nextFromSelectionTimer;//選択から次のシーンまでの時間
	//static const UINT nextFromSelectionTime;

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

};

