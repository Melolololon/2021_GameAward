#pragma once
#include"Object.h"
#include"ObjModel.h"
#include"HeapIndexManager.h"
class Player :public Object
{
public:


private:


#pragma region ���f���֌W

	static ObjModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;
	static HeapIndexManager playerModelHeapIndexManager;
	
	static int boneNum;

	Vector3 scale;
#pragma endregion

#pragma region �ړ�
	//�{�[�����W(initial����ǂ̂��炢�����Ă邩)
	std::vector<Vector3> boneMovePos;
	//�ړ��ɂ���]��
	std::vector<float> moveRotateAngle;

	//�����̉�]��?
	float velRot;

	float previousRot;

	float initSpeed;//�X�s�[�h�����l

	std::vector<Vector3>boneVelocity;

	//�{�[�����W(obj�ɋL�q����Ă�ʒu�B�����蔻��p�B����+bonePos������Ɏg�p����ʒu)
	static std::vector<Vector3> initialBonePos;
	static std::vector<Vector3> initialBonePosMulScale;//�X�P�[����Z��

	//initial&movePos
	std::vector<Vector3> bonePos;

#pragma region �K���͂މ�]
	//�^�[�Q�b�g�̎�����܂���Ă��邩�ǂ���
	bool targetRotatePlayer;
#pragma endregion


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

#pragma region �K�̍��W(��]�p)
	static std::vector<Vector3>targetObjectPos;
#pragma endregion




public:
	Player();
	Player(const Vector3& pos);
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
	

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	static void LoadResource();

#pragma region �Q�b�^�[
	Vector3 GetHeadPosition() { return initialBonePos[0] + boneMovePos[0] + position; }
	int GetHp() { return hp; }
#pragma endregion

#pragma region �Z�b�^�[
	static void SetTargetObjectPosition(const std::vector<Vector3>& pos) { targetObjectPos = pos; }
#pragma endregion


};

