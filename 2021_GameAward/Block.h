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
	void setHeapNum();


#pragma endregion



	Vector3 scale;

	static bool gameStart;



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

	static void setGameStart(const bool& flag) { gameStart = flag; }

};

