#pragma once
#include "Enemy.h"
class ShotEnemy :
	public Enemy
{
private:
	static PrimitiveModel modelData;
	static int createCount;
	static HeapIndexManager heapIndexManager;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	//球を撃つ間隔
	float shotInterval = 3;
	int shotWaitTimer = 60;
#pragma endregion

public:
	ShotEnemy();
	~ShotEnemy();
	static void LoadResource();
	void Initialize()override;
	void Update()override;
	void Draw()override;

	/// <summary>
/// 変数heapNum関係。initialize時に必ず呼び出す。
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};