#include "EnemyBullet.h"
#include"TargetObject.h"
#include"Block.h"
#include"Player.h"
#include"PlayerBullet.h"

#include"LibMath.h"

ObjModel EnemyBullet::modelData;
int EnemyBullet::createCount;
const int EnemyBullet::CREATE_NUMBER = 100;

EnemyBullet::EnemyBullet(const Vector3& pos, const Vector3& vel)
{
	Initialize();
	position = pos;
	velocity = vel;
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void EnemyBullet::LoadResource()
{
	modelData.LoadModel
	(
		"Resources/Model/EnemyBullet/bullet.obj",
		true,
		CREATE_NUMBER,
		0
	);

}

void EnemyBullet::Initialize()
{
	position = 0;
	velocity = 0;
	speed = 0.4;
	setHeapNum();

	deadTimer = 0;
	deadTime = 60 * 3;

	collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.3f;

	modelData.SetScale(0.7f, heapNum);
	
}

void EnemyBullet::Update()
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

	//ƒvƒŒƒCƒ„[‚Ì•û‚ÉŒü‚­ˆ—
	float angle = LibMath::Vecto2ToAngle(Vector2(velocity.x, velocity.z), true);
	modelData.SetAngle(Vector3(0, -angle, 0), heapNum);
}

void EnemyBullet::Draw()
{
	modelData.Draw(heapNum);
}

void EnemyBullet::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(TargetObject) ||
		typeid(*object) == typeid(Block) ||
		typeid(*object) == typeid(Player) ||
		typeid(*object) == typeid(PlayerBullet))
		eraseManager = true;

}
