#pragma once
#include <Scene.h>

#include"Player.h"
#include"Enemy.h"
#include"Block.h"
#include"TargetObject.h"
#include<vector>
#include "FreamTimer.h"

#include"MoveEnemy.h"
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

	enum TutorialState
	{
		TUTORIAL_STATE_NOT_TUTORIAL,//チュートリアルではない
		TUTORIAL_STATE_MOVE,//移動チュートリアル
		TUTORIAL_STATE_SHOT,//攻撃チュートリアル
		TUTORIAL_STATE_LOCK,//ロックチュートリアル
		TUTORIAL_STATE_TWIST,//捻りチュートリアル
	};

	enum EnemyType
	{
		ET_MoveEnemy,
		ET_FleeEnemy,
		ET_ShotEnemy,
		ET_SimEnemy,
		ET_DefenceEnemy,
		ET_HealEnemy,
	};

private:
	static int stageNum;

	std::shared_ptr<Player> player;
	std::shared_ptr<Player> pauseSnake;
	//とりあえず固定
	const int ENEMY_COUNT = 10;


	//ゲーム状態
	static PlaySceneState playSceneState;

	//チュートリアル状態
	static TutorialState tutorialState;

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

#pragma region チュートリアル
	static Sprite2D tutorialMessageSpr;
	static Texture tutorialMessageTex[4];
	static Sprite2D tutorialSkipSpr;
	static Texture tutorialSkipTex;
	static Sprite2D tutorialNextSpr;
	static Texture tutorialNextTex;

	std::shared_ptr<MoveEnemy>tutorialEnemy;
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

	static Sprite3D targetLockSprite;
	static Texture targetLockTexture;

	static Sprite2D timerSprite[6];
	static Texture timerTexture;

	static Sprite2D startTimeSpr;
	static Sprite2D startSpr;
	static Texture startTex;

	static Sprite2D targetAnimationSprite;
	static Texture targetAnimationTexture;
	UINT targetAnimationNum = 0;
	FreamTimer targetAnimationTimer;
	static const int TARGET_ANIMATION_ONE_FREAM_TIME = 60 * 0.1;

	static Sprite2D targetNumSprite;
	static Sprite2D targetCrossSprite;
	static Texture targetCrossTexture;

	static Sprite2D pauseSpr;
	static Texture pauseTex; 
#pragma endregion

#pragma region ゲームのタイマー


	FreamTimer gameTime;

	FreamTimer sceneEndTimer;
	static const int SCENE_END_TIME = 60 * 3;

	FreamTimer slowTimer;
#pragma endregion

	static bool isPause;
	bool backStageSelect = false;

public:
	Play();
	~Play();

	void Initialize();
	void Update();
	void Draw();
	void Finitialize();
	Scene* GetNextScene();

	void Tutorial();

	static void LoadResources();
	
	static void SetStageData
	(
		const int stage,
		std::vector<Vector3>blockPos,
		std::vector<Vector3>blockScale,
		const float& targetDis,
		const float& playerDis,
		const int& targetNum,
		const Vector3& leftUpPos,
		const Vector3& rightDownPos
	);

	

	static PlaySceneState GetPlaySceneState() { return playSceneState; }
	static TutorialState GetTutorialState() { return tutorialState; }

	static bool GetIsPauseFlag() { return isPause; }


	bool isCollision(const Vector3& blockLeftUp, const Vector3& blockRightDown, const Vector3& enemyLeftUp, const Vector3& enemyRightDown);
};

