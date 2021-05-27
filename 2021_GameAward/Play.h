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
		PLAY_SCENE_SET_TARGET,//�^�[�Q�b�g�ݒu��
		PLAY_SCENE_START_PREVIOUS,//�Q�[���J�n�O
		PLAY_SCENE_PLAY,//�Q�[����
		PLAY_SCENE_GAMEOVER,//�Q�[���I�[�o�[
		PLAY_SCENE_GAMECLEAR,//�Q�[���N���A
	};

	enum TutorialState
	{
		TUTORIAL_STATE_NOT_TUTORIAL,//�`���[�g���A���ł͂Ȃ�
		TUTORIAL_STATE_MOVE,//�ړ��`���[�g���A��
		TUTORIAL_STATE_SHOT,//�U���`���[�g���A��
		TUTORIAL_STATE_LOCK,//���b�N�`���[�g���A��
		TUTORIAL_STATE_TWIST,//�P��`���[�g���A��
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
	//�Ƃ肠�����Œ�
	const int ENEMY_COUNT = 10;


	//�Q�[�����
	static PlaySceneState playSceneState;

	//�`���[�g���A�����
	static TutorialState tutorialState;

#pragma region �J����
	Vector3 cameraPosition;
	Vector3 addCameraPosition;
	Vector3 cameraTarget;
#pragma endregion

#pragma region �X�e�[�W�f�[�^
	std::vector<std::shared_ptr<TargetObject> > targetObjects;
	static float targetDistance;
	static float playerDistance;
	static int targetNumber;
	static Vector3 leftUpPosition;
	static Vector3 rightDownPosition;
	static std::vector<Vector3>blockPositions;
	static std::vector<Vector3>blockScales;
#pragma endregion

#pragma region �X�v���C�g�֌W

#pragma region �`���[�g���A��
	static Sprite2D tutorialMessageSpr;
	static Texture tutorialMessageTex[4];
	static Sprite2D tutorialSkipSpr;
	static Texture tutorialSkipTex;
	static Sprite2D tutorialNextSpr;
	static Texture tutorialNextTex;

	std::shared_ptr<MoveEnemy>tutorialEnemy;
#pragma endregion


#pragma region �K�̕������������
	static Sprite2D arrowSprite;
	static Texture arrowTexture;

	//���W
	Vector2 arrowPosition;
	//��]�p�x
	float arrowAngle;
	//�`�悷�邩�ǂ���
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

#pragma region �Q�[���̃^�C�}�[


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

