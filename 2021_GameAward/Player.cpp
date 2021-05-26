#include "Player.h"
#include"LibMath.h"

#include"ValueLoader.h"
#include"XInputManager.h"
#include"ObjectManager.h"
#include"SceneManager.h"

#include"PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"

#include"EnemyBullet.h"

#include"Play.h"
#include"StageSelect.h"
#include"GameOver.h"

#include<fstream>


//ファイルから読みとってstaticに入れられるか確かめる

ObjModel Player::modelData;
const int Player::CREATE_NUMBER = 2;
std::vector<Vector3> Player::initialBonePos;
std::vector<Vector3> Player::initialBonePosMulScale;
int Player::boneNum;
HeapIndexManager Player::playerModelHeapIndexManager(CREATE_NUMBER);


Sprite2D Player::hpAnimationSprite;
Texture Player::hpAnimationTexture;
Sprite2D Player::hpCrossSpr;
Texture Player::hpCrossTex;
Sprite2D Player::hpNumSpr;
Texture Player::hpNumTex;

Player::Player()
{
	Initialize();
}


Player::Player(const Vector3& pos)
{
	modelMoveVector = pos;
	Initialize();

	std::reverse(boneMovePos.begin(), boneMovePos.end());
	//セット
	for (int i = 0; i < boneNum; i++) 
	{
		modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
	}
	position = bonePos[bonePos.size() / 2];
	modelData.SetPosition(modelMoveVector, heapNum);
}


Player::~Player()
{
	playerModelHeapIndexManager.DrawEndCallFunction(heapNum);
}

void Player::LoadResource()
{
	//モデル
	modelData.LoadModel("Resources/Model/Player/snake.obj", true, CREATE_NUMBER, 0);

	initialBonePos = modelData.GetBonePosition();
	boneNum = static_cast<int>(initialBonePos.size());

	
	//スプライト
	hpAnimationSprite.CreateSprite();
	hpAnimationTexture.LoadSpriteTexture("Resources/Texture/hpAnimation.png");
	hpAnimationSprite.SetPosition(Vector2(0, 0));

	hpCrossSpr.CreateSprite();
	hpCrossSpr.SetPosition(Vector2(70, 0));
	hpCrossSpr.SetScale(Vector2(0.4, 0.4));
	hpCrossTex.LoadSpriteTexture("Resources/Texture/numCross.png");

	hpNumSpr.CreateSprite();
	hpNumTex.LoadSpriteTexture("Resources/Texture/TimeNumber.png");
	hpNumSpr.SetPosition(Vector2(140, 10));

	////スプライト
	//targetLockSprite.CreateSprite({10,10});
	//targetLockTexture.LoadSpriteTexture("Resources/Texture/lock.png");
	//targetLockSprite.SetBillboardFlag(true,true,true);
	//targetLockSprite.SetAngle({90,0,0});
}


void Player::Initialize()
{
	heapNum = playerModelHeapIndexManager.GetHeapIndex();

#pragma region 読み取り
	std::unordered_map<std::string, int>iMap;
	std::unordered_map<std::string, float>fMap;
	std::unordered_map<std::string, Vector3>v3Map;
	ValueLoader* valieLoader = ValueLoader::getInstance();

	valieLoader->loadValue("Resources/Data/player.msd", iMap, fMap, v3Map);
#pragma endregion


	position = { 0.0f,0.0f,0.0f };
	velocity = { 1.0f,0.0f,0.0f };
	//initSpeed = fMap["speed"];
	initSpeed = 0.25f;
	speed = initSpeed;

	//selectStage = false;
#pragma region パラメーター


	//hp = iMap["hp"];
	hp = 5;
	isMuteki = false;
	mutekiTimer = 0;
	mutekiTime = 60 * 3;
#pragma endregion

	scale = { 1,1,1 };

	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))
	{
		speedMag = 25.0f;
		speed = initSpeed * speedMag;
		scale = { 20,20,20 };
	}

	modelData.SetScale(scale, heapNum);

	initialBonePosMulScale = initialBonePos;

	boneMovePos.resize(boneNum, 0.0f);
	bonePos.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
	{
		//スケールを掛ける
		initialBonePosMulScale[i] *= scale;

		//セット
		modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
	}


	

#pragma region ひねり
	rotateFlag = false;
	twistAngles.resize(boneNum);
	tienTimer = 0;
	//tienTime = iMap["tienTime"];
	tienTime = 4;
	//rotateSpeed = fMap["rotateSpeed"];//1フレームに回転する角度
	rotateSpeed = 3.0f;
	//pushRotateAngle = fMap["pushRotateAngle"];
	pushRotateAngle = 360.0f;
