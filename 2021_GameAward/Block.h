#pragma once
#include <Object.h>
#include"HeapIndexManager.h"
#include"Player.h"
class Block :
	public Object
{

private:


#pragma region ���f���֌W

	static PrimitiveModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;

	static HeapIndexManager heapIndexManager;



#pragma endregion

	static Player* pPlayer;

	//�X�e�[�W�Z���N�g�p�ϐ�
	//���Ԗڂ̃X�e�[�W�̃u���b�N��
	int stageNum = 0;
	//�v���C���[�Ɍ����ꂽ�u���b�N��stageNum(�X�e�[�W�w��p)
	static int hitStageNum;


	Vector3 scale = 0;




public:
	Block(const Vector3& pos, const Vector3& scale,const int stageNum);
	Block(const Vector3& pos, const Vector3& scale);
	~Block();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;
	const void* GetPtr()const override;

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void LoadResource();


	void MovePosition(const Vector3& vector);

	static void ResetHitStageNum() { hitStageNum = -1; }

	static int GetHitStageNum() {return hitStageNum;}

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

