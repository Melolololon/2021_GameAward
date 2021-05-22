#pragma once
#include"Object.h"
#include"Player.h"
#include<string>

#include"FreamTimer.h"

class Enemy : public Object
{

protected:
#pragma region モデル関係

	int heapNum;
	//プレイヤーのポインタ
	Player* pPlayer;

	/// <summary>
	/// 変数heapNum関係。initialize時に必ず呼び出す。
	/// </summary>
	virtual void setHeapNum() = 0;


	//歩行アニメーション用
	static const int MOVE_ANIMATION_MAX = 60 * 0.7;
	FreamTimer moveAnimationTime;
	Vector3 leftFootAngle;
	Vector3 rightFootAngle;
	static const float FREAM_MOVE_ANGLE;
	static const float MAX_ANGLE;
	static const float MIN_ANGLE;
	bool isMoveAnimation = false;
	//EnemyのUpdateで呼び出す
	void UpdateMoveAnimationData(const Vector3& vel);

	//やられアニメーション
	void UpdateDeadAnimationData();

	void LockPlayer();
#pragma endregion

#pragma region 共通パラメータ
	static const int GENERATE_COUNT = 5000;
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
	static const float OBJSIZE;

	int attackAfterTimer = 60 * 2;

	Vector3 angle = 0;
#pragma endregion

	//初期化と更新は各クラスでオーバーライド
public:
	
	Enemy();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void loadModel();

	void setPPlayer(Player* p) { pPlayer = p; };
	virtual void setPosition(Vector3 pos) = 0;

	static bool isGameStart();

};

