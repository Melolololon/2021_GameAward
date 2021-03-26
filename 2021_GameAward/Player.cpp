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

	//’x‰„’†‚©‚Ç‚¤‚©
	rotateFlag = false;


	//’x‰„ƒtƒŒ[ƒ€
	tienFream = 5;
    //ƒXƒs[ƒh
	rotateSpeed = 5;
}

void Player::update()
{
	UINT boneNum = static_cast<UINT>(initialBonePos.size());

#pragma region ‰ñ“]ˆ—


	//‰ñ“]
	if (DirectInput::keyState(DIK_SPACE))
		twistAngles[0] += rotateSpeed;
	

	//‘O‚Æˆá‚Á‚½‚ç“®‚©‚·
	for (int i = 0; i < boneNum - 1; i++) 
	{
		if (twistAngles[i] != twistAngles[i + 1])
			tienTimer[i]++;
	}
	
	

	for (int i = 1; i < boneNum; i++) 
	{
		//‹K’èŠÔ‚ğ’´‚¦‚½‚ç“ü‚é
		if (tienTimer[i - 1] >= tienFream)
		{
			//‘O‚Æˆá‚Á‚½‚ç‰ñ“]
			if (twistAngles[i] != twistAngles[i - 1])
				twistAngles[i] += rotateSpeed;
			else //“¯‚¶‚¾‚Á‚½‚ç‰ñ“]I—¹
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

