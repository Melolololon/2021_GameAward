#include "Player.h"

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
	Library::loadOBJMaterial("Resources/Model/", mtl, 1, modelData);
	initialBonePos = Library::getBonePosition(modelData);
}


void Player::loadParam()
{
	std::ifstream playerParam;
	//playerParam.open("");



	//playerParam.close();
}


void Player::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Player::initialize()
{
	setHeapNum();

	position = 0.0f;
	velocity = 1.0f;
	speed = 0.5f;

	collisionFlag.sphere = true;
	//sphereData.resize(initialBonePos.size());

	boneMovePos.resize(initialBonePos.size(), 0);
	
	//回転のregionの数値を子の関数でセットする予定
	loadParam();

#pragma region 回転
	rotateFlag = false;
	twistAngles.resize(initialBonePos.size(), 0);
	tienTimer = 0;
	tienTime = 4;
	rotateSpeed = 7;//1フレームに回転する角度
	pushRotateAngle = 360;
#pragma endregion
}

void Player::update()
{
	//ボーン数
	const UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region 移動処理

#pragma endregion

#pragma region 回転処理

	if (DirectInput::keyTrigger(DIK_SPACE))
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

			//角度セット
			Library::setOBJBoneAngle({ static_cast<float>(twistAngles[i]), 0,0 }, i, modelData, 0);
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
