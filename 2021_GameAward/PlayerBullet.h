#pragma once
#include"Object.h"
class PlayerBullet :public Object
{
private:

#pragma region ���f���֌W

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void setHeapNum();
#pragma endregion

	int deadTimer;
	int deadTime;

public:
	PlayerBullet(const Vector3& pos,const Vector3& vel);
	~PlayerBullet();



	static void loadModel();

	void initialize()override;
	void update()override;
	void draw()override;
	void hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

};

