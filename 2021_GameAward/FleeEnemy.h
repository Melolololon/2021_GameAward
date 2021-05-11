#pragma once
#include "Enemy.h"
class FleeEnemy :
	public Enemy
{
private:
	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;

#pragma region �ǉ��p�����[�^
	//�����鎞��
	int escapeTimer = 300;
#pragma endregion

public:
	FleeEnemy();
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