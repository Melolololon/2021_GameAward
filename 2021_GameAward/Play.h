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
		PLAY_SCENE_SETTARGET,//ターゲット設置中
		PLAY_SCENE_STARTPREBIOUS,//ゲーム開始前
		PLAY_SCENE_PLAY,//ゲーム中
		PLAY_SCENE_GAMEOVER,//ゲームオーバー
		PLAY_SCENE_GAMECREAL,//ゲームクリア
	};

private:
	Player* player;
	Enemy* enemy;


	//ゲーム状態
	PlaySceneState playSceneState;

#pragma region カメラ
	Vector3 cameraPosition;
	Vector3 cameraTarget;
#pragma endregion

#pragma region ステージデータ
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

