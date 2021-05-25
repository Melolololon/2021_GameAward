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
		PLAY_SCENE_SET_TARGET,//�^�[�Q�b�g�ݒu��
		PLAY_SCENE_START_PREVIOUS,//�Q�[���J�n�O
		PLAY_SCENE_PLAY,//�Q�[����
		PLAY_SCENE_GAMEOVER,//�Q�[���I�[�o�[
		PLAY_SCENE_GAMECLEAR,//�Q�[���N���A
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
	//�Ƃ肠�����Œ�
	const int ENEMY_COUNT = 5;


	//�Q�[�����
	static PlaySceneState playSceneState;

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

	static Sprite2D targetAnimationSprite;
	static Texture targetAnimationTexture;
	UINT targetAnimationNum = 0;
	FreamTimer targetAnimationTimer;
	static const int TARGET_ANIMATION_ONE_FREAM_TIME = 60 * 0.1;


#pragma endregion

#pragma region �Q�[���̃^�C�}�[


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

