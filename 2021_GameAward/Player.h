#pragma once
#include"Object.h"
class Player:public Object
{
private:

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUM;
	void setHeapNum();
	int heapNum;

	



	//�{�[�����W(obj�ɋL�q����Ă�ʒu�B�����蔻��p�B����+bonePos������Ɏg�p����ʒu)
	static std::vector<Vector3> initialBonePos;
	
	//�{�[�����W(initial����ǂ̂��炢�����Ă邩)
	std::vector<Vector3> boneMovePos;


#pragma region ��]
	//�x�������ǂ���
	bool rotateFlag;

	UINT tienFream;
	int rotateSpeed;

	//�Ђ˂�p�x
	std::vector<int>twistAngles;
	std::vector<UINT>tienTimer;

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

