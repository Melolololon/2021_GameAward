#pragma once
#include"Object.h"
#include"ObjModel.h"
class Player :public Object
{
public:


private:

#pragma region ���f���֌W

	static ObjModel modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// �ϐ�heapNum�֌W�Binitialize���ɕK���Ăяo���B
	/// </summary>
	void SetHeapNum();
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


	std::vector<Vector3>boneVelocity;
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

#pragma region �e

	int shotTimer;
	int shotTime;

#pragma endregion

#pragma region �p�����[�^�[
	int hp;

	bool isMuteki;
	int mutekiTimer;
	int mutekiTime;
#pragma endregion



public:
	Player();
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;
	void* GetPtr()override;
	void SetHp(int hp) { this->hp = hp; }
	int GetHp() { return hp; }

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void LoadResource();

#pragma region �Q�b�^�[
	Vector3 GetHeadPosition() { return initialBonePos[0] + boneMovePos[0] + position; }
#pragma endregion


};

