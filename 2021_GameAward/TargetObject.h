#pragma once
#include"Object.h"

//�j��Ώ�(�K)
class TargetObject :public Object
{


private:
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;
	static PrimitiveModel modelData;
	static std::vector<Sprite3D> lifeGaugeSprite;
	static std::vector<Sprite3D> lifeGaugeFreamSprite;
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
	static void LoadResource();


	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	void trueEraseManager();
	void trueSetEnd();
	bool getSetEnd();
	bool getCreateHitObject();

	void setPosition(const Vector3& pos);

};

