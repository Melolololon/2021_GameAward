#pragma once
#include"Object.h"
#include"Player.h"
#include<string>

#include"FreamTimer.h"

class Enemy : public Object
{
private:
	//倒した数
	static int deadCount;

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
	static const int GENERATE_COUNT = 50;

	float moveSpeed = 0.10f;
	int hp = 3;
	static const float OBJSIZE;

	int attackAfterTimer = 60 * 2;

	Vector3 angle = 0;

	FreamTimer* pGameTime;

	int decreaseTime = 4;

	
	//点滅
	FreamTimer flashingTimer;
	const int FLASHING_TIME = 60 * 0.3f;


#pragma endregion

	//点滅
	void FlashingTimerMaxCheck();

	//初期化と更新は各クラスでオーバーライド


	//経路関係
	//A*で求めたプレイヤーへのベクトル
	std::vector<Vector2>toPlayerRouteVectors;
	void CalcToPlayerRouteVectors();

public:
	
	Enemy();
	virtual ~Enemy();

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
	void setPGameTime(FreamTimer* pGameTime) { this->pGameTime = pGameTime; }
	virtual void setPosition(Vector3 pos) = 0;

	static bool isGameStart();

	static void ResetDeadCount() { deadCount = 0; }
	static int GetDeadCount() { return deadCount; }
};

