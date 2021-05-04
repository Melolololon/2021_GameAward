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
		PLAY_SCENE_SET_TARGET,//�^�[�Q�b�g�ݒu��
		PLAY_SCENE_START_PREBIOUS,//�Q�[���J�n�O
		PLAY_SCENE_PLAY,//�Q�[����
		PLAY_SCENE_GAMEOVER,//�Q�[���I�[�o�[
		PLAY_SCENE_GAMECREAL,//�Q�[���N���A
	};

private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Enemy> enemy;


	//�Q�[�����
	static PlaySceneState playSceneState;

#pragma region �J����
	Vector3 cameraPosition;
	Vector3 addCameraPosition;
	Vector3 cameraTarget;
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

