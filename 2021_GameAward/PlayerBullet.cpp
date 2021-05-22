#include "PlayerBullet.h"
#include "TargetObject.h"
#include "Block.h"
#include "Player.h"
#include "Enemy.h"
#include "MoveEnemy.h"
#include "ShotEnemy.h"
#include "FleeEnemy.h"
#include "SimEnemy.h"
#include "DefenceEnemy.h"
#include "HealEnemy.h"
#include"EnemyBullet.h"

#include"LibMath.h"

#include"Scene.h"
#include"SceneManager.h"
#include"StageSelect.h"

ObjModel PlayerBullet::modelData;
const int PlayerBullet::CREATE_NUMBER = 300;
HeapIndexManager PlayerBullet::heapIndexManager(CREATE_NUMBER);

bool PlayerBullet::IsEnemy(const Object* p)
{

	if (typeid(*p) == typeid(MoveEnemy)
		|| typeid(*p) == typeid(ShotEnemy)
		|| typeid(*p) == typeid(FleeEnemy)
		|| typeid(*p) == typeid(SimEnemy))
	{
		return true;
	}

	return false;


}

PlayerBullet::PlayerBullet(const Vector3& pos, const Vector3& vel)
{
	Initialize();
	position = pos;
	velocity = vel;

	float velAngle = LibMath::Vecto2ToAngle({ vel.x,vel.z }, true);
	modelData.SetAngle({ 0,-velAngle ,0 }, heapNum);
	modelData.SetScale(0.7f, heapNum);
}

PlayerBullet::~PlayerBullet()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}


void PlayerBullet::LoadResource()
{
	modelData.LoadModel("Resources/Model/PlayerBullet/bullet.obj", true, CREATE_NUMBER, 0);
}

void PlayerBullet::Initialize()
{
	position = 0;
	velocity = 0;
	speed = 0.6f;
	heapNum = heapIndexManager.GetHeapIndex();

	deadTimer = 0;
	deadTime = 60 * 2;

	collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.5f;

	scale = 1.0f;

	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(StageSelect))
	{
		scale = 15.0f;
		speed *= 25.0f;
		sphereData[0].r *= scale.x;
	}
	if (heapNum != -1)
		modelData.SetScale(scale, heapNum);
}

void PlayerBullet::Update()
{
	if (heapNum == -1)
	{
		eraseManager = true;
		return;
	}

	deadTimer++;
	if (deadTimer >= deadTime)
		eraseManager = true;

	position += velocity * speed;
	sphereData[0].position = position;
	modelData.SetPosition(position, heapNum);
}

void PlayerBullet::Draw()
{
	if (heapNum == -1)return;

	modelData.Draw(heapNum);
}

void PlayerBullet::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(TargetObject) ||
		typeid(*object) == typeid(Block)||
		typeid(*object) == typeid(EnemyBullet))
		eraseManager = true;


	if (typeid(*object) == typeid(Player) &&
		deadTimer >= 60 * 0.05f)
		eraseManager = true;


	if (IsEnemy(object))
		eraseManager = true;
}

