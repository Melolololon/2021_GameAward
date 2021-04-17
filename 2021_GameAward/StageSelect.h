#pragma once
#include"Scene.h"
#include"Block.h"
#include<memory>
class StageSelect :public Scene
{
private:
	int selectStageNum;

	//マップ表示用(回すために入れる)allDeleteで消さないようにするために、マネージャー管理外
	static std::vector<std::vector<std::unique_ptr<Block>>>blocks;
	static std::vector<std::vector<Vector3>>blockPositions;
	static std::vector<std::vector<Vector3>>blockScales;
	static std::vector<float>targetDistance;
	static std::vector<float>playerDistance;
	static std::vector<int>targetNumbers;
	static std::vector<Vector3>leftUpPositions;
	static std::vector<Vector3>rightDownPositions;
public:
	StageSelect();
	~StageSelect();

	void initialize();
	void update();
	void draw();
	void end();
	Scene* getNextScene();
	static void loadResources();
};

