#pragma once
#include"Object.h"
class TargetObject :public Object
{


private:
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;
	static ModelData modelData;
	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void setHeapNum();

	//�̗�
	int hp;

	//�Z�b�g�����������ǂ���
	bool setEnd;
	//�������Ƀu���b�N��v���C���[�Əd�Ȃ��Ă��܂������ǂ���
	bool createHitObject;

	static bool hitSegment;

public:
	TargetObject(const Vector3& pos);
	~TargetObject();

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void loadModel();

	
	void initialize()override;
	void update()override;
	void draw()override;
	void hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	void trueEraseManager();
	void trueSetEnd();
	bool getSetEnd();
	bool getCreateHitObject();
	
	void setPosition(const Vector3& pos);

};

