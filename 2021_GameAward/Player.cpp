#include "Player.h"
#include"LibMath.h"

#include"ValueLoader.h"
#include"XInputManager.h"
#include"ObjectManager.h"

#include"PlayerBullet.h"
#include"TargetObject.h"
#include"Block.h"

#include<fstream>



//ファイルから読みとってstaticに入れられるか確かめる

ObjModel Player::modelData;
int Player::createCount;
const int Player::CREATE_NUMBER = 1;
std::vector<Vector3> Player::initialBonePos;


Player::Player()
{
	Initialize();
}

Player::~Player()
{
}

void Player::LoadResource()
{
	std::string mtl;

	modelData.LoadModel("Resources/Model/testSnake.obj", true, CREATE_NUMBER, 0);
	/*Library::loadOBJVertex("Resources/Model/testSnake.obj", true, true, &mtl, modelData);
	Library::loadOBJMaterial("Resources/Model/", mtl, CREATE_NUMBER, modelData);*/
	//initialBonePos = Library::getBonePosition(modelData);
	initialBonePos = modelData.GetBonePosition();
}



void Player::SetHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Player::Initialize()
{
	//ボーン数
	const int boneNum = static_cast<int>(initialBonePos.size());

	SetHeapNum();

#pragma region 読み取り
	std::unordered_map<std::string, int>iMap;
	std::unordered_map<std::string, float>fMap;
	std::unordered_map<std::string, Vector3>v3Map;
	ValueLoader* valieLoader = ValueLoader::getInstance();

	valieLoader->loadValue("Resources/Data/player.msd", iMap, fMap, v3Map);
#pragma endregion


	position = { 0.0f,0.0f,0.0f };
	velocity = { 1.0f,0.0f,0.0f };
	speed = fMap["speed"];

#pragma region パラメーター


	hp = iMap["hp"];
	isMuteki = false;
	mutekiTimer = 0;
	mutekiTime = 60 * 3;
#pragma endregion

	boneMovePos.resize(boneNum, 0.0f);
	bonePos.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;

#pragma region ひねり
	rotateFlag = false;
	twistAngles.resize(boneNum);
	tienTimer = 0;
	tienTime = iMap["tienTime"];
	rotateSpeed = fMap["rotateSpeed"];//1フレームに回転する角度
	pushRotateAngle = fMap["pushRotateAngle"];
#pragma endregion

#pragma region 移動
	velRot = 0.0f;
	previousRot = 0.0f;
	moveRotateAngle.resize(boneNum, 0.0f);
	boneVelocity.resize(boneNum, 0.0f);
#pragma endregion

#pragma region 弾の発射
	//shotTime = iMap["shotTimer"];
	shotTime = 60 * 1;
	shotTimer = 0;
#pragma endregion


#pragma region 判定
	collisionFlag.sphere = true;

	sphereData.resize(boneNum);
	for (int i = 0; i < boneNum; i++)
	{
		sphereData[i].position = bonePos[i];
		sphereData[i].r = 0.4f;
	}

#pragma endregion


}
void Player::Update()
{
	//ボーン数
	const int boneNum = static_cast<int>(initialBonePos.size());



#pragma region 移動_移動時の回転処理

#pragma region オオイシ移動

	//if (DirectInput::keyState(DIK_LEFT))
	//	velRot += 5.0f;
	//if (DirectInput::keyState(DIK_RIGHT))
	//	velRot -= 5.0f;



	/*float radVelRot = LibMath::angleConversion(0, velRot);
	velocity = { cos(radVelRot) ,0,sin(radVelRot) };
	float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	velocity /= length;
*/

#pragma endregion

#pragma region イチカワ移動
	previousRot = velRot;

	if (XInputManager::GetPadConnectedFlag(1))
	{
		if (XInputManager::LeftStickDown(40, 1) ||
			XInputManager::LeftStickUp(40, 1) ||
			XInputManager::LeftStickLeft(40, 1) ||
			XInputManager::LeftStickRight(40, 1))
		{
			velRot = XInputManager::LeftStickAngle(1);
		}
	}
	else
		velRot = DirectInput::ArrowKeyAngle();


	if (velRot != -1)
	{
		float radVelRot = LibMath::AngleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;
	}
	else
		velRot = previousRot;

#pragma endregion

#pragma region Unity移動
	//velRot = DirectInput::getMouseAngle();

	//float radVelRot = LibMath::angleConversion(0, velRot);
	//velocity = { cos(radVelRot) ,0,sin(radVelRot) };
	//float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	//velocity /= length;
#pragma endregion


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
		}
		else
		{
			//前のボーンへのベクトル
			Vector3 forwardVector = bonePos[i - 1] - bonePos[i];

			float length = sqrt
			(
				forwardVector.x * forwardVector.x +
				forwardVector.y * forwardVector.y +
				forwardVector.z * forwardVector.z
			);

			moveRotateAngle[i] = LibMath::AngleConversion(1, atan2(forwardVector.z, forwardVector.x));

			//moveRotateAngle[i] = moveRotateAngle[i] == 90.0f ? 0 : moveRotateAngle[i];


			float size = initialBonePos[i - 1].x - initialBonePos[i].x;
			//近づくバグ対策のif?
			if (length >= size)
			{
				boneMovePos[i] += forwardVector / length * speed;
				//Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
				modelData.SetBoneMoveVector(boneMovePos[i], i, heapNum);
				boneVelocity[i] = forwardVector / length;
			}
		}

		//座標を代入
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;
	}


	//場外防止
	//テストプレイ終了のリセットに使用



