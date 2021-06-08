#pragma once
#include <Object.h>
#include"HeapIndexManager.h"
#include"Player.h"
class Block :
	public Object
{

private:


#pragma region ÉÇÉfÉãä÷åW

	static ObjModel treeModelData;

	static const int CREATE_NUMBER;
	std::vector<int> heapNums;

	static HeapIndexManager heapIndexManager;

	void SetModel();

#pragma endregion


	Vector3 scale = 0;
	Vector3 treeScale = 4;



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
	/// ÉÇÉfÉãì«Ç›çûÇ›
	/// </summary>
	static void LoadResource();


	void MovePosition(const Vector3& vector);


};

