#pragma once
#include"Object.h"
class Player :public Object
{
private:

#pragma region モデル関係

	static ModelData modelData;
	static int createCount;
	static const int CREATE_NUMBER;
	int heapNum;

	/// <summary>
	/// 変数heapNum関係。initialize時に必ず呼び出す。
	/// </summary>
	void setHeapNum();
#pragma endregion



	//ボーン座標(objに記述されてる位置。あたり判定用。これ+bonePosが判定に使用する位置)
	static std::vector<Vector3> initialBonePos;

	//initial&movePos
	std::vector<Vector3> bonePos;

#pragma region 移動
	//ボーン座標(initialからどのくらい動いてるか)
	std::vector<Vector3> boneMovePos;
	//移動による回転量
	std::vector<float> moveRotateAngle;

	//向きの回転量?
	float velRot;

	float previousRot;
#pragma endregion

#pragma region 回転
	//遅延中かどうか
	bool rotateFlag;
	//回転速度(1フレームに回転する角度)
	float rotateSpeed;
	//ひねり角度
	std::vector<float>twistAngles;
	//回転を遅延させるためのタイマー
	int tienTimer;
	//遅延させる時間(次のボーンを回転させるフレーム)
	int tienTime;
	//回転する角度(回転した角度がこれに達したら回転終了)
	float pushRotateAngle;
#pragma endregion

#pragma region パラメーター
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
	/// モデル読み込み
	/// </summary>
	static void loadModel();
	
#pragma region ゲッター
	Vector3 getHeadPosition() {return initialBonePos[0] + boneMovePos[0] + position; }
#pragma endregion


};

