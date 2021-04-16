#pragma once
#include <Scene.h>

#include"Player.h"
#include"Enemy.h"
#include"Block.h"
#include"TargetObject.h"
class Play :
	public Scene
{
public:
	enum PlaySceneState
	{
		PLAY_SCENE_SETTARGET,//�^�[�Q�b�g�ݒu��
		PLAY_SCENE_STARTPREBIOUS,//�Q�[���J�n�O
		PLAY_SCENE_PLAY,//�Q�[����
		PLAY_SCENE_GAMEOVER,//�Q�[���I�[�o�[
		PLAY_SCENE_GAMECREAL,//�Q�[���N���A
	};

private:
	Player* player;
	Enemy* enemy;


	//�Q�[�����
	PlaySceneState playSceneState;

#pragma region �J����
	Vector3 cameraPosition;
	Vector3 cameraTarget;
#pragma endregion

#pragma region �X�e�[�W�f�[�^
	std::vector<TargetObject*> targetObjects;
	static float targetDistance;
	static float playerDistance;
	static int targetNumber;
	static Vector3 leftUpPosition;
	static Vector3 rightDownPosition;
	static std::vector<Vector3>blockPositions;
	static std::vector<Vector3>blockScales;
#pragma endregion


public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string getNextScene();

	static void setStageData
	(
		std::vector<Vector3>blockPos,
		std::vector<Vector3>blockScale,
		const float& targetDis,
		const float& playerDis,
		const int& targetNum,
		const Vector3& leftUpPos,
		const Vector3& rightDownPos
	);
};

