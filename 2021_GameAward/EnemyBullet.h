#pragma once
#include"Object.h"
#include"Player.h"
class EnemyBullet :public Object
{
private:

#pragma region ���f���֌W

	static ObjModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void setHeapNum();
#pragma endregion

	int deadTimer;
	int deadTime;

public:
	EnemyBullet(const Vector3& pos,const Vector3& vel);
	~EnemyBullet();


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
};

