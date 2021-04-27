#pragma once
#include "Enemy.h"
class MoveEnemy :
    public Enemy
{
private:
	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ

#pragma endregion

public:
	MoveEnemy();
	static void loadModel();
	void initialize()override;
	void update()override;
	void draw()override;

	/// <summary>
/// 変数heapNum関係。initialize時に必ず呼び出す。
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};