#pragma endregion

#pragma region 移動
	velRot = 0.0f;
	previousRot = velRot;
	moveRotateAngle.resize(boneNum, 0.0f);
	boneVelocity.resize(boneNum, 0.0f);

	targetRotatePlayer = false;
	lockTargetNum = -1;
#pragma endregion

#pragma region 弾の発射
	//shotTime = iMap["shotTimer"];
	shotTime = 60 * 1;
	shotTimer = 0;
#pragma endregion

#pragma region 判定
	collisionFlag.sphere = true;

	sphereData.resize(boneNum);
	sphereData[0].r = 0.6f * scale.x;
	for (int i = 1; i < boneNum; i++)
	{
		sphereData[i].position = bonePos[i];
			
			/*if(i <= boneNum - 2)
				sphereData[i].r = 0.8f * scale.x;
			else*/
				sphereData[i].r = 1.5f * scale.x;
	}

#pragma endregion


	modelData.SetPosition(modelMoveVector, heapNum);
	//角度セット
	for (int i = 0; i < boneNum; i++) 
		modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);

	//スプライト関係
	hpAnimationTimer.SetMaxTime(HP_ANIMATION_ONE_FREAM_TIME * 8);
	hpAnimationTimer.SetStopFlag(false);

}

void Player::PlayMove()
{

	lockTarget = false;

	//近いオブジェクトの距離を求める
	float nearTergetDis = 9999999.0f;
	float targetDistance = 0.0f;
	Vector3 nearTergetPosition = 0;

	auto targetPosSize = targetPos.size();
	int nearTargetNum = -1;
	for (int i = 0; i < targetPosSize; i++)
	{
		targetDistance = LibMath::CalcDistance3D
		(
			bonePos[0],
			targetPos[i]
		);
		if (nearTergetDis > targetDistance)
		{
			nearTergetPosition = targetPos[i];
			nearTergetDis = targetDistance;
			nearTargetNum = i;
		}
	}
	float maxDistance = 40.0f;
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))maxDistance *= 17.0f;

	//周り回転フラグ
	bool distanceFlag = nearTergetDis <= maxDistance;
	
	bool padTargetObjectRot =
		XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonState(XInputManager::XInputButton::XINPUT_LB_BUTTON, 1)
		&& distanceFlag;

	bool keyTargetObjectRot = DirectInput::KeyState(DIK_X)
		&& distanceFlag;

	if (keyTargetObjectRot
		|| padTargetObjectRot)
	{
		lockTarget = true;
		lockTargetNum = nearTargetNum;

		//祠に近づく処理
		float minDistance = 10.0f;
		if (typeid(*currentScene) == typeid(StageSelect))minDistance *= 30.0f;

		if (nearTergetDis >= minDistance
			&& !targetRotatePlayer)
		{
			targetRotatePlayer = false;

			Vector3 playerToNeraTarget = nearTergetPosition - bonePos[0];
			float playerToNeraTargetAngle = LibMath::Vecto2ToAngle
			(
				{ playerToNeraTarget.x,playerToNeraTarget.z },
				true
			);
			velRot = playerToNeraTargetAngle;

		}
		//回る処理
		else
		{
			if (!targetRotatePlayer)
			{
				targetRotatePlayer = true;
				//ここで、ターゲットからプレイヤーへのベクトルの角度を求め、velRotに入れる
				Vector3 targetToPlayerToNera = bonePos[0] - nearTergetPosition;
				float targetToPlayerToNeraAngle = LibMath::Vecto2ToAngle
				(
					{ targetToPlayerToNera.x,targetToPlayerToNera.z },
					true
				);
				velRot = targetToPlayerToNeraAngle - 90.0f;
			}
			if (typeid(*currentScene) == typeid(StageSelect))
			{
				/*if (selectStage)
				{
					float subRotNum = 2.5f;
					velRot -= subRotNum;
					speed = initSpeed * stageSelectSpeedMag * subRotNum;
				}
				else*/
				velRot -= initSpeed * 5;
			}
			else
				velRot -= initSpeed * 6;
		}

	}
	//移動
	else
	{
		targetRotatePlayer = false;

		if (XInputManager::GetPadConnectedFlag(1))
		{
			if (XInputManager::LeftStickDown(40, 1) ||
				XInputManager::LeftStickUp(40, 1) ||
				XInputManager::LeftStickLeft(40, 1) ||
				XInputManager::LeftStickRight(40, 1))
				velRot = XInputManager::LeftStickAngle(1);
		}
		else
			velRot = DirectInput::ArrowKeyAngle();
	}


}

