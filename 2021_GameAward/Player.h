#pragma once
#include"Object.h"
#include"ObjModel.h"
#include"HeapIndexManager.h"
class Player :public Object
{
public:


private:


#pragma region モデル関係

	static ObjModel modelData;
	static const int CREATE_NUMBER;
	int heapNum;
	static HeapIndexManager playerModelHeapIndexManager;
	
	static int boneNum;

	Vector3 scale;
#pragma endregion


#pragma region 移動
	//モデル自体のベクトル
	Vector3 modelMoveVector = 0;

	//ボーン座標(initialからどのくらい動いてるか)
	std::vector<Vector3> boneMovePos;
	//移動による回転量
	std::vector<float> moveRotateAngle;

	//向きの回転量?
	float velRot;

	float previousRot;

	float initSpeed;//スピード初期値

	std::vector<Vector3>boneVelocity;

	//ボーン座標(objに記述されてる位置。あたり判定用。これ+bonePosが判定に使用する位置)
	static std::vector<Vector3> initialBonePos;
	static std::vector<Vector3> initialBonePosMulScale;//スケール乗算版

	//initial&movePos
	std::vector<Vector3> bonePos;

#pragma region 祠を囲む回転
	//ターゲットの周りをまわっているかどうか
	bool targetRotatePlayer;
#pragma endregion

#pragma region 祠の周りを回転するときのターゲット情報
	std::vector<Vector3>targetPos;
	int lockTargetNum;
	bool lockTarget = false;
#pragma endregion

#pragma region ステージセレクト時
	float stageSelectSpeedMag;
	int selectStageNum = 0;
	//bool selectStage;//ステージ選択したかどうか
#pragma endregion


#pragma endregion

#pragma region 回転(捻り)
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

#pragma region 弾

	int shotTimer;
	int shotTime;

#pragma endregion

#pragma region パラメーター
	int hp;

	bool isMuteki;
	int mutekiTimer;
	int mutekiTime;
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

	void PlayMove();
	void StageSelectMove();
	
	void DamageFromEnemy();

	/// <summary>
	/// モデル読み込み
	/// </summary>
	static void LoadResource();

#pragma region ゲッター
	Vector3 GetHeadPosition() { return initialBonePos[0] + boneMovePos[0] + modelMoveVector; }
	int GetHp() { return hp; }

	bool GetTargetRotatePlayer() { return targetRotatePlayer; }
	int GetLockTargetNum() { return lockTargetNum; }

#pragma endregion

#pragma region セッター
	//囲むための配列を渡す関数
	void SetTargetPosition(const std::vector<Vector3>& pos) { targetPos = pos; }
	void SetSelectStageNum(const int num) { selectStageNum = num; }
	void SetModelMoveVector(const Vector3& vector);
	//void SetSelectStage(const bool flag) { selectStage = flag; }
#pragma endregion


};

