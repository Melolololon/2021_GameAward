#include "TargetObject.h"
#include"SceneManager.h"
#include"Player.h"
#include"Block.h"
#include"PlayerBullet.h"
ObjModel TargetObject::modelData;
const int TargetObject::CREATE_NUMBER = 100;
bool TargetObject::hitSegment;

std::vector<Sprite3D> TargetObject::hpGaugeSprite(CREATE_NUMBER);
std::vector<Sprite3D> TargetObject::hpGaugeFreamSprite(CREATE_NUMBER);
Texture TargetObject::lifeGaugeTexture;
Texture TargetObject::hpGaugeFreamTexture;

int TargetObject::maxHp;
float TargetObject::hpGaugeOneNumSizeX;

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

	Vector2 hpSize = { 10,2 };
	for (int i = 0; i < CREATE_NUMBER; i++)
	{
		hpGaugeSprite[i].CreateSprite({ hpSize.x,hpSize.y });
		hpGaugeFreamSprite[i].CreateSprite({ hpSize.x + 0.3f,hpSize.y +0.3f});
	}
	lifeGaugeTexture.LoadSpriteTexture("Resources/Texture/lifeGauge.png");
	hpGaugeFreamTexture.LoadSpriteTexture("Resources/Texture/lifeFream.png");

	maxHp = 30;
	hpGaugeOneNumSizeX = hpSize.x / 30;
}


void TargetObject::Initialize()
{

	heapNum = heapIndexManager.GetHeapIndex();

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].position = position;
	boxData[0].size = { 2,5,2 };


	modelData.SetPosition(position, heapNum);

	hp = maxHp;
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
	hpGaugeFreamSprite[heapNum].SetPosition(lifePos + Vector3(0, 0.2, 0));
	hpGaugeFreamSprite[heapNum].SetBillboardFlag(true, true, true);

	float hpGaugeOneNumScaleX = 1.0f / (float)maxHp;
	hpGaugeSprite[heapNum].SetScale(Vector2(hpGaugeOneNumScaleX * hp, 1.0f));
	lifePos.x -= hpGaugeOneNumSizeX * (maxHp - hp) / 2;
	hpGaugeSprite[heapNum].SetPosition(lifePos);
	hpGaugeSprite[heapNum].SetBillboardFlag(true, true, true);
}

void TargetObject::Draw()
{
	
	modelData.Draw(heapNum);
	
	hpGaugeSprite[heapNum].Draw(&lifeGaugeTexture);
	hpGaugeFreamSprite[heapNum].Draw(&hpGaugeFreamTexture);
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


