#include "Block.h"

#include"Play.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include"PlayerBullet.h"
#include"Random.h"
#include"LibMath.h"

ObjModel Block::treeModelData;
const int Block::CREATE_NUMBER = 5000;
HeapIndexManager Block::heapIndexManager(CREATE_NUMBER);


Block::Block(const Vector3& pos, const Vector3& scale)
{
	Initialize();
	position = pos;

	this->scale = scale;

	//クランプ
	this->scale.x = LibMath::MultipleClamp(scale.x, treeScale.x,true );
	this->scale.z = LibMath::MultipleClamp(scale.z, treeScale.z ,true);

	//小さかったら木のサイズと同じに
	this->scale.x = scale.x < treeScale.x ? treeScale.x : scale.x;
	this->scale.z = scale.z < treeScale.z ? treeScale.z : scale.z;

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
	//クランプして、元々のサイズより小さくなったから、隙間できてる

	//木の配置
	Vector3 leftUpPos = position + Vector3(-scale.x / 2, 0, scale.z / 2);
	for (int z = 0 ,loopNumZ = static_cast<int>(scale.z / treeScale.z); z < loopNumZ; z++)
	{
		for (int x = 0, loopNumX = static_cast<int>(scale.x / treeScale.x); x < loopNumX; x++)
		{
			Vector3 randNum = 0;
			
			float treeMag = treeScale.x;
			if (scale.x >= treeScale.x * treeMag
				&& scale.z >= treeScale.z * treeMag) 
			{
				randNum = Vector3
				(
					Random::GetRandomNumberRangeSelect(-20.0f, 20.0f) / 10,
					0,
					Random::GetRandomNumberRangeSelect(-20.0f, 20.0f) / 10
				);
			}

			treeModelData.SetPosition
			(
				leftUpPos + Vector3(x * treeScale.x + 2.5f, 0 , -z * treeScale.z -3.5f) + randNum,
				heapNums[z * loopNumX + x]
			);
		}
	}
}
