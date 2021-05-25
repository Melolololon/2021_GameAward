#pragma once
#include <Scene.h>

#include"Player.h"
#include"Enemy.h"
#include"Block.h"
#include"TargetObject.h"
#include<vector>
#include "FreamTimer.h"
class Play :
	public Scene
{
public:
	enum PlaySceneState
	{
		PLAY_SCENE_SET_TARGET,//ターゲット設置中
		PLAY_SCENE_START_PREVIOUS,//ゲーム開始前
		PLAY_SCENE_PLAY,//ゲーム中
		PLAY_SCENE_GAMEOVER,//ゲームオーバー
		PLAY_SCENE_GAMECLEAR,//ゲームクリア
	};

	enum EnemyType
	{
		ET_MoveEnemy,
		ET_ShotEnemy,
		ET_FleeEnemy,
		ET_SimEnemy,
		ET_DefenceEnemy,
		ET_HealEnemy,
	};

private:
	std::shared_ptr<Player> player;
	//とりあえず固定
	const int ENEMY_COUNT = 5;


	//ゲーム状態
	static PlaySceneState playSceneState;

#pragma region カメラ
	Vector3 cameraPosition;
	Vector3 addCameraPosition;
	Vector3 cameraTarget;
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

#pragma region スプライト関係


#pragma region 祠の方向を示す矢印
	static Sprite2D arrowSprite;
	static Texture arrowTexture;

	//座標
	Vector2 arrowPosition;
	//回転角度
	float arrowAngle;
	//描画するかどうか
	bool drawArrow;
#pragma endregion

	static Sprite3D targetLockSprite;
	static Texture targetLockTexture;

	static Sprite2D timerSprite[6];
	static Texture timerTexture;

	static Sprite2D targetAnimationSprite;
	static Texture targetAnimationTexture;
	UINT targetAnimationNum = 0;
	FreamTimer targetAnimationTimer;
	static const int TARGET_ANIMATION_ONE_FREAM_TIME = 60 * 0.1;


#pragma endregion

#pragma region ゲームのタイマー


	FreamTimer gameTime;

	FreamTimer sceneEndTimer;
	static const int SCENE_END_TIME = 60 * 3;

	FreamTimer slowTimer;
#pragma endregion

	bool isPause = false;

public:
	Play();
	~Play();

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();

	static void LoadResources();
	
	static void SetStageData
	(
		std::vector<Vector3>blockPos,
		std::vector<Vector3>blockScale,
		const float& targetDis,
		const float& playerDis,
		const int& targetNum,
		const Vector3& leftUpPos,
		const Vector3& rightDownPos
	);

	static PlaySceneState GetPlaySceneState()
	{
		return playSceneState;
	}

};

