#include "Block.h"

#include"Play.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include"PlayerBullet.h"

ObjModel Block::treeModelData;
const int Block::CREATE_NUMBER = 5000;
HeapIndexManager Block::heapIndexManager(CREATE_NUMBER);


Block::Block(const Vector3& pos, const Vector3& scale)
{
	Initialize();
	position = pos;
	this->scale = scale;
	this->scale.x = scale.x < treeScale.x ? treeScale.x :scale.x;
	this->scale.z = scale.z < treeScale.z ? treeScale.z :scale.z;

	//scaleをtreeScaleの倍数にしないといけない

	//スケールx*スケールz = 表示モデル数
	heapNums.resize(static_cast<int>(this->scale.x / treeScale.x) * static_cast<int>(this->scale.z / treeScale.z));

	//添え字設定
	for (auto& h : heapNums)
	{
	
		h = heapIndexManager.GetHeapIndex();
	}

	SetModel();

	boxData[0].position = position;
}



Block::~Block()
{
	for (auto& h : heapNums) 
	{
		heapIndexManager.DrawEndCallFunction(h);
	}
}

void Block::LoadResource()
{
	treeModelData.LoadModel("Resources/Model/tree/tree.obj", true, CREATE_NUMBER, 0);
}

void Block::Initialize()
{
	collisionFlag.box = true;
	boxData.resize(1);

	for (auto& h : heapNums) 
	{
		treeModelData.SetScale(treeScale, h);
	}
}

void Block::Update()
{
	if (Play::GetPlaySceneState() == Play::PLAY_SCENE_SET_TARGET) 
	{
		boxData[0].size = scale + Vector3(15, 0, 15);
	}
	else 
	{
		boxData[0].size = scale;
	}
}

void Block::Draw()
{
	for (auto& h : heapNums) 
	{
		treeModelData.Draw(h);
	}
}

void Block::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
	Scene* currentScene = SceneManager::GetInstace()->GetCurrentScene();
	if (typeid(*currentScene) != typeid(StageSelect))return;

}

const void* Block::GetPtr()const
{
	return this;
}

void Block::MovePosition(const Vector3& vector)
{
	position += vector;
	boxData[0].position = position;

	SetModel();

}

void Block::SetModel()
{
	//木の配置
	Vector3 leftUpPos = position + Vector3(-scale.x / 2, 0, scale.z / 2);
	for (int z = 0 ,loopNumZ = static_cast<int>(scale.z / treeScale.z); z < loopNumZ; z++)
	{
		for (int x = 0, loopNumX = static_cast<int>(scale.x / treeScale.x); x < loopNumX; x++)
		{
		
			treeModelData.SetPosition
			(
				leftUpPos + Vector3(x * treeScale.x, 0, -z * treeScale.z),
				heapNums[z * loopNumX + x]
			);
		}
	}
}
