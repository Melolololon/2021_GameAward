#include"LibMath.h"
#include"DirectInput.h"


#pragma region 最短経路

void LibMath::SetAStarNodePosition
(
	const Vector2& leftUpPos,
	const Vector2& rightDownPos,
	const UINT& nodeNumX,
	const UINT& nodeNumY,
	std::vector< std::vector<AStarNode>>& nodes,
	const bool upPlus
)
{
	//マスのサイズを求める
	Vector2 size = rightDownPos - leftUpPos;
	size.x = abs(size.x);
	size.y = abs(size.y);
	if (nodeNumX >= 1) size.x /= nodeNumX - 1;
	if (nodeNumY >= 1) size.y /= nodeNumY - 1;

	//ノードリサイズ
	nodes.resize(nodeNumY, std::vector<AStarNode>(nodeNumX));

	//ノードの座標をセットしていく
	Vector2 addPos = 0;
	for (int y = 0; y < nodeNumY; y++)
	{
		for (int x = 0; x < nodeNumX; x++)
		{
			if (upPlus)addPos = Vector2(size.x * x, size.y * -y);
			else addPos = Vector2(size.x * x, size.y * y);
			nodes[y][x].position = leftUpPos + addPos;
		}
	}

}

void LibMath::SetAStarNodeHitObjectNodeFlag
(
	const std::vector<Vector2>& hitObjectsPos,
	const std::vector<Vector2>& hitObjectsSize,
	std::vector< std::vector<AStarNode>>& nodes
)
{
	//サイズを求める
	Vector2 size = 0;
	size.x = abs(nodes[0][0].position.x - nodes[0][1].position.x);

	float smallPos = 0.0f;
	float bigPos = 0.0f;
	if (nodes[0][0].position.y >= nodes[1][0].position.y)
	{
		bigPos = nodes[0][0].position.y;
		smallPos = nodes[1][0].position.y;
	}
	else
	{
		bigPos = nodes[1][0].position.y;
		smallPos = nodes[0][0].position.y;
	}
	size.y = abs(smallPos - bigPos);

	//判定
	auto nodesXNum = nodes[0].size();
	auto nodesYNum = nodes.size();
	auto hitObjectNum = hitObjectsPos.size();

	//当たってるか確認
	Vector2 addPos = 0;
	for (int y = 0; y < nodesYNum; y++)
	{
		for (int x = 0; x < nodesXNum; x++)
		{
			for (int i = 0; i < hitObjectNum; i++)
			{
				nodes[y][x].hitObjectNode =
					LibMath::RectAndRectCollision
					(
						nodes[y][x].position,
						size,
						hitObjectsPos[i],
						hitObjectsSize[i]
					);

				//1つでもぶつかってたらリターン
				if (nodes[y][x].hitObjectNode)break;;
			}
		}
	}
}

