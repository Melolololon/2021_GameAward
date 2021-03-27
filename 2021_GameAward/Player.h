#pragma once
#include"Object.h"
class Player :public Object
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



	//�{�[�����W(obj�ɋL�q����Ă�ʒu�B�����蔻��p�B����+bonePos������Ɏg�p����ʒu)
	static std::vector<Vector3> initialBonePos;

	//�{�[�����W(initial����ǂ̂��炢�����Ă邩)
	std::vector<Vector3> boneMovePos;


#pragma region ��]
	//�x�������ǂ���
	bool rotateFlag;
	//��]���x(1�t���[���ɉ�]����p�x)
	int rotateSpeed;
	//�Ђ˂�p�x
	std::vector<int>twistAngles;
	//��]��x�������邽�߂̃^�C�}�[
	UINT tienTimer;
	//�x�������鎞��(���̃{�[������]������t���[��)
	UINT tienTime;
	//��]����p�x(��]�����p�x������ɒB�������]�I��)
	int pushRotateAngle;
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

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void loadModel();
	
	/// <summary>
	/// ���l�ǂݍ���
	/// </summary>
	void loadParam();

};