void Player::StageSelectMove()
{
	float velRotAngle = initSpeed * 5;
	if (StageSelect::GetStageSelectState() == StageSelect::STAGE_SELECT_STATE_SELECT_END)
	{
	 	const float mulAngleNum = 6.0f;
		velRotAngle *= mulAngleNum;
		speed = initSpeed * speedMag * mulAngleNum;
	}

	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360;
}

void Player::GameOverMove()
{
	float velRotAngle = initSpeed * 5;
	if (GameOver::GetSelectEndFlag())
	{
		const float mulAngleNum = 6.0f;
		velRotAngle *= mulAngleNum;
		speed = initSpeed *  mulAngleNum;
	}

	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360;
}

void Player::PauseMove()
{
	speedMag = 2.0f;
	speed = initSpeed * speedMag;
	scale = 0.3f;

	float velRotAngle = initSpeed * 14.0f;
	
	velRot -= velRotAngle;
	if (velRot >= 360)
		velRot -= 360;
	if (velRot <= 0)
		velRot += 360; 
	
	modelData.SetScale(scale, heapNum);
}

void Player::Update()
{
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	//準備前は動かないように
	if (typeid(*currentScene) == typeid(Play)
		&& !Play::GetIsPauseFlag())
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET)return;
		else if (Play::GetPlaySceneState() == Play::PLAY_SCENE_START_PREVIOUS)
		{
			//**ここにゲーム開始前のプレイヤー停止処理を書く**//

			return;
		}
	}


#pragma region パラメーター処理

#pragma region ライフ
	if (hp <= 0) 
	{
		isDead = true;
	}
#pragma endregion

#pragma region 無敵処理
	if (isMuteki)
	{
		mutekiTimer++;
		if (mutekiTimer >= mutekiTime)
		{
			mutekiTimer = 0;
			isMuteki = false;
		}
	}
#pragma endregion


#pragma endregion

	if(isDead)
	{
		//点滅用
		isMuteki = true;
		return;
	}

	lockTargetNum = -1;


#pragma region 移動_移動時の回転処理

	previousRot = velRot;

	if (typeid(*currentScene) == typeid(Play))
	{
		if (Play::GetIsPauseFlag())
		{
			PauseMove();
		}
		else 
		{
			PlayMove();
		}
	}
	if (typeid(*currentScene) == typeid(StageSelect)) 
	{
		StageSelectMove();
	}
	if (typeid(*currentScene) == typeid(GameOver)) 
	{
		GameOverMove();
	}
	

	if (velRot != -1)
	{
		float radVelRot = LibMath::AngleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;
	}
	else//入力されてなかったら前フレームの角度を代入
		velRot = previousRot;


	for (int i = 0; i < boneNum; i++)
	{
		if (i == 0)
		{
			//回転角度
			moveRotateAngle[i] = velRot;
			boneMovePos[i] += velocity * speed;

			//初期位置からどのくらい動いているかをセット
			//Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
			modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
			boneVelocity[i] = velocity;

			//座標を代入
			bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
		}
		else
		{
			////前のボーンへのベクトル
			Vector3 forwardVector = bonePos[i - 1] - bonePos[i];
			////移動によるY軸基準の回転角度を代入
			//moveRotateAngle[i] = LibMath::AngleConversion(1, atan2(forwardVector.z, forwardVector.x));
			moveRotateAngle[i] = LibMath::Vecto2ToAngle({ forwardVector.x,forwardVector.z }, true);


			float bonePosDistance = LibMath::CalcDistance3D(bonePos[i - 1], bonePos[i]);
			float defaultBoneDistance = LibMath::CalcDistance3D(initialBonePosMulScale[i - 1], initialBonePosMulScale[i]);

			//デフォルト距離以上の時に、defaultBoneDistanceを超えないように移動bonePos[i] = initialBonePosMulScale[i] 
			if (bonePosDistance >= defaultBoneDistance)
			{
				//差を求める
				float disDifference = bonePosDistance - defaultBoneDistance;

				boneVelocity[i] = Vector3Normalize(bonePos[i - 1] - bonePos[i]);

				//距離の差だけ移動させる
				//これにより近づきすぎを防げる
				boneMovePos[i] = LibMath::FloatDistanceMoveVector3(boneMovePos[i], boneVelocity[i], disDifference);

				//セット
				modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
				bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
			}

		}



	}
