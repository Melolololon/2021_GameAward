#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region �ǉ��p�����[�^

#pragma endregion

public:
	MoveEnemy();
	static void LoadResource();
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
/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};

