#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	//逃げる時間
	int escapeTimer = 300;
#pragma endregion

public:
	FleeEnemy();
	static void loadModel();
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
/// 変数heapNum関係。initialize時に必ず呼び出す。
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};