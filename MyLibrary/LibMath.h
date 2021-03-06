#pragma once
#include"Vector.h"
#include"Matrix.h"
#include"Quaternion.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"Library.h"
#include"CollisionType.h"
//GameにMathクラスがあったから名前をMathにするとエラーが出る
//Math意外だとエラー出なかったので、Mathという名前自体がいけないのかもしれない
//エラー出ないこともある
//includeすると確実にエラーが出る。includeしないと出ない
//lib(スタティックライブラリのプロジェクト)に作らなければエラー出ない?



//Aスターのノード
struct AStarNode
{
	//座標
	Vector2 position = 0;

	//コスト
	UINT cost = 1;

	//計算結果(ステップ + 距離 + コスト)
	UINT calcNum = UINT_MAX;


	//以下探索用変数

	//配列のインデックス
	int indexX = INT_MAX;
	int indexY = INT_MAX;

	//close配列のインデックス
	int closeIndex = INT_MAX;

	AStarNode* previousNode = nullptr;

	bool openFlag = false;
	bool closeFlag = false;

	//進行不能オブジェクトと重なっているノード
	bool hitObjectNode = false;
};



class LibMath
{
private:

public:

#pragma region 最短経路

	/// <summary>
	/// 引数を基準にノードの座標をセットします。
	/// </summary>
	/// <param name="leftUpPos">左上座標</param>
	/// <param name="rightDownPos">右下座標</param>
	/// <param name="nodeNumX">横分割数</param>
	/// <param name="nodeNumY">縦分割数</param>
	/// <param name="nodes">ノードのvector(sizeは0でよい)</param>
	/// <param name="upPlus">上方向がプラスかどうか</param>
	static void SetAStarNodePosition
	(
		const Vector2& leftUpPos,
		const Vector2& rightDownPos,
		const UINT& nodeNumX,
		const UINT& nodeNumY,
		std::vector< std::vector<AStarNode>>& nodes,
		const bool upPlus
	);

	//この関数でコストが1じゃないオブジェクトとそのコストを渡すようにする?
	/// <summary>
	/// ノードが進行不能オブジェクトにヒットしてるかを確認します。
	/// </summary>
	/// <param name="hitObjectsPos">障害物の座標のvector</param>
	/// <param name="hitObjectsSize">障害物のサイズのvector</param>
	/// <param name="nodes">ノードのvector(SetAStarNodePositionに渡した後の配列)</param>
	static void SetAStarNodeHitObjectNodeFlag
	(
		const std::vector<Vector2>& hitObjectsPos,
		const std::vector<Vector2>& hitObjectsSize,
		std::vector< std::vector<AStarNode>>& nodes
	);

	/// <summary>
	/// 渡されたデータをもとに最短経路を求めます。
	/// </summary>
	/// <param name="startPos">スタート地点の座標</param>
	/// <param name="endPos">ゴール地点の座標</param>
	/// <param name="nodes">ノードのvector(SetAStarNodeHitObjectNodeFlagに渡した後の配列)</param>
	/// <param name="routeVector">ゴールまでのルートを格納するvector(sizeは0でよい)</param>
	/// <returns>探索が成功したかどうか</returns>
	static bool GetAStarCalcResult
	(
		const Vector2& startPos,
		const Vector2& endPos,
		std::vector< std::vector<AStarNode>>& nodes,
		std::vector<Vector2>& routeVector
	);

#pragma endregion



	/// <summary>
	/// num1とnum2の値の差を求め、差が基準の値より多かったらtrueを返します
	/// </summary>
	/// <param name="num1">値1</param>
	/// <param name="num2">値2</param>
	/// <param name="difference">基準の値</param>
	/// <returns></returns>
	static bool Difference(const float num1, const float num2, const float difference);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="angle1">角度(0から359)</param>
	/// <param name="angle2">角度(0から359)</param>
	/// <param name="difference"></param>
	/// <returns></returns>
	static bool AngleDifference(const float angle1, const float angle2, const float difference);

	/// <summary>
		/// 度数法の角度をラジアンに　ラジアンを度数法の角度に
		/// </summary>
		/// <param name="pattern">0 度数法の角度をラジアンに   1 ラジアンを度数法の角度に</param>
		/// <param name="angle">角度</param>
	static float AngleConversion(int pattern, float angle);


