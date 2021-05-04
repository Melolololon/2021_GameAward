#include "Block.h"

#include"Play.h"

PrimitiveModel Block::modelData;
int Block::createCount;
const int Block::CREATE_NUMBER = 300;



Block::Block(const Vector3& pos, const Vector3& scale)
{
	Initialize();
	position = pos;
	this->scale = scale;

	/*Library::setScale(scale, modelData, heapNum);
	Library::setPosition(position, modelData, heapNum);*/
	modelData.SetScale(scale, heapNum);
	modelData.SetPosition(position, heapNum);
}


Block::~Block()
{
}

void Block::LoadResource()
{
	std::string mtl;

	//modelData.key = "block";
	////Library::loadOBJVertex("Resources/Model/testSnake.obj", true, true, &mtl, modelData);
	////Library::loadOBJMaterial("Resources/Model/", mtl, CREATE_NUMBER, modelData);
	//Library::create3DBox({ 1,1,1 }, modelData);
	//Library::createHeapData2({ 150,150,150,255 }, CREATE_NUMBER, modelData);

	modelData.CreateBox({ 1,1,1 }, { 150,150,150,255 }, CREATE_NUMBER);
}

void Block::SetHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void Block::Initialize()
{

	SetHeapNum();

	velocity = 0;
	collisionFlag.box = true;
	boxData.resize(1);


	scale = 1;


	//Library::setScale(scale, modelData, heapNum);
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetScale(scale, heapNum);
	modelData.SetPosition(position, heapNum);

}

void Block::Update()
{

	if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET)
		boxData[0].size = scale + Vector3(15, 0, 15);
	else
		boxData[0].size = scale;
	
	boxData[0].position = position;

}

void Block::Draw()
{
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);
}

void Block::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

void* Block::GetPtr()
{
	return this;
}

void Block::MovePosition(const Vector3& vector)
{
	position += vector;
	modelData.SetPosition(position, heapNum);
}
