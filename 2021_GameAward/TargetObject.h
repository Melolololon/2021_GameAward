#pragma once
#include"Object.h"
#include"HeapIndexManager.h"

//破壊対象(祠)
class TargetObject :public Object
{


private:
	static const int CREATE_NUMBER;
	int heapNum;
	static ObjModel modelData;
	static std::vector<Sprite3D> lifeGaugeSprite;
	static std::vector<Sprite3D> lifeGaugeFreamSprite;
	static Texture lifeGaugeTexture;
	static Texture lifeGaugeFreamTexture;

	static HeapIndexManager heapIndexManager;

	//体力
	int hp;

	//セット完了したかどうか
	bool setEnd;
	//生成時にブロックやプレイヤーと重なってしまったかどうか
	bool createHitObject;

	static bool hitSegment;

public:
	TargetObject(const Vector3& pos);
	~TargetObject();

	/// <summary>
	/// モデル読み込み
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