#pragma endregion

	auto SetAngleAndPos = [&]()
	{
		//角度セット
		for (int i = 0; i < boneNum; i++)
		{
			modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);
		}

		position = bonePos[bonePos.size() / 2];
		modelData.SetPosition(modelMoveVector, heapNum);
	};

	if(Play::GetIsPauseFlag())
	{
		SetAngleAndPos();
		return;
	}

#pragma region ひねり処理

	if (!XInputManager::GetPadConnectedFlag(1))
	{
		if (DirectInput::KeyTrigger(DIK_SPACE))
			rotateFlag = true;
	}
	else
		if (XInputManager::ButtonTrigger(XInputManager::XINPUT_RB_BUTTON, 1))
			rotateFlag = true;


	if (rotateFlag && typeid(*currentScene) == typeid(Play))
	{
		for (int i = 0; i < boneNum; i++)
		{
			//設定回転量を超えてなかったら入る
			if (twistAngles[i] < pushRotateAngle &&
				pushRotateAngle >= 0 ||
				twistAngles[i] > pushRotateAngle &&
				pushRotateAngle <= -1)
			{
				//遅延終了時間になったら回転
				if (tienTimer >= tienTime * i)
					twistAngles[i] += rotateSpeed;
			}
			//超えたら設定量を代入し、回転を止める
			else
				twistAngles[i] = pushRotateAngle;
		}

		//全部回転し終わったら入る
		if (twistAngles[boneNum - 1] == pushRotateAngle)
		{
			//リセット
			rotateFlag = false;
			tienTimer = 0;
			for (auto& a : twistAngles)
				a = 0;
		}

		//回転してたらカウント
		tienTimer++;
	}

#pragma endregion

	SetAngleAndPos();

#pragma region 弾を発射

	auto shotBullet = [&](const UINT& arrayNum)
	{
		if (Play::GetPlaySceneState() == Play::PLAY_SCENE_GAMECLEAR)return;

		Vector3 forwordVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
		Vector3 normalizeForwordVector = Vector3Normalize(forwordVector);
		Quaternion q = GetRotateQuaternion(normalizeForwordVector, { 0,1,0 }, -90 + twistAngles[arrayNum]);

		ObjectManager::GetInstance()->AddObject(std::make_shared<PlayerBullet>
			(
				//Vector3(bonePos[arrayNum].x, bonePos[arrayNum].y - 0.5f, bonePos[arrayNum].z),
				bonePos[arrayNum] + Vector3(q.x * 2.5 ,-0.2f, q.z * 2.5),
				Vector3(q.x, 0, q.z)
				));

		Library::PlaySoundEveryLoad("Resources/Sound/SE/PlayerSE/SneakBulletSe.wav");
	};

	//旧ショット(自動)
	/*if (shotTimer >= shotTime)
		shotTimer = 0;
	if (twistAngles[0] == 0)
		shotTimer++;

	if (shotTimer >= shotTime)
	{
		for (int i = 1; i < boneNum - 1; i++)
		{
			shotBullet(i);
		}
	}*/

	//新ショット(手動)
	bool keyShot = DirectInput::KeyTrigger(DIK_Z)
		&& !rotateFlag
		&& typeid(*currentScene) == typeid(Play);

	bool padShot = XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XInputButton::XINPUT_X_BUTTON, 1) 
		&& !rotateFlag
		&& typeid(*currentScene) == typeid(Play);

	if (keyShot || padShot)
	{
		for (int i = 3; i < boneNum - 3; i++)
		{
			if (i % 2 == 0)continue;
			shotBullet(i);
		}
	}


	for (int i = 3; i < boneNum - 3; i++)
	{
		if (i % 2 == 0)continue;

		if (twistAngles[i] >= 180 &&
			twistAngles[i] < 180 + rotateSpeed)
			shotBullet(i);
	}


#pragma endregion

#pragma region 判定
	for (int i = 0; i < boneNum; i++)
		sphereData[i].position = bonePos[i];

#pragma endregion

	//スプライト座標
	/*if (targetLock) 
	{
		Vector3 targetSprPos = targetPos[targetNum];
		targetLockSprite.SetPosition(targetSprPos);
	}*/


}

void Player::Draw()
{
	if (isMuteki == false || (isMuteki == true && mutekiTimer % 2 == 0))
	modelData.Draw(heapNum);

	/*if (targetLock)
		targetLockSprite.Draw(&targetLockTexture);*/

	

}

