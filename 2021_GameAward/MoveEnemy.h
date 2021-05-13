#pragma once
#include "Enemy.h"
class MoveEnemy :
	public Enemy
{
private:
	static ObjModel modelData;
	static HeapIndexManager heapIndexManager;
	static int createCount;
	static const int CREATE_NUMBER;


#pragma region 追加パラメータ
#pragma endregion

public:
	MoveEnemy();
	~MoveEnemy();
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