bool LibMath::GetAStarCalcResult
(
	const Vector2& startPos,
	const Vector2& endPos,
	std::vector< std::vector<AStarNode>>& nodes,
	std::vector<Vector2>& routeVector
)
{

	//やること
	//openに再追加、closeからの移動(wikiの7.のところ)実装する
	//ブロックに隣接してるマスから、同じブロックに隣接してるマスへ移動しないようにする


	//リセット
	for (auto& n1 : nodes)
	{
		for (auto& n2 : n1)
		{
			n2.closeFlag = false;
			n2.openFlag = false;
			n2.previousNode = nullptr;
			n2.calcNum = UINT_MAX;
			n2.closeIndex = INT_MAX;
		}
	}

	//スタート地点に一番近いノードの距離を格納する変数
	float startMinDistance = FLT_MAX;
	//ゴール地点に一番近いノードの距離を格納する変数
	float endMinDistance = FLT_MAX;

	auto nodeXArrayNum = nodes[0].size();
	auto nodeYArrayNum = nodes.size();

	//スタートの添え字
	int startNodeIndexX = 0;
	int startNodeIndexY = 0;

	//ノードの配列のゴール地点の場所を示す添え字
	int endNodeIndexX = 0;
	int endNodeIndexY = 0;

	for (int y = 0; y < nodeYArrayNum; y++)
	{
		for (int x = 0; x < nodeXArrayNum; x++)
		{
			float distance = 0.0f;
			distance = CalcDistance2D(nodes[y][x].position, startPos);

			if (distance <= startMinDistance)
			{
				startMinDistance = distance;
				startNodeIndexX = x;
				startNodeIndexY = y;
			}

			distance = CalcDistance2D(nodes[y][x].position, endPos);
			if (distance <= endMinDistance)
			{
				endMinDistance = distance;
				endNodeIndexX = x;
				endNodeIndexY = y;
			}

			//インデックス代入
			nodes[y][x].indexX = x;
			nodes[y][x].indexY = y;

		}
	}


	//ゴールのノードまでの距離を求めるラムダ式
	auto CalcNodeDistance = []
	(
		const int startX,
		const int startY,
		const int endX,
		const int endY
		)
	{
		//Xの差
		int startXToEndXDiff = abs(startX - endX);
		//Yの差
		int startYToEndYDiff = abs(startY - endY);

		//Xの差とYの差のどちらが大きいか求める。斜め移動ありの場合、大きいほうが最短距離
		if (startXToEndXDiff >= startYToEndYDiff)return startXToEndXDiff;
		return startYToEndYDiff;

	};

	//スタートのノードのインデックスを代入
	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, endNodeIndexX, endNodeIndexY);

	nodes[startNodeIndexY][startNodeIndexX].calcNum = startToEndDis + nodes[startNodeIndexY][startNodeIndexX].cost;

	std::vector<AStarNode*>openNodes(1, &nodes[startNodeIndexY][startNodeIndexX]);
	std::vector<AStarNode*>closeNodes;


	AStarNode* endNode;


	//ステップの計算は周りのノード調べるときにやる?

	while (1)
	{
		//ゴールにたどり着けない場合
		if (openNodes.size() == 0) return false;

		//並び替え
		std::sort
		(
			openNodes.begin(),
			openNodes.end(),
			[]
		(
			const AStarNode* node1,
			const AStarNode* node2
			)
		{
			int num1 = node1->calcNum;
			int num2 = node2->calcNum;

			return num1 > num2;
		});

		//calcNumが一番少ないやつを取得
		AStarNode* mainNode = openNodes[openNodes.size() - 1];

		bool checkEnd = false;

		//オープンに追加するノードを格納する配列
		std::vector<AStarNode*>openPushBackNode;

		for (int y = -1; y < 2; y++)
		{
			int indexY = mainNode->indexY + y;

			//Y範囲外指定防止
			if (indexY <= -1 || indexY >= nodeYArrayNum)continue;

			for (int x = -1; x < 2; x++)
			{
				int indexX = mainNode->indexX + x;

				//X範囲外指定防止
				if (indexX <= -1 || indexX >= nodeXArrayNum)continue;

				//基準検索防止
				if (indexX == mainNode->indexX && indexY == mainNode->indexY)continue;

				AStarNode* checkNode = &nodes[indexY][indexX];



				//オブジェクトに重なってるかどうか
				if (checkNode->hitObjectNode)
				{
					checkNode->closeFlag = true;
					continue;
				}


				//calcNum = スタートからの距離 + ゴールまでの距離 + コスト
				int calcNum = CalcNodeDistance(startNodeIndexX, startNodeIndexY, indexX, indexY)
					+ CalcNodeDistance(indexX, indexY, endNodeIndexX, endNodeIndexY)
					+ checkNode->cost;

				//条件を満たしたら代入
				if (checkNode->openFlag)
				{
					if (calcNum < checkNode->calcNum)
					{
						checkNode->calcNum = calcNum;
						checkNode->previousNode = &nodes[mainNode->indexY][mainNode->indexX];
					}

					continue;
				}

				//条件を満たしたら、closeからopenに移動&代入
				if (checkNode->closeFlag)
				{
					if (calcNum < checkNode->calcNum)
					{
						//closeから削除
						closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
						checkNode->closeFlag = false;
						checkNode->closeIndex = INT_MAX;

						//openに追加
						openNodes.push_back(checkNode);
						checkNode->openFlag = true;

						checkNode->calcNum = calcNum;
						checkNode->previousNode = &nodes[mainNode->indexY][mainNode->indexX];
					}
					continue;
				}


				checkNode->calcNum = calcNum;
				checkNode->previousNode = &nodes[mainNode->indexY][mainNode->indexX];

				if (checkNode->indexX == endNodeIndexX && checkNode->indexY == endNodeIndexY)
				{
					checkEnd = true;
					endNode = checkNode;
				}


				openPushBackNode.push_back(checkNode);
				checkNode->openFlag = true;
			}
		}

		if (checkEnd)break;

		closeNodes.push_back(mainNode);
		mainNode->closeFlag = true;
		mainNode->closeIndex = closeNodes.size() - 1;

		//検索したやつを取り出す(ソートで最小が最後に来るようになっている)
		openNodes[openNodes.size() - 1]->openFlag = false;
		openNodes.erase(openNodes.begin() + openNodes.size() - 1);


		for (auto& n : openPushBackNode)
		{
			openNodes.push_back(n);
		}
		openPushBackNode.clear();
	}

	routeVector.clear();
	AStarNode* currentNode = endNode;
	while (1)
	{
		if (!currentNode)break;
		routeVector.push_back(currentNode->position);
		currentNode = currentNode->previousNode;
	}
	std::reverse(routeVector.begin(), routeVector.end());


	return true;
}


