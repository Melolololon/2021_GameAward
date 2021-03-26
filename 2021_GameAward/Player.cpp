#include "Player.h"

ModelData Player::modelData;
int Player::createCount;
const int Player::CREATE_NUM = 1;
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

void Player::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUM ? 0 : createCount;
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
	twistAngles.resize(initialBonePos.size(), 0);
	tienTimer = 0;

	//twistAngles = 0.0f;

	//遅延中かどうか
	rotateFlag = false;


	//遅延フレーム
	tienFream = 4;
    //スピード
	rotateSpeed = 2;
}

void Player::update()
{
	UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region 回転処理


	//回転
	if (DirectInput::keyState(DIK_SPACE) &&
		!rotateFlag)
		rotateFlag = true;
	
	//回転してたら入る
	if (rotateFlag) 
	{
		//時間計測
		tienTimer++;

		//1回転したらストップ
		if (twistAngles[0] < 360)
			twistAngles[0] += rotateSpeed;
	}

	for(int i = 1; i < boneNum; i++)
	{
		//遅延終了時間になったら入る
		if (tienTimer >= tienFream * i) 
		{
			//前と違ったら回転
			if (twistAngles[i - 1] != twistAngles[i])
				twistAngles[i] += rotateSpeed;
			else//同じだったら代入して誤差をなくし、回転終了
				twistAngles[i] = twistAngles[i] >= twistAngles[i - 1] ? twistAngles[i - 1] : twistAngles[i];
		}
	}

	//全部回転し終わったら入る
	if(twistAngles[0] == twistAngles[twistAngles.size() - 1] &&
		rotateFlag)
	{

		rotateFlag = false;
		tienTimer = 0;

		for (auto& a : twistAngles)
			a = 0;
	}


		
		for (int i = 0; i < boneNum; i++)
		Library::setOBJBoneAngle({ static_cast<float>(twistAngles[i]), 0,0 }, i, modelData, 0);
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