#pragma endregion

#pragma region ひねり処理

	if (!XInputManager::GetPadConnectedFlag(1))
	{
		if (DirectInput::KeyTrigger(DIK_SPACE))
			rotateFlag = true;
	}
	else
		if (XInputManager::ButtonTrigger(XInputManager::XINPUT_RB_BUTTON, 1))
			rotateFlag = true;



	if (rotateFlag)
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


	//角度セット
	for (int i = 0; i < boneNum; i++)
		//Library::setOBJBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, modelData, 0);
		modelData.SetBoneAngle({ twistAngles[i] ,-moveRotateAngle[i],0 }, i, heapNum);
#pragma region 弾を発射

	auto shotBullet = [&](const UINT& arrayNum)
	{
		Vector3 normalizeForwordVector = Vector3Normalize(bonePos[arrayNum - 1] - bonePos[arrayNum]);
		Quaternion q = GetRotateQuaternion(normalizeForwordVector, { 0,1,0 }, -90 + twistAngles[arrayNum]);

		ObjectManager::GetInstance()->AddObject(std::make_shared<PlayerBullet>(bonePos[arrayNum], Vector3(q.x, 0, q.z)));
	};

	if (shotTimer >= shotTime)
		shotTimer = 0;
	if (twistAngles[0] == 0)
		shotTimer++;

	if (shotTimer >= shotTime)
	{
		for (int i = 1; i < boneNum - 1; i++)
		{
			shotBullet(i);
		}
	}

	for (int i = 1; i < boneNum - 1; i++)
	{
		if (twistAngles[i] >= 180 &&
			twistAngles[i] <= 180 + rotateSpeed)
			shotBullet(i);
	}


#pragma endregion

#pragma region 判定
	for (int i = 0; i < boneNum; i++)
		sphereData[i].position = bonePos[i];

#pragma endregion

#pragma region パラメーター処理

#pragma region ライフ

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



}

void Player::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}

void Player::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	//結構狭い隙間でも通れちゃうから修正するか、隙間ができないようにする
	if (typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(TargetObject))
	{
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

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;
			}

			break;
		case BOX_HIT_DIRECTION_LEFT:
			if (boneVelocity[arrayNum].x <= 0)
				boneMovePos[arrayNum].x += -1.0f * speed.x;
			else
				boneMovePos[arrayNum].x += -boneVelocity[arrayNum].x * speed.x;


			if (arrayNum != 0)
			{

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.x = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].z += previousBoneVector.z * speed.z;
			}

			break;
		case BOX_HIT_DIRECTION_FRONT:
			if (boneVelocity[arrayNum].z <= 0)
				boneMovePos[arrayNum].z += -1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
			{

				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.z = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].x += previousBoneVector.x * speed.x;
			}

			break;
		case BOX_HIT_DIRECTION_BACK:
			if (boneVelocity[arrayNum].z >= 0)
				boneMovePos[arrayNum].z += 1.0f * speed.z;
			else
				boneMovePos[arrayNum].z += -boneVelocity[arrayNum].z * speed.z;


			if (arrayNum != 0)
			{
				Vector3 previousBoneVector = bonePos[arrayNum - 1] - bonePos[arrayNum];
				previousBoneVector.z = 0;
				previousBoneVector = Vector3Normalize(previousBoneVector);
				boneMovePos[arrayNum].x += previousBoneVector.x * speed.x;
			}
			break;
		}

		bonePos[arrayNum] = initialBonePos[arrayNum] + boneMovePos[arrayNum] + position;

	}

	if (isMuteki)return;
	//if (typeid(*object) == typeid(Enemy))
	{
		isMuteki = true;

	}
}

void* Player::GetPtr()
{
	return this;
}
