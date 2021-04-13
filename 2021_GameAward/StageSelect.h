#pragma once
#include"Scene.h"
#include"Block.h"
class StageSelect :public Scene
{
private:
	std::vector<std::vector<Block*>>blocks;
	std::vector<float>targetDistance;
	std::vector<float>playerDistance;
	std::vector<int>targetNumbers;

public:
	StageSelect();
	~StageSelect();

	void initialize();
	void update();
	void draw();
	void end();
	std::string getNextScene();
};

