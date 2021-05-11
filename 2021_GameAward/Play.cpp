#include "Play.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include "SimEnemy.h"
#include "DefenceEnemy.h"
#include "HealEnemy.h"

#include"ObjectManager.h"
#include"XInputManager.h"

#include"LibMath.h"
#include"StageSelect.h"
#pragma region オブジェクト
#include"Block.h"


#pragma endregion

Sprite2D Play::arrowSprite;
Texture Play::arrowTexture;

float Play::targetDistance;
float Play::playerDistance;
int Play::targetNumber;
Vector3 Play::leftUpPosition;
Vector3 Play::rightDownPosition;
std::vector<Vector3> Play::blockPositions;
std::vector<Vector3> Play::blockScales;

Play::PlaySceneState Play::playSceneState;
Play::Play()
{
}


Play::~Play() {}

void Play::LoadResources()
{
	//リソースの読み込み
	/*Library::createSprite(&arrowSprite);
	arrowTexture = Library::loadTexture(L"Resources/Texture/arrow.png");*/
	arrowSprite.CreateSprite();
	arrowTexture.LoadSpriteTexture("Resources/Texture/arrow.png");
}


void Play::Initialize()
{
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);
	

	//敵追加
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		std::shared_ptr<Enemy> enemy;
		Play::EnemyType enemyType = (Play::EnemyType)1;// (Play::EnemyType)Library::GetRandomNumber(1);
		if (enemyType == Play::EnemyType::ET_MoveEnemy){
			enemy = std::make_shared<MoveEnemy>();
		}
		else if (enemyType == EnemyType::ET_ShotEnemy) {
			enemy = std::make_shared<ShotEnemy>();
		}
		else if (enemyType == EnemyType::ET_FleeEnemy) {
			enemy = std::make_shared<FleeEnemy>();
		}
		/*
		else if (enemyType == EnemyType::ET_SimEnemy) {
			enemy = std::make_shared<SimEnemy>();
		}
		else if (enemyType == EnemyType::ET_DefenceEnemy) {
			enemy = std::make_shared<DefenceEnemy>();
		}
		else if (enemyType == EnemyType::ET_HealEnemy) {
			enemy = std::make_shared<HealEnemy>();
		}*/

		enemy->setPPlayer(player.get());
		//enemy->setPosition(Vector3(Library::GetRandomNumber(100) - 50, 0, Library::GetRandomNumber(100) - 50));
		enemy->setPosition(Vector3(50,0,20));
		ObjectManager::GetInstance()->AddObject(enemy);
		enemies.push_back(enemy);
	}

	playSceneState = PlaySceneState::PLAY_SCENE_SET_TARGET;

#pragma region カメラ
	addCameraPosition = { 0,50,-2 };
	cameraPosition = addCameraPosition;
	cameraTarget = { 0,0,0 };
#pragma endregion

#pragma region ブロックセット
	int blockNum = static_cast<int>(blockPositions.size());

	for (int i = 0; i < blockNum; i++)
		ObjectManager::GetInstance()->AddObject(std::make_shared<Block>(blockPositions[i], blockScales[i]));
#pragma endregion


#pragma region 祠セット
	targetObjects.resize(targetNumber);
	for (auto& t : targetObjects)
	{

		//座標セット
		//ここ範囲乱数作って変える
		Vector3 targetPos =
		{
			Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
			0,
			Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
		};

		t = std::make_shared<TargetObject>(targetPos);
		ObjectManager::GetInstance()->AddObject(t);
	}
#pragma endregion


#pragma region 矢印
	arrowPosition = 0.0f;
	arrowAngle = 0.0f;
	drawArrow = false;
#pragma endregion


}

