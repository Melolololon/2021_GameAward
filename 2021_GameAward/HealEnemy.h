#pragma once
#include "Enemy.h"
class HealEnemy :
	public Enemy
{
private:
	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region �ǉ��p�����[�^
	//�������Ԋu
	float shotInterval = 3;
	int shotWaitTimer = 60;
#pragma endregion

public:
	HealEnemy();
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