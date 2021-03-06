#pragma once
#include <Object.h>
#include"HeapIndexManager.h"
#include"Player.h"
class Block :
	public Object
{

private:


#pragma region モデル関係

	static PrimitiveModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;

	static HeapIndexManager heapIndexManager;



#pragma endregion


	Vector3 scale = 0;




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
	const void* GetPtr()const override;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void LoadResource();


	void MovePosition(const Vector3& vector);


};

