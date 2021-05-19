#pragma once
#include "Enemy.h"
class SimEnemy :
	public Enemy
{
private:
	static ObjModel modelData;
	static int createCount;
	static HeapIndexManager heapIndexManager;
	static const int CREATE_NUMBER;

#pragma region 追加パラメータ
	bool isDeadOneself = false;
	//他の2体が死んだかどうかを取得するためのもの
	std::shared_ptr<SimEnemy> other0 = nullptr;
	std::shared_ptr<SimEnemy> other1 = nullptr;
	//このIDで中心からの距離を決める 0～2
	int id = -1;
	Vector3 personalPos;
	//1体のみ倒したときにカウントするタイマー　これが0になると復活
	int partDeadTimer = 60 * 0.5;
#pragma endregion

public:
	SimEnemy();
	~SimEnemy();
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

	void SetOther(int num, std::shared_ptr<SimEnemy> p);
	void SetID(int id) { this->id = id; }

	/// <summary>
/// 変数heapNum関係。initialize時に必ず呼び出す。
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos);

};