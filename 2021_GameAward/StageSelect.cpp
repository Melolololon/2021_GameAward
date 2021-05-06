#include "StageSelect.h"

#include<fstream>

#include"ObjectManager.h"
#include"Play.h"
#include"Quaternion.h"
#include"LibMath.h"

int StageSelect::maxStageNum;
int StageSelect::selectStageNum;

std::vector<std::vector<std::shared_ptr<Block>>>StageSelect::blocks;
std::vector<std::vector<Vector3>>StageSelect::blockPositions;
std::vector<std::vector<Vector3>>StageSelect::blockScales;
std::vector<float>StageSelect::targetDistance;
std::vector<float>StageSelect::playerDistance;
std::vector<int>StageSelect::targetNumbers;
std::vector<Vector3>StageSelect::leftUpPositions;
std::vector<Vector3>StageSelect::rightDownPositions; 


StageSelect::StageSelect()
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::LoadResources()
{
	//マップ読み込み
	for (int i = 0; ; i++)
	{
		const std::wstring path = L"Resources/Map/ms_map" + std::to_wstring(i + 1) + L".msmap";
		std::ifstream openFile;
		openFile.open(path, std::ios_base::binary);

		//全部読み込み終わったら(開くの失敗したら)抜ける
		//ここ最初にパス全部取得してその分だけループしたほうがいい?
		if (!openFile)break;
		maxStageNum++;

		targetDistance.resize(i + 1);
		playerDistance.resize(i + 1);
		targetNumbers.resize(i + 1);
		blocks.resize(i + 1);
		blockPositions.resize(i + 1);
		blockScales.resize(i + 1);
		leftUpPositions.resize(i + 1);
		rightDownPositions.resize(i + 1);

		auto readData = [&](auto* readValue)
		{
			openFile.read(static_cast<char*>(static_cast<void*>(readValue)), sizeof(*readValue));
		};

		//祠同士の距離読み込み
		readData(&targetDistance[i]);

		//プレイヤーとの距離読み込み
		readData(&playerDistance[i]);

		//祠数読み込み
		readData(&targetNumbers[i]);

		//角の座標取得
		readData(&leftUpPositions[i]);
		readData(&rightDownPositions[i]);

		//ブロック数読み込み
		int blockNum;
		readData(&blockNum);

		//ブロック読み込み
		blocks[i].resize(blockNum);
		blockPositions[i].resize(blockNum);
		blockScales[i].resize(blockNum);
		for (int j = 0; j < blockNum; j++)
		{
			Vector3 blockPos;
			Vector3 blockScale;
			readData(&blockPos);
			readData(&blockScale);
			blockPositions[i][j] = blockPos;
			blockScales[i][j] = blockScale;

			blocks[i][j] = std::make_shared<Block>(blockPos, blockScale);
			ObjectManager::GetInstance()->AddObject(blocks[i][j]);
		}

		openFile.close();
	}

	
	//マップを中心からどのくらい動かすか(1マップ分だけ座標用意してクォータニオンで回す)
	const Vector3 mapMovePos = { 0,0,550 };
	const float mapRotateAngle = 360.0f / maxStageNum;
	
	for(int i = 0; i < maxStageNum;i++)
	{
		auto blockNum = blocks[i].size();
		Vector3 movePos = mapMovePos;
		float rotateAngle = mapRotateAngle * i;

		movePos = LibMath::RotateVector3(movePos, { 0,1,0 }, rotateAngle);
		for(int j = 0; j < blockNum;j++)
			blocks[i][j]->MovePosition(movePos);
		
	}

	
}

void StageSelect::Initialize()
{
	Library::SetCamera({ 0,1400,0 }, { 0 ,0,  2 }, { 0,0,1 });

	moveToAnotherStage = false;
	player = std::make_shared<Player>();
	ObjectManager::GetInstance()->AddObject(player);
}

void StageSelect::Update()
{

	ObjectManager::GetInstance()->Update();

	isEnd = true;



}

void StageSelect::Draw()
{
	ObjectManager::GetInstance()->Draw();
}

void StageSelect::Finitialize()
{
	//情報セット
	Play::SetStageData
	(
		blockPositions[selectStageNum],
		blockScales[selectStageNum],
		targetDistance[selectStageNum],
		playerDistance[selectStageNum],
		targetNumbers[selectStageNum],
		leftUpPositions[selectStageNum],
		rightDownPositions[selectStageNum]
	);

	ObjectManager::GetInstance()->AllEraseObject();

}

Scene* StageSelect::GetNextScene()
{
	return new Play();
}

