#include "TargetObject.h"
#include"SceneManager.h"
#include"Player.h"
#include"Block.h"
#include"PlayerBullet.h"
ObjModel TargetObject::modelData;
const int TargetObject::CREATE_NUMBER = 100;
bool TargetObject::hitSegment;

std::vector<Sprite3D> TargetObject::lifeGaugeSprite(CREATE_NUMBER);
std::vector<Sprite3D> TargetObject::lifeGaugeFreamSprite(CREATE_NUMBER);
Texture TargetObject::lifeGaugeTexture;
Texture TargetObject::lifeGaugeFreamTexture;

HeapIndexManager TargetObject::heapIndexManager(CREATE_NUMBER);

TargetObject::TargetObject(const Vector3& pos)
{
	position = pos;
	Initialize();
}

TargetObject::~TargetObject()
{
	heapIndexManager.DrawEndCallFunction(heapNum);
}


void TargetObject::LoadResource()
{
	modelData.LoadModel("Resources/Model/TargetObject/Shrine.obj", true, CREATE_NUMBER, 0);

	for (int i = 0; i < CREATE_NUMBER; i++)
	{
		Vector2 lifeSize = { 10,2 };
		lifeGaugeSprite[i].CreateSprite({ lifeSize.x,lifeSize.y });
		lifeGaugeFreamSprite[i].CreateSprite({ lifeSize.x + 0.1f,lifeSize.y +0.1f});
	}
	lifeGaugeTexture.LoadSpriteTexture("Resources/Texture/lifeGauge.png");
	lifeGaugeFreamTexture.LoadSpriteTexture("Resources/Texture/lifeFream.png");
}


void TargetObject::Initialize()
{

	heapNum = heapIndexManager.GetHeapIndex();

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].position = position;
	boxData[0].size = { 2,5,2 };


	modelData.SetPosition(position, heapNum);

	hp = 30;
	setEnd = false;
}

void TargetObject::Update()
{
	hitSegment = false;
	createHitObject = false;


	boxData[0].position = position;
	modelData.SetPosition(position, heapNum);

	if (hp <= 0)
		eraseManager = true;


	Vector3 lifePos = position + Vector3(0, 2.5f, -7.0f);
	lifeGaugeSprite[heapNum].SetPosition(lifePos);
	lifeGaugeFreamSprite[heapNum].SetPosition(lifePos + Vector3(0, 0.1, 0));
	lifeGaugeSprite[heapNum].SetBillboardFlag(true, true, true);
	lifeGaugeFreamSprite[heapNum].SetBillboardFlag(true, true, true);
}

void TargetObject::Draw()
{
	
	modelData.Draw(heapNum);
	lifeGaugeSprite[heapNum].Draw(&lifeGaugeTexture);
	lifeGaugeFreamSprite[heapNum].Draw(&lifeGaugeFreamTexture);
}


void TargetObject::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	if (typeid(*object) == typeid(Player))
		createHitObject = true;
	if (typeid(*object) == typeid(Block))
		createHitObject = true;


	if (typeid(*object) == typeid(PlayerBullet))
		hp--;

}


void TargetObject::trueEraseManager() { eraseManager = true; }

void TargetObject::trueSetEnd() { setEnd = true; }

bool TargetObject::getSetEnd() { return setEnd; }
bool TargetObject::getCreateHitObject() { return createHitObject; }

void TargetObject::setPosition(const Vector3& pos)
{
	position = pos;
	modelData.SetPosition(position, heapNum);
	boxData[0].position = position;
}


