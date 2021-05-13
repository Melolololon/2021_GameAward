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

#pragma region �ǉ��p�����[�^
	//�������Ԋu
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
/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos)override;
};