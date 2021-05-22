#pragma once
#include"Object.h"
#include"Player.h"
#include<string>

#include"FreamTimer.h"

class Enemy : public Object
{

protected:
#pragma region ���f���֌W

	int heapNum;
	//�v���C���[�̃|�C���^
	Player* pPlayer;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	virtual void setHeapNum() = 0;


	//���s�A�j���[�V�����p
	static const int MOVE_ANIMATION_MAX = 60 * 0.7;
	FreamTimer moveAnimationTime;
	Vector3 leftFootAngle;
	Vector3 rightFootAngle;
	static const float FREAM_MOVE_ANGLE;
	static const float MAX_ANGLE;
	static const float MIN_ANGLE;
	bool isMoveAnimation = false;
	//Enemy��Update�ŌĂяo��
	void UpdateMoveAnimationData(const Vector3& vel);

	//����A�j���[�V����
	void UpdateDeadAnimationData();

	void LockPlayer();
#pragma endregion

#pragma region ���ʃp�����[�^
	static const int GENERATE_COUNT = 5000;
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
	static const float OBJSIZE;

	int attackAfterTimer = 60 * 2;

	Vector3 angle = 0;
#pragma endregion

	//�������ƍX�V�͊e�N���X�ŃI�[�o�[���C�h
public:
	
	Enemy();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void loadModel();

	void setPPlayer(Player* p) { pPlayer = p; };
	virtual void setPosition(Vector3 pos) = 0;

	static bool isGameStart();

};

