#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	//逃げる時間
	int escapeTimer = 300;
#pragma endregion

public:
	FleeEnemy();
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