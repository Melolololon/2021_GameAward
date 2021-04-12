#pragma once
#include <Scene.h>
#include"Player.h"
#include"Enemy.h"
class Play :
	public Scene
{
private:
	Player* player;
	Enemy* enemy;

#pragma region �J����
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

