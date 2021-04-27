#pragma once
#include <Scene.h>

#include"Player.h"
#include"Enemy.h"
#include"Block.h"
#include"TargetObject.h"
#include<vector>

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
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Enemy>> enemies;


	//ゲーム状態
	PlaySceneState playSceneState;

#pragma region カメラ
	Vector3 cameraPosition;
	Vector3 addCameraPosition;
	Vector3 cameraTarget;
#pragma endregion

#pragma region 祠の方向を示す矢印
	static sprite arrowSprite;
	static texture arrowTexture;

	//座標
	Vector2 arrowPosition;
	//回転角度
	float arrowAngle;
	//描画するかどうか
	bool drawArrow;
#pragma endregion


#pragma region ステージデータ
	std::vector<std::shared_ptr<TargetObject> > targetObjects;
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
	Scene* getNextScene();

	static void loadResources();
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

