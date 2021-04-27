#pragma once
#include"Object.h"
#include"Player.h"

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
#pragma endregion

#pragma region 共通パラメータ
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
	static const float OBJSIZE;

#pragma endregion

	//初期化と更新は各クラスでオーバーライド
public:
	void initialize()override;
	void update()override;
	void draw()override;
	void hit
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
};

