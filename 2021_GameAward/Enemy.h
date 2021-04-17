#pragma once
#include"Object.h"
#include"Player.h"

class Enemy : public Object
{
protected:
#pragma region ���f���֌W

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;
	//�v���C���[�̃|�C���^
	std::shared_ptr<Player> pPlayer;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void setHeapNum();
#pragma endregion

#pragma region ���ʃp�����[�^
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
#pragma endregion

	//�������ƍX�V�͊e�N���X�ŃI�[�o�[���C�h
public:
	void initialize()override;
	void update()override;
	void draw()override;

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void loadModel();

	void setPPlayer(std::shared_ptr<Player> p) { pPlayer = p; };
	void setPosition(Vector3 pos);
};

