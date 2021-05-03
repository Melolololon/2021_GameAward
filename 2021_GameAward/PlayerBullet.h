#pragma once
#include"Object.h"
class PlayerBullet :public Object
{
private:

#pragma region モデル関係

	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// 変数heapNum関係。initialize時に必ず呼び出す。
	/// </summary>
	void setHeapNum();
#pragma endregion

	int deadTimer;
	int deadTime;

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

