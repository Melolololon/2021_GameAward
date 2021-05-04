#pragma once
#include <Object.h>
class Block :
	public Object
{

private:


#pragma region ���f���֌W

	static PrimitiveModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void SetHeapNum();


#pragma endregion



	Vector3 scale;




public:
	Block(const Vector3& pos, const Vector3& scale);
	~Block();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;
	void* GetPtr()override;

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void LoadResource();


	void MovePosition(const Vector3& vector);
};