#pragma endregion




float LibMath::AngleConversion(int paterrn, float angle)
{
	if (paterrn == 0)
	{
		return (float)(angle * (M_PI / 180));
	}

	return (float)(angle * (180 / M_PI));

}

bool LibMath::Difference(const float num1, const float num2, const float difference)
{
	float sum = num1 - num2;
	float diff = abs(sum);//差を求める

		//差が指定した値以内だったらtrue
	if (difference >= diff)
		return true;

	return false;

}
bool LibMath::AngleDifference(const float angle1, const float angle2, const float difference)
{
	if (angle1 < 0
		|| angle1 >= 360
		|| angle2 < 0
		|| angle2 >= 360)return false;

	float sum = angle2 - angle1;
	float diff = abs(sum);//差を求める

	//通常の判定
	bool flag1 = difference >= diff;

	//小さいほうを入れる
	int smallNum = min(angle1, angle2);
	int bigNum = max(angle1, angle2);
	sum = bigNum - (smallNum + 360.0f);
	diff = abs(sum);
	bool flag2 = difference >= diff;

	if (flag1 || flag2)
		return true;

	return false;
}


float LibMath::GetFloatPI()
{
	return static_cast<float>(M_PI);
	//return 0;
}

double LibMath::GetDoublePI()
{
	return M_PI;
	//return 0;
}

#pragma region ベクトル


#pragma region vector2
float LibMath::CalcDistance2D(const Vector2& pos1, const Vector2& pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y)
	);
}
char LibMath::PointLeftRightCheck(const Vector2& vector, const Vector2& point)
{
	float num = Vector2Cross(vector, point);

	if (num > 0)return 1;
	if (num < 0)return -1;
	return 0;
}

float LibMath::TwoVector2Angle(const Vector2& v1, const Vector2& v2)
{

	float f = Vector2Dot(v1, Vector2Normalize(v2));
	f = std::acos(f);
	f = AngleConversion(1, f);

	Vector3 v = Vector3Cross({ v1.x,v1.y,0 }, { v2.x,v2.y,0 });
	if (v.z < 0)f = 360 - f;

	return f;
}

