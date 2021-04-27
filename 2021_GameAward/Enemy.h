#pragma once
#include"Object.h"
#include"Player.h"

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
#pragma endregion

#pragma region ���ʃp�����[�^
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
	static const float OBJSIZE;

#pragma endregion

	//�������ƍX�V�͊e�N���X�ŃI�[�o�[���C�h
public:
	void initialize()override;
	void update()override;
	void draw()override;
	void hit
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
};

