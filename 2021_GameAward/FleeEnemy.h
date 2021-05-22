#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
	static ObjModel modelData;
	static int createCount;
	static HeapIndexManager heapIndexManager;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	//逃げる時間
	int escapeTimer = 300;
	Vector3 shiver;
#pragma endregion

public:
	FleeEnemy();
	~FleeEnemy();
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
/// 変数heapNum関係。initialize時に必ず呼び出す。
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};