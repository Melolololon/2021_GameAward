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

	//initial&movePos
	std::vector<Vector3> bonePos;

#pragma region �ړ�
	//�{�[�����W(initial����ǂ̂��炢�����Ă邩)
	std::vector<Vector3> boneMovePos;
	//�ړ��ɂ���]��
	std::vector<float> moveRotateAngle;

	//�����̉�]��?
	float velRot;

	float previousRot;
#pragma endregion

#pragma region ��]
	//�x�������ǂ���
	bool rotateFlag;
	//��]���x(1�t���[���ɉ�]����p�x)
	float rotateSpeed;
	//�Ђ˂�p�x
	std::vector<float>twistAngles;
	//��]��x�������邽�߂̃^�C�}�[
	int tienTimer;
	//�x�������鎞��(���̃{�[������]������t���[��)
	int tienTime;
	//��]����p�x(��]�����p�x������ɒB�������]�I��)
	float pushRotateAngle;
#pragma endregion

#pragma region �p�����[�^�[
	int hp;
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
	
#pragma region �Q�b�^�[
	Vector3 getHeadPosition() {return initialBonePos[0] + boneMovePos[0] + position; }
#pragma endregion


};

