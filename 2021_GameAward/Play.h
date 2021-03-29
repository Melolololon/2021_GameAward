#pragma once
#include <Scene.h>
#include"Player.h"
class Play :
	public Scene
{
private:
	Player* player;

#pragma region ƒJƒƒ‰
	Vector3 cameraPosition;
	Vector3 cameraTarget;
#pragma endregion

public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string getNextScene();
};

