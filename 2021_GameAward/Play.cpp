#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#include"LibMath.h"
#include"StageSelect.h"
#pragma region オブジェクト
#include"Block.h"


#pragma endregion

sprite Play::arrowSprite;
texture Play::arrowTexture;

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

void Play::loadResources()
{
	//リソースの読み込み
	Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");
}


void Play::initialize()
{
	player = std::make_shared<Player>();
	
	//敵追加
	std::shared_ptr<Enemy> enemy1 = std::make_shared<MoveEnemy>();
	enemy1->setPPlayer(player.get());
	enemy1->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	enemies.push_back(enemy1);
	std::shared_ptr<Enemy> enemy2 = std::make_shared<ShotEnemy>();
	enemy2->setPPlayer(player.get());
	enemy2->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	enemies.push_back(enemy2);
	std::shared_ptr<Enemy> enemy3 = std::make_shared<FleeEnemy>();
	enemy3->setPPlayer(player.get());
	enemy3->setPosition(Vector3(Library::getRandomNumber(50), 0, Library::getRandomNumber(50)));
	enemies.push_back(enemy3);


	ObjectManager::getInstance()->addObject(player);

	for (int i = 0; i < enemies.size(); i++) {
		ObjectManager::getInstance()->addObject(enemies[i]);
	}

	playSceneState = PlaySceneState::PLAY_SCENE_SETTARGET;

#pragma region カメラ
	addCameraPosition = { 0,50,-2 };
	cameraPosition = addCameraPosition;
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region ブロックセット
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::getInstance()->addObject(std::make_shared<Block>(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region 祠セット
	targetObjects.resize(targetNumber);
	for (auto& t : targetObjects)
	{

		//座標セット
		//ここ範囲乱数作って変える
		Vector3 targetPos =
		{
			Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
			0,
			Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
		};

		t = std::make_shared<TargetObject>(targetPos);
		ObjectManager::getInstance()->addObject(t);
	}
#pragma endregion


#pragma region 矢印
	arrowPosition = 0.0f;
	arrowAngle = 0.0f;
	drawArrow = false;
#pragma endregion


}

void Play::update()
{

	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();
#pragma region 祠処理

//	//祠セット
//	//座標の値が変わってないのにワープしてたのは、
//	//生成を繰り返してヒープの番号が被った時に、元から置かれてた祠のモデルの座標が消えるやつの座標で上書きされたから
	if (playSceneState == PlaySceneState::PLAY_SCENE_SETTARGET)
	{
		int createMissCount = 0;
		for (auto& t : targetObjects)
		{
			//nullptrは飛ばす
			if (!t)continue;
			//設置完了済は飛ばす
			if (t->getSetEnd())continue;

			//ブロックとかプレイヤーと重なったたらカウント
			if (t->getCreateHitObject())
			{
				createMissCount++;
				continue;
			}

			//距離測定
			bool distanceCheck = true;//距離の基準を満たしたかどうか
			float dis;
			for (auto& t2 : targetObjects)
			{
				//ターゲット同士
				if (!t2 || t == t2)continue;
				dis = LibMath::calcDistance3D
				(
					t->getPosition(),
					t2->getPosition()
				);

				//最低ライン超えられなかったらカウント&即抜け
				if (dis <= targetDistance)
				{
					createMissCount++;
					distanceCheck = false;
					break;
				}


			}


			//プレイヤー
			dis = LibMath::calcDistance3D
			(
				t->getPosition(),
				player->getPosition()
			);
			if (dis <= playerDistance)
			{
				createMissCount++;
				distanceCheck = false;
				break;
			}

			//距離の基準を満たしたら設置完了
			if (distanceCheck)
				t->trueSetEnd();

		}


#pragma region 祠座標再セット
		for (auto& t : targetObjects)
		{
			//設置完了済は飛ばす
			if (t->getSetEnd())continue;

			//座標セット
			//ここ範囲乱数作って変える
			Vector3 targetPos =
			{
				Library::getRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::getRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->setPosition(targetPos);

		}

#pragma endregion

		//カウントされなかったらセット終了
		if (createMissCount == 0)
		{
			//仮にこうしてる
			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
			Block::setGameStart(true);
		}
	}
	else//祠生き残り確認(クリア判定用)
	{
		int deadCount = 0;
		for (auto& t : targetObjects)
		{
			//やられてオブジェクトマネージャーから削除されてたらカウント
			if (t->getEraseManager())
				deadCount++;
		}

		//終了処理
		/*if(deadCount == targetObjects.size())
		{ 
		}*/
	}

#pragma endregion

#pragma region カメラ移動
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += addCameraPosition;
	cameraTarget = pHeapPos;
	Library::setCamera(cameraPosition, cameraTarget, { 0,1,0 });

#pragma endregion


#pragma region 祠を示す矢印
	//一番近い祠を求める
	float minDistance = 9999999.0f;
	Vector3 playerHeadPos = player->getHeadPosition();
	Vector3 nearTargetPos = 0;
	for(auto& t : targetObjects)
	{
		if(!t->getEraseManager())
		{
			float dis = LibMath::calcDistance3D
			(
				playerHeadPos,
				t->getPosition()
			);
			if (dis < minDistance) 
			{
				minDistance = dis;
				nearTargetPos = t->getPosition();
			}
		}
	}
	//祠あったら計算
	if (minDistance < 9999999.0f) 
	{
		Vector2 arrowTexSize = Library::getTextureSize(arrowTexture);

		//プレイヤーから一番近いターゲットのベクトルを正規化したもの
		Vector3 playerToTargetVector = nearTargetPos - playerHeadPos;
		Vector3 playerToTargetNVector = vector3Normalize(playerToTargetVector);
		
		//クォータニオンで回す?
		//座標
		arrowPosition = { 1280.0f / 2.0f - arrowTexSize.x /2, 720.0f / 2.0f - arrowTexSize.y / 2};
		//祠の方に移動させる
		arrowPosition.x += playerToTargetNVector.x * (1280 / 2);
		arrowPosition.y -= playerToTargetNVector.z * (720 / 2);

		//画面内に収まるようにする
		if (arrowPosition.x >= 1280 - arrowTexSize.x)
			arrowPosition.x = 1280 - arrowTexSize.x;
		if (arrowPosition.x <= 0)
			arrowPosition.x = 0;
		if (arrowPosition.y >= 720 - arrowTexSize.y)
			arrowPosition.y = 720 - arrowTexSize.y;
		if (arrowPosition.y <= 0)
			arrowPosition.y = 0;

		//角度
		arrowAngle = LibMath::vecto2ToAngle({ playerToTargetNVector.x,playerToTargetNVector.z }, false);

		//描画するかどうか
		drawArrow = false;
		if (abs(playerToTargetVector.x) >= 50.0f ||
			abs(playerToTargetVector.z) >= 30.0f)
			drawArrow = true;
	}
#pragma endregion
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();
	

	Library::setSpriteAngle(arrowAngle, arrowSprite);
	if(drawArrow)
	Library::drawSprite(arrowPosition, arrowSprite, arrowTexture);
}

void Play::end()
{
	ObjectManager::getInstance()->allEraseObject();
}

Scene* Play::getNextScene()
{
	return new StageSelect();
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