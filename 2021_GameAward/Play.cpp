#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#include"LibMath.h"

#pragma region オブジェクト
#include"Block.h"


#pragma endregion


float Play::targetDistance;
float Play::playerDistance;
int Play::targetNumber;
Vector3 Play::leftUpPosition;
Vector3 Play::rightDownPosition;
std::vector<Vector3> Play::blockPositions;
std::vector<Vector3> Play::blockScales;

Play::Play()
{
}

Play::~Play(){}

void Play::initialize()
{
	player = new Player();
	enemy = new ShotEnemy();
	enemy->setPPlayer(player);
	enemy->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	ObjectManager::getInstance()->addObject(player);
	ObjectManager::getInstance()->addObject(enemy);

	playSceneState = PlaySceneState::PLAY_SCENE_SETTARGET;

#pragma region カメラ
	cameraPosition = { 0,50,-2 };
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region ブロックセット
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::getInstance()->addObject(new Block(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region 祠セット
	//targetObjects.resize(targetNumber);
	//for (auto& t : targetObjects)
	//{

	//	//座標セット
	//	//ここ範囲乱数作って変える
	//	Vector3 targetPos =
	//	{
	//		Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
	//		0,
	//		Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
	//	};

	//	t = new TargetObject(targetPos);
	//	ObjectManager::getInstance()->addObject(t);
	//}
#pragma endregion



	//仮
	Block::setGameStart(true);

}

void Play::update()
{
	ObjectManager::getInstance()->update();

#pragma region 祠処理

//	//祠セット
//	//座標の値が変わってないのにワープしてたのは、
//	//生成を繰り返してヒープの番号が被った時に、元から置かれてた祠のモデルの座標が消えるやつの座標で上書きされたから
//	if (playSceneState == PlaySceneState::PLAY_SCENE_SETTARGET)
//	{
//		int createMissCount = 0;
//		for (auto& t : targetObjects)
//		{
//			//nullptrは飛ばす
//			if (!t)continue;
//			//設置完了済は飛ばす
//			if (t->getSetEnd())continue;
//
//			//ブロックとかプレイヤーと重なったたらカウント
//			if (t->getCreateHitObject())
//			{
//				createMissCount++;
//				continue;
//			}
//
//			//距離測定
//			bool distanceCheck = true;//距離の基準を満たしたかどうか
//			float dis;
//			for (auto& t2 : targetObjects)
//			{
//				//ターゲット同士
//				if (!t2 || t == t2)continue;
//				dis = LibMath::calcDistance3D
//				(
//					t->getPosition(),
//					t2->getPosition()
//				);
//
//				//最低ライン超えられなかったらカウント&即抜け
//				if (dis <= targetDistance)
//				{
//					createMissCount++;
//					distanceCheck = false;
//					break;
//				}
//
//
//			}
//
//
//			//プレイヤー
//			dis = LibMath::calcDistance3D
//			(
//				t->getPosition(),
//				player->getPosition()
//			);
//			if (dis <= playerDistance)
//			{
//				createMissCount++;
//				distanceCheck = false;
//				break;
//			}
//
//			//距離の基準を満たしたら設置完了
//			if (distanceCheck)
//				t->trueSetEnd();
//
//		}
//
//
//#pragma region 祠座標再セット
//		for (auto& t : targetObjects)
//		{
//			//設置完了済は飛ばす
//			if (t->getSetEnd())continue;
//
//			//座標セット
//			//ここ範囲乱数作って変える
//			Vector3 targetPos =
//			{
//				Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
//				0,
//				Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
//			};
//
//			t->setPosition(targetPos);
//
//		}
//
//#pragma endregion
//
//		//カウントされなかったらセット終了
//		if (createMissCount == 0)
//		{
//			//仮にこうしてる
//			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
//			Block::setGameStart(true);
//		}
//	}
//	else//祠生き残り確認(クリア判定用)
//	{
//		int deadCount = 0;
//		for (auto& t : targetObjects)
//		{
//			if (!t)
//			{
//				deadCount++;
//				continue;
//			}
//			//やられたら消す
//			if (t->getEraseManager())
//				t = nullptr;
//		}
//
//		//if(deadCount == targetObjects.size())
//			
//	}

#pragma endregion

#pragma region カメラ移動
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += {0, 50, -2};
	cameraTarget = pHeapPos;
	Library::setCamera(cameraPosition, cameraTarget, { 0,1,0 });

#pragma endregion

	ObjectManager::getInstance()->isDeadCheck();
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();



}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}

void Play::setStageData
(
	std::vector<Vector3>blockPos,
	std::vector<Vector3>blockScale,
	const float& targetDis,
	const float& playerDis,
	const int& targetNum,
	const Vector3& leftUpPos,
	const Vector3& rightDownPos
)
{
	blockPositions = blockPos;
	blockScales = blockScale;
	targetDistance = targetDis;
	playerDistance = playerDis;
	targetNumber = targetNum;
	leftUpPosition = leftUpPos;
	rightDownPosition = rightDownPos;

}