float LibMath::Vecto2ToAngle(const Vector2& v, const bool& v3)
{
	float f = 0.0f;
	if (v3)
		f = TwoVector2Angle({ 1,0 }, v);
	else
		f = TwoVector2Angle({ 1,0 }, { v.x,-v.y });
	return f;
}


Vector2 LibMath::AngleToVector2(const float& angle, const bool& v3)
{
	Quaternion q = GetRotateQuaternion({ 1,0,0 }, { 0,0,1 }, angle);

	if (v3)
		return { q.x,q.y };
	return { q.x,-q.y };
}


Vector2 LibMath::RotateVector2(const Vector2& v, const float& angle)
{
	Quaternion q = GetRotateQuaternion({ v.x,v.y,0 }, { 0,0,1 }, angle);
	return { q.x,q.y };
}
#pragma endregion

#pragma region Vector3

float LibMath::CalcDistance3D(Vector3 pos1, Vector3 pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z)
	);
}

Vector3 LibMath::OtherVector(const Vector3& vec1, const Vector3& vec2)
{
	Vector3 vec;
	vec = vec2 - vec1;
	return Vector3Normalize(vec);
}

Vector3 LibMath::RotateVector3(const Vector3& rotateV, const Vector3& vec, const float& angle)
{
	Quaternion q = GetRotateQuaternion(rotateV, vec, angle);
	return { q.x, q.y, q.z };
}

Vector3 LibMath::FloatDistanceMoveVector3
(
	const Vector3& pos,
	const Vector3& vector,
	const float distance
)
{
	return pos + vector * distance;
}


#pragma endregion


#pragma endregion


#pragma region 当たり判定
#pragma region 2D

bool LibMath::RectAndRectCollision
(
	const Vector2& pos1,
	const Vector2& size1,
	const Vector2& pos2,
	const Vector2& size2
)//四角判定
{
	if (size2.x + pos2.x > pos1.x &&
		pos2.x < size1.x + pos1.x &&
		pos2.y < size1.y + pos1.y &&
		size2.y + pos2.y > pos1.y)
		return true;
	return false;
}

bool LibMath::CircleCollision(Vector2 centerPosition1, float r1, Vector2 centerPosition2, float r2)//円判定
{
	if ((centerPosition1.x - centerPosition2.x) *
		(centerPosition1.x - centerPosition2.x) +
		(centerPosition1.y - centerPosition2.y) *
		(centerPosition1.y - centerPosition2.y) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

#pragma endregion

#pragma region 3D

bool LibMath::SphereAndSphereCollision(Vector3 centerPosition1, float r1, Vector3 centerPosition2, float r2)
{
	if ((centerPosition2.x - centerPosition1.x) *
		(centerPosition2.x - centerPosition1.x) +
		(centerPosition2.y - centerPosition1.y) *
		(centerPosition2.y - centerPosition1.y) +
		(centerPosition2.z - centerPosition1.z) *
		(centerPosition2.z - centerPosition1.z) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

bool LibMath::SphereAndPlaneCollision(Vector3 spherePos, float r, Vector3 normal, float planeDist, Vector3* hitPos)
{


	//原点から球の距離
	float sphereDist = Vector3Dot(spherePos, normal);

	float dict = sphereDist - planeDist;
	if (fabsf(dict) > r)return false;

	if (hitPos) *hitPos = normal * -dict + spherePos;
	return true;
}

bool LibMath::SphereAndTryangleCollision
(
	Vector3 spherePos,
	float r,
	Vector3 triPos1,
	Vector3 triPos2,
	Vector3 triPos3,
	Vector3 normal,
	Vector3* hitPos
)
{
	//最近接点
	Vector3 closest = { 0,0,0 };
	//内積
	float dot1, dot2;
	//所属エリア
	int area = 0;

#pragma region 領域チェック1

	if (area == 0)
	{
		//tryPos1、tryPos2のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot1 = Vector3Dot(triPos2 - triPos1, spherePos - triPos1);

		//tryPos1、tryPos3のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot2 = Vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 1;
			closest = triPos1;
		}
	}
#pragma endregion

#pragma region 領域チェック2

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos1 - triPos2, spherePos - triPos2);

		dot2 = Vector3Dot(triPos3 - triPos2, spherePos - triPos2);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 2;
			closest = triPos2;
		}
	}
#pragma endregion

#pragma region 領域チェック4

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos2 - triPos3, spherePos - triPos3);

		dot2 = Vector3Dot(triPos1 - triPos3, spherePos - triPos3);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 4;
			closest = triPos3;
		}
	}
