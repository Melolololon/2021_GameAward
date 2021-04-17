#pragma once
#include"Object.h"
#include"Player.h"

class Enemy : public Object
{
protected:
#pragma region モデル関係

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;
	//プレイヤーのポインタ
	std::shared_ptr<Player> pPlayer;

	/// <summary>
	/// 変数heapNum関係。initialize時に必ず呼び出す。
	/// </summary>
	void setHeapNum();
#pragma endregion

#pragma region 共通パラメータ
	Vector3 position;
	Vector3 velocity;
	float moveSpeed = 0.15f;
	int hp = 3;
#pragma endregion

	//初期化と更新は各クラスでオーバーライド
public:
	void initialize()override;
	void update()override;
	void draw()override;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void loadModel();

	void setPPlayer(std::shared_ptr<Player> p) { pPlayer = p; };
	void setPosition(Vector3 pos);
};

