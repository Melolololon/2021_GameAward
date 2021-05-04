#pragma once
#include <Object.h>
#include"HeapIndexManager.h"
class Block :
	public Object
{

private:


#pragma region ÉÇÉfÉãä÷åW

	static PrimitiveModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;

	static HeapIndexManager heapIndexManager;



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
	/// ÉÇÉfÉãì«Ç›çûÇ›
	/// </summary>
	static void LoadResource();


	void MovePosition(const Vector3& vector);
};

