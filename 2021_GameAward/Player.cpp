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
	tienTimer.resize(initialBonePos.size() - 1, 0);

	//twistAngles = 0.0f;

	//遅延中かどうか
	rotateFlag = false;


	//遅延フレーム
	tienFream = 5;
    //スピード
	rotateSpeed = 5;
}

void Player::update()
{
	UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region 回転処理


	//回転
	if (DirectInput::keyState(DIK_SPACE))
		twistAngles[0] += rotateSpeed;
	

	//前と違ったら動かす
	for (int i = 0; i < boneNum - 1; i++) 
	{
		if (twistAngles[i] != twistAngles[i + 1])
			tienTimer[i]++;
	}
	
	

	for (int i = 1; i < boneNum; i++) 
	{
		//規定時間を超えたら入る
		if (tienTimer[i - 1] >= tienFream)
		{
			//前と違ったら回転
			if (twistAngles[i] != twistAngles[i - 1])
				twistAngles[i] += rotateSpeed;
			else //同じだったら回転終了
			{
				twistAngles[i] = twistAngles[i - 1];
				tienTimer[i - 1] = 0;
			}
		}
	}

	/*if (twistAngles[0] == twistAngles[twistAngles.size() - 1]) 
	{
		tienTimer = 0;
		rotateFlag = false;
	}*/
		

		
		
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

