#pragma once
#include"Object.h"
#include"HeapIndexManager.h"

//�j��Ώ�(�K)
class TargetObject :public Object
{


private:
	static const int CREATE_NUMBER;
	int heapNum;
	static ObjModel modelData;
	static std::vector<Sprite3D> hpGaugeSprite;
	static std::vector<Sprite3D> hpGaugeFreamSprite;
	static Texture lifeGaugeTexture;
	static Texture hpGaugeFreamTexture;

	static HeapIndexManager heapIndexManager;

	//�̗�
	int hp;
	static int maxHp;
	static float hpGaugeOneNumSizeX;

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

