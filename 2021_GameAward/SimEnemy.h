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

#pragma region �ǉ��p�����[�^
	bool isDeadOneself = false;
	//����2�̂����񂾂��ǂ������擾���邽�߂̂���
	std::shared_ptr<SimEnemy> other0 = nullptr;
	std::shared_ptr<SimEnemy> other1 = nullptr;
	//����ID�Œ��S����̋��������߂� 0�`2
	int id = -1;
	Vector3 personalPos;
	//1�̂̂ݓ|�����Ƃ��ɃJ�E���g����^�C�}�[�@���ꂪ0�ɂȂ�ƕ���
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
/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
/// </summary>
	void setHeapNum()override;
	void setPosition(Vector3 pos);

};