	/// <summary>
	/// 円周率を取得します
	/// </summary>
	/// <returns></returns>
	static float GetFloatPI();

	/// <summary>
	/// 円周率を取得します
	/// </summary>
	/// <returns></returns>
	static double GetDoublePI();

#pragma region ベクトル


#pragma region vector2

	/// <summary>
/// 2つの座標の距離を取得します
/// </summary>
/// <param name="pos1">座標1</param>
/// <param name="pos2">座標2</param>
/// <returns></returns>
	static float CalcDistance2D(const Vector2& pos1, const Vector2& pos2);


	/// <summary>
	/// 左右判定を行います。点がベクトルより右の場合は1、左の場合は-1、ベクトル上の場合は0を返します。
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static char PointLeftRightCheck(const Vector2& vector, const Vector2& point);

	/// <summary>
	/// 2つのベクトルがなす角度を求めます
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float TwoVector2Angle(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// ベクトルを角度に変換します
	/// </summary>
	/// <param name="v"></param>
	/// <param name="v3">3次元座標系かどうか</param>
	/// <returns></returns>
	static float Vecto2ToAngle(const Vector2& v, const bool& v3);

	/// <summary>
	/// 角度をVector2に変換します
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="v3">3次元座標系かどうか</param>
	/// <returns></returns>
	static Vector2 AngleToVector2(const float& angle, const bool& v3);

	/// <summary>
	/// Vector2のベクトルを指定した角度に応じて回転させます
	/// </summary>
	/// <param name="v"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Vector2 RotateVector2(const Vector2& v, const float& angle);
#pragma endregion

#pragma region Vector3

	/// <summary>
	/// 2つの座標の距離を取得します
	/// </summary>
	/// <param name="pos1">座標1</param>
	/// <param name="pos2">座標2</param>
	/// <returns></returns>
	static float CalcDistance3D(Vector3 pos1, Vector3 pos2);

	/// <summary>
	/// vex1からvec2のベクトルを取得します(正規化は行う)
	/// </summary>
	/// <param name="myPos"></param>
	/// <param name="otherPos"></param>
	/// <returns></returns>
	static Vector3 OtherVector(const Vector3& vec1, const Vector3& vec2);

	/// <summary>
	/// ベクトルを回転させます
	/// </summary>
	/// <param name="rotateV"></param>
	/// <param name="vec">軸のベクトル</param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Vector3 RotateVector3(const Vector3& rotateV, const Vector3& vec, const float& angle);


	/// <summary>
	/// 座標を指定したベクトル方向にdistance分移動させたVector3型の値を返します。
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vector"></param>
	/// <param name="distance"></param>
	/// <returns></returns>
	static Vector3 FloatDistanceMoveVector3(const Vector3& pos, const Vector3& vector, const float distance);
#pragma endregion


#pragma endregion

#pragma region 当たり判定

#pragma region 2D

	/// <summary>
	/// 四角の当たり判定です
	/// </summary>
	/// <param name="start1">1つ目の左上座標</param>
	/// <param name="end1">1つ目の右下上座標</param>
	/// <param name="start2">1つ目の左上座標</param>
	/// <param name="end2">1つ目の右下座標</param>
	/// <returns>当たったかどうか</returns>
	static bool RectAndRectCollision
	(
		const Vector2& pos1,
		const Vector2& size1,
		const Vector2& pos2,
		const Vector2& size2
	);

	/// <summary>
	/// 円の当たり判定です
	/// </summary>
	/// <param name="centerPosition1">1つ目の座標</param>
	/// <param name="r1">1つ目の半径</param>
	/// <param name="centerPosition2">1つ目の座標</param>
	/// <param name="r2">1つ目の円</param>
	/// <returns>当たったかどうか</returns>
	static bool CircleCollision(Vector2 centerPosition1, float r1, Vector2 centerPosition2, float r2);

#pragma endregion

#pragma region 3D
	/// <summary>
	/// 球同士の当たり判定
	/// </summary>
	/// <param name="centerPosition1">1つ目の座標</param>
	/// <param name="r1">1つ目の半径</param>
	/// <param name="centerPosition2">1つ目の座標</param>
	/// <param name="r2">1つ目の円</param>
	/// <returns>当たったかどうか</returns>
	static bool SphereAndSphereCollision(Vector3 centerPosition1, float r1, Vector3 centerPosition2, float r2);

	/// <summary>
	/// 球と平面の座標
	/// </summary>
	/// <param name="spherePos"></param>
	/// <param name="r"></param>
	/// <param name="normal">平面の法線ベクトル</param>
	/// <param name="planeDist">平面の原点から平面の距離(マイナス可能)</param>
	/// <param name="hitPos"></param>
	/// <returns></returns>
	static bool SphereAndPlaneCollision
	(
		Vector3 spherePos,
		float r,
		Vector3 normal,
		float planeDist,
		Vector3* hitPos
	);

	/// <summary>
	/// 球と平らな三角形の判定
	/// </summary>
	/// <param name="spherePos"></param>
	/// <param name="r"></param>
	/// <param name="triPos1"></param>
	/// <param name="triPos2"></param>
	/// <param name="triPos3"></param>
	/// <param name="normal"></param>
	/// <param name="hitPos"></param>
	/// <returns></returns>
	static bool SphereAndTryangleCollision
	(
		Vector3 spherePos,
		float r,
		Vector3 triPos1,
		Vector3 triPos2,
		Vector3 triPos3,
		Vector3 normal,
		Vector3* hitPos
	);

	/// <summary>
	/// 球と立方体(AABB)の判定
	/// </summary>
	/// <param name="spherePos"></param>
	/// <param name="r"></param>
	/// <param name="minPos"></param>
	/// <param name="maxPos"></param>
	/// <param name="direction">球は、箱のどこに当たったか。箱は球が自分のどこにぶつかったかを返す</param>
	/// <returns></returns>
	static bool SphereAndBoxCollision
	(
		const Vector3& spherePos,
		const float& r,
		const Vector3& centerPos,
		const Vector3& size,
		BoxHitDirection* direction
	);

	/// <summary>
	/// 立方体(AABB)同士の判定
	/// </summary>
	/// <param name="centerPos1"></param>
	/// <param name="size1"></param>
	/// <param name="centerPos2"></param>
	/// <param name="size2"></param>
	/// <param name="direction1">自分のどこがぶつかったかを返す</param>
	/// <param name="direction2">自分のどこがぶつかったかを返す</param>
	/// <returns></returns>
	static bool BoxAndBoxCollision
	(
		const Vector3& centerPos1,
		const Vector3& size1,
		const Vector3& centerPos2,
		const Vector3& size2,
		BoxHitDirection* direction1,
		BoxHitDirection* direction2
	);

	/// <summary>
	/// 線分と板の当たり判定
	/// </summary>
	/// <param name="linePos1">線1</param>
	/// <param name="linePos2">線2</param>
	/// <param name="normal">板の法線ベクトル</param>
	/// <param name="pointPos">板の座標</param>
	/// <param name="vertexPoint">頂点の座標4つ(回転など適応後)(回転前の左下、左上、右下、右上の順で配列に入れる)</param>
	/// <param name="crossPos">線とポリゴンがぶつかった場所</param>
	/// /// <returns></returns>
	static bool LineSegmentAndBoardCollision
	(
		Vector3 linePos1,
		Vector3 linePos2,
		Vector3 normal,
		Vector3 pointPos,
		std::vector<Vector3>vertexPoint,
		Vector3* crossPosition
	);

	/// <summary>
	/// 半直線と平面の
	/// </summary>
	/// <param name="layStartPos"></param>
	/// <param name="layDirection"></param>
	/// <param name="normal"></param>
	/// <param name="planeDist"></param>
	/// <param name="distance"></param>
	/// <param name="crossPos"></param>
	/// <returns></returns>
	static bool RayAndPlaneCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 normal,
		float planeDistance,
		float* distance,
		Vector3* crossPos
	);

	static bool RayAndTryangleCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 triPos1,
		Vector3 triPos2,
		Vector3 triPos3,
		Vector3 normal,
		float* distance,
		Vector3* crossPos
	);

	/// <summary>
	/// 半直線と球の当たり判定
	/// </summary>
	static bool RayAndSphereCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 spherePos,
		float r,
		float* distance,
		Vector3* crossPos
	);
#pragma endregion

#pragma endregion


};

