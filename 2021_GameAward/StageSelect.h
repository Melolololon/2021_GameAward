#pragma once
#include"Scene.h"
#include"Block.h"
#include"Player.h"
#include<memory>

#include"FreamTimer.h"
class StageSelect :public Scene
{
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

	std::shared_ptr<Player>player;
	//UINT playerRotateTimer;
	//static const UINT playerRotateTime;
	//UINT nextFromSelectionTimer;//選択から次のシーンまでの時間
	//static const UINT nextFromSelectionTime;

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
};