void Player::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (isDead)
		return;

	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))return;

	//結構狭い隙間でも通れちゃうから修正するか、隙間ができないようにする
	if (typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(TargetObject))
	{

		auto MoveBone = [&]()
		{
			float bonePosDistance = LibMath::CalcDistance3D(bonePos[arrayNum - 1], bonePos[arrayNum]);
			float defaultBoneDistance = LibMath::CalcDistance3D(initialBonePosMulScale[arrayNum - 1], initialBonePosMulScale[arrayNum]);

			//デフォルト距離以上の時に、defaultBoneDistanceを超えないように移動
			if (bonePosDistance >= defaultBoneDistance)
			{
				//差を求める
				float disDifference = bonePosDistance - defaultBoneDistance;

				boneVelocity[arrayNum] = Vector3Normalize(bonePos[arrayNum - 1] - bonePos[arrayNum]);

				//距離の差だけ移動させる
				//これにより近づきすぎを防げる
				boneMovePos[arrayNum] = LibMath::FloatDistanceMoveVector3
				(
					boneMovePos[arrayNum],
					boneVelocity[arrayNum],
					disDifference
				);

				//セット
				modelData.SetBoneMoveVector(boneMovePos[arrayNum], arrayNum, heapNum);
				bonePos[arrayNum] = initialBonePosMulScale[arrayNum] + boneMovePos[arrayNum] + modelMoveVector;
			}
		};

		switch (sphereData[arrayNum].boxHitDistance)
		{
		case BOX_HIT_DIRECTION_RIGHT:
			//内側に入り込むの防止するif
			//velocityを固定のfloatにすればめり込まないが、カメラが動きまくるのでこうしてる
			if (boneVelocity[arrayNum].x >= 0)
				boneMovePos[arrayNum].x += 1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			//右にぶつかってるかつ引っかかりをとるには、
			//xの進行をなくし、z方向に動かすしかない
			//先頭のvelocityを基準にすると、途中で逆行ったときに
			//引っかかりがひどくなってしまうから前のボーンのを参照
			//velocityじゃなくて相手へのベクトルを使用(velocityだと逆に進んでしまうときがあった)
			//何か不具合あってどうしても対策ができなさそうだったら、
			//上のif分を、if(typeid(*object) == typeid(Block) && arrayNum == 0)に変更してください
			if (arrayNum != 0)
			{

				/*Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;*/

				MoveBone();
			}

			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (boneVelocity[arrayNum].x <= 0)
				boneMovePos[arrayNum].x += -1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			if (arrayNum != 0)
				MoveBone();

			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (boneVelocity[arrayNum].z <= 0)
				boneMovePos[arrayNum].z += -1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
				MoveBone();

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (boneVelocity[arrayNum].z >= 0)
				boneMovePos[arrayNum].z += 1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
				MoveBone();
			break;
		}

		bonePos[arrayNum] = initialBonePosMulScale[arrayNum] + boneMovePos[arrayNum] + modelMoveVector;

	}


	if (typeid(*object) == typeid(EnemyBullet))
		DamageFromEnemy();

	//if (isMuteki)return;
	////if (typeid(*object) == typeid(Enemy))
	//{
	//	isMuteki = true;

	//}
}

void Player::UpdateHpAnimation()
{
	//HPアニメーション
	if (hpAnimationTimer.GetMultipleTimeFlag(HP_ANIMATION_ONE_FREAM_TIME))
		hpAnimationNum++;
	if (hpAnimationTimer.GetSameAsMaximumFlag())
		hpAnimationNum = 0;

}

void Player::DrawHp()
{	
	
	//蛇のアニメーション
	const Vector2 hpAnimationTextureSize = hpAnimationTexture.GetTextureSize();
	hpAnimationSprite.SelectDrawAreaDraw
	(
		Vector2(hpAnimationTextureSize.x / 8 * hpAnimationNum, 0),
		Vector2(hpAnimationTextureSize.x / 8 * (hpAnimationNum + 1), hpAnimationTextureSize.y),
		&hpAnimationTexture
	);

	//かける
	hpCrossSpr.Draw(&hpCrossTex);

	//数字
	const float numSize = hpNumTex.GetTextureSize().y;
	hpNumSpr.SelectDrawAreaDraw(Vector2(numSize * hp, 0), Vector2(numSize * (hp + 1), numSize), &hpNumTex);
}


void Player::SetModelMoveVector(const Vector3& vector) 
{
	modelMoveVector = vector; 
	modelData.SetPosition(modelMoveVector,heapNum);
	for (int i = 0; i < boneNum; i++) 
	{
		bonePos[i] = initialBonePosMulScale[i] + boneMovePos[i] + modelMoveVector;
		sphereData[i].position = bonePos[i];
	}
}


void Player::DamageFromEnemy()
{
	if (isMuteki)
		return;
	hp--;
	isMuteki = true;
}


