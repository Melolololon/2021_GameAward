#pragma once
#include <Object.h>
class Block :
	public Object
{

private:


#pragma region モデル関係

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// 変数heapNum関係。initialize時に必ず呼び出す。
	/// </summary>
	void setHeapNum();


#pragma endregion



	Vector3 scale;

	static bool gameStart;
	
	

public:
	Block(const Vector3& pos ,const Vector3& scale);
	~Block();
	void initialize()override;
	void update()override;
	void draw()override;
	void hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;
	void* getPtr()override;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void loadModel();

	static void setGameStart(const bool& flag) { gameStart = flag; }


};