#pragma endregion


#pragma region 領域チェック3

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos2 - triPos1, spherePos - triPos1);
		dot2 = Vector3Dot(triPos2 - triPos1, spherePos - triPos2);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
				Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1) <= 0)
			{
				area = 3;
				float num =
					Vector3Dot(triPos2 - triPos1, spherePos - triPos1) /
					(Vector3Dot(triPos2 - triPos1, spherePos - triPos1) - Vector3Dot(triPos2 - triPos1, spherePos - triPos2));
				closest.x = triPos1.x + num * (triPos2.x - triPos1.x);
				closest.y = triPos1.y + num * (triPos2.y - triPos1.y);
				closest.z = triPos1.z + num * (triPos2.z - triPos1.z);
			}


		}
	}
#pragma endregion

#pragma region 領域チェック5
	if (area == 0)
	{
		dot1 = Vector3Dot(triPos3 - triPos2, spherePos - triPos2);
		dot2 = Vector3Dot(triPos3 - triPos2, spherePos - triPos3);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos3 - triPos2, spherePos - triPos2) * Vector3Dot(triPos1 - triPos2, spherePos - triPos3) -
				Vector3Dot(triPos3 - triPos2, spherePos - triPos3) * Vector3Dot(triPos1 - triPos2, spherePos - triPos2) <= 0)
			{

				area = 5;

				float num =
					Vector3Dot(triPos3 - triPos2, spherePos - triPos2) /
					(Vector3Dot(triPos3 - triPos2, spherePos - triPos2) - Vector3Dot(triPos3 - triPos2, spherePos - triPos3));
				closest.x = triPos2.x + num * (triPos3.x - triPos2.x);
				closest.y = triPos2.y + num * (triPos3.y - triPos2.y);
				closest.z = triPos2.z + num * (triPos3.z - triPos2.z);

			}
		}
	}
#pragma endregion

#pragma region 領域チェック6
	if (area == 0)
	{
		dot1 = Vector3Dot(triPos1 - triPos3, spherePos - triPos3);
		dot2 = Vector3Dot(triPos1 - triPos3, spherePos - triPos1);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos1 - triPos3, spherePos - triPos3) * Vector3Dot(triPos2 - triPos3, spherePos - triPos1) -
				Vector3Dot(triPos1 - triPos3, spherePos - triPos1) * Vector3Dot(triPos2 - triPos3, spherePos - triPos3) <= 0)
			{
				area = 6;

				float num =
					Vector3Dot(triPos1 - triPos3, spherePos - triPos3) /
					(Vector3Dot(triPos1 - triPos3, spherePos - triPos3) - Vector3Dot(triPos1 - triPos3, spherePos - triPos1));
				closest.x = triPos3.x + num * (triPos1.x - triPos3.x);
				closest.y = triPos3.y + num * (triPos1.y - triPos3.y);
				closest.z = triPos3.z + num * (triPos1.z - triPos3.z);

			}


		}
	}
#pragma endregion


#pragma region 領域チェック7
	if (area == 0)
	{
		float va =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos3) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos3) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2);

		float vb =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos3) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos3);

		float vc =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		closest.x = triPos1.x + (triPos2.x - triPos1.x) * v + (triPos3.x - triPos1.x) * w;
		closest.y = triPos1.y + (triPos2.y - triPos1.y) * v + (triPos3.y - triPos1.y) * w;
		closest.z = triPos1.z + (triPos2.z - triPos1.z) * v + (triPos3.z - triPos1.z) * w;

		area = 7;
	}
