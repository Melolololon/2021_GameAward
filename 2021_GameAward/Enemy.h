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
	FreamTimer moveAnimationTimer;
#pragma endregion

#pragma region 共通パラメータ
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
	static const float OBJSIZE;

	int attackAfterTimer = 60 * 2;

#pragma endregion

	//初期化と更新は各クラスでオーバーライド
public:
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

