#pragma once
#include "Enemy.h"
class ShotEnemy :
	public Enemy
{
private:
	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	//球を撃つ間隔
	float shotInterval = 3;
	int shotWaitTimer = 60;
#pragma endregion

public:
	ShotEnemy();
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