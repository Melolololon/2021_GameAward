#pragma once
#include"Object.h"
class Player :public Object
{
private:

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUM;
	void setHeapNum();
	int heapNum;





	//ボーン座標(objに記述されてる位置。あたり判定用。これ+bonePosが判定に使用する位置)
	static std::vector<Vector3> initialBonePos;

	//ボーン座標(initialからどのくらい動いてるか)
	std::vector<Vector3> boneMovePos;


#pragma region 回転
	//遅延中かどうか
	bool rotateFlag;
	//回転速度
	int rotateSpeed;
	//ひねり角度
	std::vector<int>twistAngles;
	//遅延させるためのタイマー
	UINT tienTimer;
	//遅延させる時間
	UINT tienFream;
	//1回押した時の回転する角度
	int pushRotateAngle;
	//float twistAngles;
#pragma endregion


public:
	Player();
	~Player();
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

	static void loadModel();
};

