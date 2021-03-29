#include "Player.h"
#include"LibMath.h"

#include"ValueLoader.h"
#include"XInputManager.h"

#include<fstream>

//ファイルから読みとってstaticに入れられるか確かめる

ModelData Player::modelData;
int Player::createCount;
const int Player::CREATE_NUMBER = 1;
std::vector<Vector3> Player::initialBonePos;

Player::Player()
{
}

Player::~Player()
{
}

void Player::loadModel()
{
	std::string mtl;

	modelData.key = "player";
	Library::loadOBJVertex("Resources/Model/testSnake.obj", true, true, &mtl, modelData);
	Library::loadOBJMaterial("Resources/Model/", mtl, CREATE_NUMBER, modelData);
	initialBonePos = Library::getBonePosition(modelData);
}



void Player::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Player::initialize()
{
	//ボーン数
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

	setHeapNum();

#pragma region 読み取り
	std::unordered_map<std::string, int>iMap;
	std::unordered_map<std::string, float>fMap;
	std::unordered_map<std::string, Vector3>v3Map;
	ValueLoader* valieLoader = ValueLoader::getInstance();

	valieLoader->loadValue("Resources/Data/player.msd", iMap, fMap, v3Map);
#pragma endregion


	position = v3Map["position"];
	velocity = {1.0f,0.0f,0.0f};
	speed = fMap["speed"];

	collisionFlag.sphere = true;
	//sphereData.resize(initialBonePos.size());

	hp = iMap["hp"];

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
#pragma endregion

}
void Player::update()
{
	//ボーン数
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

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

#pragma region Unity移動
	previousRot = velRot;

	
	if(XInputManager::getPadConnectedFlag(1))
		velRot = XInputManager::leftStickAngle(1);
	else
		velRot = DirectInput::arrowKeyAngle();


	if (velRot != -1)
	{

		float radVelRot = LibMath::angleConversion(0, velRot);
		velocity = { cos(radVelRot) ,0,sin(radVelRot) };
		float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= length;
	}
	else
		velRot = previousRot;

#pragma endregion




	for (UINT i = 0; i < boneNum; i++)
	{
		if (i == 0)
		{
			//回転角度
			moveRotateAngle[i] = velRot;
			boneMovePos[i] += velocity * speed;

			//初期位置からどのくらい動いているかをセット
			Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
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

			moveRotateAngle[i] = LibMath::angleConversion(1, atan2(forwardVector.z, forwardVector.x));
			moveRotateAngle[i] = moveRotateAngle[i] == 90.0f ? 0 : moveRotateAngle[i];


			float size = initialBonePos[i - 1].x - initialBonePos[i].x;
			//近づくバグ対策のif?
			if (length >= size)
			{
				boneMovePos[i] += forwardVector / length * speed;
				Library::setOBJBoneMoveVector(boneMovePos[i], i, modelData, heapNum);
			}
		}

		//座標を代入
		bonePos[i] = initialBonePos[i] + boneMovePos[i] + position;
	}

#pragma endregion

#pragma region ひねり処理

	if (DirectInput::keyTrigger(DIK_SPACE))
		rotateFlag = true;

	if (rotateFlag) 
	{
		for (UINT i = 0; i < boneNum; i++)
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
	for(UINT i = 0; i < boneNum;i++)
		Library::setOBJBoneAngle({twistAngles[i] ,-moveRotateAngle[i],0 }, i, modelData, 0);
	
	Library::setPosition(position, modelData, heapNum);
}

void Player::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);
}

void Player::hit
(
	const Object *const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

void* Player::getPtr()
{
	return this;
}
