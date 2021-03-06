#pragma once
#include"Object.h"
#include"HeapIndexManager.h"
class PlayerBullet :public Object
{
private:

#pragma region ���f���֌W

	static ObjModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;

	static HeapIndexManager heapIndexManager;

#pragma endregion

	int deadTimer;
	int deadTime;

	Vector3 scale;

	bool IsEnemy(const Object* p);

public:
	PlayerBullet(const Vector3& pos, const Vector3& vel);
	~PlayerBullet();



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

