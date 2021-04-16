#pragma once
#include"Scene.h"
#include"Block.h"
#include<memory>
class StageSelect :public Scene
{
private:
	int selectStageNum;

	//�}�b�v�\���p(�񂷂��߂ɓ����)allDelete�ŏ����Ȃ��悤�ɂ��邽�߂ɁA�}�l�[�W���[�Ǘ��O
	std::vector<std::vector<std::unique_ptr<Block>>>blocks;
	std::vector<std::vector<Vector3>>blockPositions;
	std::vector<std::vector<Vector3>>blockScales;
	std::vector<float>targetDistance;
	std::vector<float>playerDistance;
	std::vector<int>targetNumbers;
	std::vector<Vector3>leftUpPositions;
	std::vector<Vector3>rightDownPositions;
public:
	StageSelect();
	~StageSelect();

	void initialize();
	void update();
	void draw();
	void end();
	std::string getNextScene();
};

