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
		PLAY_SCENE_SET_TARGET,//ターゲット設置中
		PLAY_SCENE_START_PREBIOUS,//ゲーム開始前
		PLAY_SCENE_PLAY,//ゲーム中
		PLAY_SCENE_GAMEOVER,//ゲームオーバー
		PLAY_SCENE_GAMECREAL,//ゲームクリア
	};

private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Enemy> enemy;


	//ゲーム状態
	static PlaySceneState playSceneState;

#pragma region カメラ
	Vector3 cameraPosition;
	Vector3 addCameraPosition;
	Vector3 cameraTarget;
#pragma endregion

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

