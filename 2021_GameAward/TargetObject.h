#pragma once
#include"Object.h"
#include"HeapIndexManager.h"
#include"PipelineState.h"
//破壊対象(祠)
class TargetObject :public Object
{
private:

	static const int CREATE_NUMBER;
	int heapNum;
	static ObjModel modelData;
	static std::vector<Sprite3D> hpGaugeSprite;
	static std::vector<Sprite3D> hpGaugeFreamSprite;
	static Texture hpGaugeTexture;
	static Texture hpGaugeFreamTexture;
	static PipelineState hpPipeline;

	static HeapIndexManager heapIndexManager;

	//体力
	int hp;
	static int maxHp;
	static float hpGaugeOneNumSizeX;

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

	void DrawHp();


	void TrueEraseManager();
	void TrueSetEnd();
	bool GetSetEnd();
	bool GetCreateHitObject();

	void SetPosition(const Vector3& pos);

};