void Play::Update()
{

	ObjectManager::GetInstance()->Update();
#pragma region 祠処理

	//	//祠セット
	//	//座標の値が変わってないのにワープしてたのは、
	//	//生成を繰り返してヒープの番号が被った時に、元から置かれてた祠のモデルの座標が消えるやつの座標で上書きされたから
	if (playSceneState == PlaySceneState::PLAY_SCENE_SET_TARGET)
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
				dis = LibMath::CalcDistance3D
				(
					t->GetPosition(),
					t2->GetPosition()
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
			dis = LibMath::CalcDistance3D
			(
				t->GetPosition(),
				player->GetPosition()
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
				Library::GetRandomNumberRangeSelectFloat(leftUpPosition.x,rightDownPosition.x),
				0,
				Library::GetRandomNumberRangeSelectFloat(rightDownPosition.z,leftUpPosition.z)
			};

			t->setPosition(targetPos);

		}

#pragma endregion

		//カウントされなかったらセット終了
		if (createMissCount == 0) 
		{
			//仮にこうしてる
			playSceneState = PlaySceneState::PLAY_SCENE_PLAY;
		}
	}
	else//祠生き残り確認(クリア判定用)
	{
		int deadCount = 0;
		for (auto& t : targetObjects)
		{
			//やられてオブジェクトマネージャーから削除されてたらカウント
			if (t->GetEraseManager())
				deadCount++;
		}

		//ターゲットの座標をプレイヤーに渡す
		auto targetObjectsSize = targetObjects.size();
		std::vector<Vector3>targetObjectPos;
		targetObjectPos.reserve(30);
		for (int i = 0; i < targetObjectsSize; i++) 
		{
			if (!targetObjects[i]->GetEraseManager())
				targetObjectPos.push_back(targetObjects[i]->GetPosition());
		}
		player->SetTargetPosition(targetObjectPos);

		//終了処理
		/*if(deadCount == targetObjects.size())
		{
		}*/
	}

#pragma endregion

#pragma region カメラ移動
	Vector3 pHeapPos = player->GetHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += addCameraPosition;
	cameraTarget = pHeapPos;
	Library::SetCamera(cameraPosition, cameraTarget, { 0,0,1 });

#pragma endregion


#pragma region 祠を示す矢印
	//一番近い祠を求める
	float minDistance = 9999999.0f;
	Vector3 playerHeadPos = player->GetHeadPosition();
	Vector3 nearTargetPos = 0;
	for (auto& t : targetObjects)
	{
		if (!t->GetEraseManager())
		{
			float dis = LibMath::CalcDistance3D
			(
				playerHeadPos,
				t->GetPosition()
			);
			if (dis < minDistance)
			{
				minDistance = dis;
				nearTargetPos = t->GetPosition();
			}
		}
	}
	//祠あったら計算
	if (minDistance < 9999999.0f)
	{
		//Vector2 arrowTexSize = Library::GetTextureSize(arrowTexture);
		Vector2 arrowTexSize = arrowTexture.GetTextureSize();

		//プレイヤーから一番近いターゲットのベクトルを正規化したもの
		Vector3 playerToTargetVector = nearTargetPos - playerHeadPos;
		Vector3 playerToTargetNVector = Vector3Normalize(playerToTargetVector);

		//クォータニオンで回す?
		//座標
		arrowPosition = { 1280.0f / 2.0f - arrowTexSize.x / 2, 720.0f / 2.0f - arrowTexSize.y / 2 };
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
		arrowAngle = LibMath::Vecto2ToAngle({ playerToTargetNVector.x,playerToTargetNVector.z }, false);

		//描画するかどうか
		drawArrow = false;
		if (abs(playerToTargetVector.x) >= 50.0f ||
			abs(playerToTargetVector.z) >= 30.0f)
			drawArrow = true;
	}
#pragma endregion
}

void Play::Draw()
{
	ObjectManager::GetInstance()->Draw();


	//Library::setSpriteAngle(arrowAngle, arrowSprite);
	arrowSprite.SetAngle(arrowAngle);
	arrowSprite.SetPosition(arrowPosition);
	if (drawArrow)
		//Library::drawSprite(arrowPosition, arrowSprite, arrowTexture);
		arrowSprite.Draw(&arrowTexture);
}

void Play::Finitialize()
{
	ObjectManager::GetInstance()->AllEraseObject();
}

Scene* Play::GetNextScene()
{
	return new StageSelect();
}


void Play::SetStageData
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