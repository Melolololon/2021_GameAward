#pragma once
#include <Scene.h>
class Play :
	public Scene
{
private:
public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string getNextScene();
};