#pragma endregion


	//最接近点 - 球の中心
	Vector3 v = closest - spherePos;
	float f = Vector3Dot(v, v);
	if (f > r * r) return false;

	if (hitPos)*hitPos = closest;
	return true;

}

bool LibMath::SphereAndBoxCollision
(
	const Vector3& spherePos,
	const float& r,
	const Vector3& centerPos,
	const Vector3& size,
	BoxHitDirection* direction
)
{
	float dir2 = 0.0f;

	Vector3 minPos = centerPos - size / 2;
	Vector3 maxPos = centerPos + size / 2;

	//x
	if (spherePos.x < minPos.x)
		dir2 += (minPos.x - spherePos.x) * (minPos.x - spherePos.x);
	if (spherePos.x > maxPos.x)
		dir2 += (spherePos.x - maxPos.x) * (spherePos.x - maxPos.x);

	//y
	if (spherePos.y < minPos.y)
		dir2 += (minPos.y - spherePos.y) * (minPos.y - spherePos.y);
	if (spherePos.y > maxPos.y)
		dir2 += (spherePos.y - maxPos.y) * (spherePos.y - maxPos.y);

	//z
	if (spherePos.z < minPos.z)
		dir2 += (minPos.z - spherePos.z) * (minPos.z - spherePos.z);
	if (spherePos.z > maxPos.z)
		dir2 += (spherePos.z - maxPos.z) * (spherePos.z - maxPos.z);

	bool flag = dir2 < r* r;

	//ボックスのどこに当たったかを返す
	if (direction)
	{

		if (!flag)
		{
			*direction = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return flag;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//ボックスへのベクトル
		Vector3 sphereToVector = centerPos - spherePos;

		if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
			abs(sphereToVector.x) > size.x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
				abs(sphereToVector.z) > size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
				abs(sphereToVector.z) > size.z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
		if (top == 2)
		{
			if (sphereToVector.y >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
		if (top == 3)
		{
			if (sphereToVector.z >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
	}

	return flag;
}

bool LibMath::BoxAndBoxCollision
(
	const Vector3& centerPos1,
	const Vector3& size1,
	const Vector3& centerPos2,
	const Vector3& size2,
	BoxHitDirection* direction1,
	BoxHitDirection* direction2
)
{
	Vector3 minPos1 = centerPos1 - size1 / 2;
	Vector3 maxPos1 = centerPos1 + size1 / 2;
	Vector3 minPos2 = centerPos2 - size2 / 2;
	Vector3 maxPos2 = centerPos2 + size2 / 2;
	bool isHit = false;

	//1か2のminかmaxがXYZ全部相手の範囲内だったら当たってる
	if (minPos1.x >= minPos2.x &&
		minPos1.x < maxPos2.x ||
		maxPos1.x >= minPos2.x &&
		maxPos1.x < maxPos2.x ||
		minPos2.x >= minPos1.x &&
		minPos2.x < maxPos1.x ||
		maxPos2.x >= minPos1.x &&
		maxPos2.x < maxPos1.x)
	{

		if (minPos1.y >= minPos2.y &&
			minPos1.y < maxPos2.y ||
			maxPos1.y >= minPos2.y &&
			maxPos1.y < maxPos2.y ||
			minPos2.y >= minPos1.y &&
			minPos2.y < maxPos1.y ||
			maxPos2.y >= minPos1.y &&
			maxPos2.y < maxPos1.y)
		{
			if (minPos1.z >= minPos2.z &&
				minPos1.z < maxPos2.z ||
				maxPos1.z >= minPos2.z &&
				maxPos1.z < maxPos2.z ||
				minPos2.z >= minPos1.z &&
				minPos2.z < maxPos1.z ||
				maxPos2.z >= minPos1.z &&
				maxPos2.z < maxPos1.z)
				isHit = true;

		}
	}

	if (direction1 || direction2)
	{

		if (!isHit)
		{
			if (direction1)
				*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			if (direction2)
				*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return isHit;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//対象へのベクトル
		Vector3 targetToVector = centerPos2 - centerPos1;

		if (abs(targetToVector.x) > abs(targetToVector.y) &&
			abs(targetToVector.x) > size2.x / 2)
		{
			top = 1;
			if (abs(targetToVector.z) > abs(targetToVector.x) &&
				abs(targetToVector.z) > size2.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(targetToVector.z) > abs(targetToVector.y) &&
				abs(targetToVector.z) > size2.z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (targetToVector.x >= 0)
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		if (top == 2)
		{
			if (targetToVector.y >= 0)
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				if (direction2)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				if (direction2)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		if (top == 3)
		{
			if (targetToVector.z >= 0)
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}
	}

	return isHit;
}

bool LibMath::LineSegmentAndBoardCollision
(
	Vector3 linePos1,
	Vector3 linePos2,
	Vector3 normal,
	Vector3 pointPos,
	std::vector<Vector3>vertexPoint,
	Vector3* crossPosition
)
{

	Vector3 v1;
	Vector3 v2;

	v1 = linePos1 - pointPos;
	v2 = linePos2 - pointPos;

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, normal) * Vector3Dot(v2, normal) > 0)
		return false;


	//ここにポリゴンの範囲内かどうかの処理を書く

	//線の端 - ポリゴンの角
	v1 = linePos1 - vertexPoint[0];
	v2 = linePos2 - vertexPoint[0];
	/*v1 = normalize(v1);
	v2 = normalize(v2);*/


	//ここがおかしい?

	////線の端から板ポリまでの距離を求める
	//float d = dot(normal,pointPos);
	//float kyori1;//平面から、linePos1までの距離
	//kyori1 = 
	//	abs(normal.x * linePos1.x + normal.y * linePos1.y +normal.z * linePos1.z + d) / 
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	//float kyori2;
	//kyori2 = 
	//	abs(normal.x * linePos2.x + normal.y * linePos2.y + normal.z * linePos2.z + d) /
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	Vector3 vec1 = linePos1 - pointPos;
	Vector3 vec2 = linePos2 - pointPos;

	float kyori1 = 0;
	if (normal.x != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.x);
	if (normal.y != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.y);
	if (normal.z != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.z);
	float kyori2 = 0;
	if (normal.x != 0)kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.x);
	if (normal.y != 0)	kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.y);
	if (normal.z != 0)	kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.z);

	/*float kyori1 = abs(dot(normal, linePos1)) / abs(normal.x);
	float kyori2 = abs(dot(normal, linePos2)) / abs(normal.x);*/

	//内分比
	float a;
	a = kyori1 / (kyori1 + kyori2);


	//ここおかしい

	//線とポリゴンが当たっている場所を調べる
	Vector3 crossVector;//ポリゴンの角から当たってる場所の座標へのベクトル
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = vertexPoint[0] + crossVector;
	//crossPos.y *= - 1;



	//三角形1個目の判定
	Vector3 normal1;
	Library::CalculationNormal(vertexPoint[0], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Vector3 normal2;
	Library::CalculationNormal(vertexPoint[1], vertexPoint[2], crossPos, normal2, normal2, normal2);
	Vector3 normal3;
	Library::CalculationNormal(vertexPoint[2], vertexPoint[0], crossPos, normal3, normal3, normal3);

	//板ポリと法線が同じか調べる
	bool equal1 = false;//板ポリと法線が同じかどうか

	//同じだったらtrue
	if (Difference(normal.x, normal1.x, 0.0001f) &&
		Difference(normal.y, normal1.y, 0.0001f) &&
		Difference(normal.z, normal1.z, 0.0001f) &&
		Difference(normal.x, normal2.x, 0.0001f) &&
		Difference(normal.y, normal2.y, 0.0001f) &&
		Difference(normal.z, normal2.z, 0.0001f) &&
		Difference(normal.x, normal3.x, 0.0001f) &&
		Difference(normal.y, normal3.y, 0.0001f) &&
		Difference(normal.z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}


	//三角形2個目の判定
	Library::CalculationNormal(vertexPoint[2], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Library::CalculationNormal(vertexPoint[1], vertexPoint[3], crossPos, normal2, normal2, normal2);
	Library::CalculationNormal(vertexPoint[3], vertexPoint[2], crossPos, normal3, normal3, normal3);

	//板ポリと法線が同じか調べる
	bool equal2 = false;//板ポリと法線が同じかどうか

	//同じ(誤差0.0001以内)だったらtrue
	if (Difference(normal.x, normal1.x, 0.0001f) &&
		Difference(normal.y, normal1.y, 0.0001f) &&
		Difference(normal.z, normal1.z, 0.0001f) &&
		Difference(normal.x, normal2.x, 0.0001f) &&
		Difference(normal.y, normal2.y, 0.0001f) &&
		Difference(normal.z, normal2.z, 0.0001f) &&
		Difference(normal.x, normal3.x, 0.0001f) &&
		Difference(normal.y, normal3.y, 0.0001f) &&
		Difference(normal.z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//どちらかが同じ(板ポリの中)だったらifの中に
	if (equal1 || equal2)
	{


		return true;
	}

	//衝突位置と中心が同じだったらヒット
	if (Difference(crossPos.x, pointPos.x, 0.01f) &&
		Difference(crossPos.y, pointPos.y, 0.01f) &&
		Difference(crossPos.z, pointPos.z, 0.01f))
	{
		return true;
	}

	//衝突位置を返す
	if (crossPosition) *crossPosition = { 0,0,0 };

	return false;

}


bool LibMath::RayAndPlaneCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 normal,
	float planeDistance,
	float* distance,
	Vector3* crossPos
)
{
	const float epsilon = 1.0e-5f;

	float d1 = Vector3Dot(normal, layDirection);

	if (d1 > -epsilon) return false;

	float d2 = Vector3Dot(normal, layStartPos);

	float dist = d2 - planeDistance;

	float t = dist / -d1;

	if (t < 0)return false;

	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

bool LibMath::RayAndTryangleCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 triPos1,
	Vector3 triPos2,
	Vector3 triPos3,
	Vector3 normal,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 planeNormal;
	float planeDistance;
	Vector3 interPlane;

	planeNormal = normal;
	planeDistance = Vector3Dot(normal, triPos1);

	if (!RayAndPlaneCollision(layStartPos, layDirection, planeNormal, planeDistance, distance, &interPlane))return false;



	//交差判定
	const float epsilon = 1.0e-5f;
	Vector3 m;

	Vector3 pt_p0 = triPos1 - interPlane;
	Vector3 p0_p1 = triPos2 - triPos1;
	m = Vector3Cross(pt_p0, p0_p1);
	if (Vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p1 = triPos2 - interPlane;
	Vector3 p1_p2 = triPos3 - triPos2;
	m = Vector3Cross(pt_p1, p1_p2);
	if (Vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p2 = triPos3 - interPlane;
	Vector3 p2_p0 = triPos1 - triPos3;
	m = Vector3Cross(pt_p2, p2_p0);
	if (Vector3Dot(m, normal) < -epsilon)return false;

	if (crossPos)*crossPos = interPlane;
	return true;

}

bool LibMath::RayAndSphereCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 spherePos,
	float r,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 m = layStartPos - spherePos;
	float b = Vector3Dot(m, layDirection);
	float c = Vector3Dot(m, m) - r * r;
	if (c > 0.0f && b > 0.0f)return false;

	float discr = b * b - c;
	if (discr < 0.0f)return false;


	float t = -b - sqrt(discr);
	if (t < 0)t = 0.0f;
	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

#pragma endregion

#pragma endregion

