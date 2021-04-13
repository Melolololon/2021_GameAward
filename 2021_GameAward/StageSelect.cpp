#include "StageSelect.h"
#include<fstream>
#include"ObjectManager.h"
StageSelect::StageSelect()
{
	//マップ読み込み
	
	float targetDis;
	float playerDis;
	int targetNum;
	int blockSize;
	std::vector<Block*>blockVector;

	for (int i = 0; ; i++)
	{
		std::ifstream openFile;
		openFile.open(L"ms_map" + std::to_wstring(i) + L".msmap", std::ios_base::binary);
		
		//全部読み込み終わったら(開くの失敗したら)抜ける
		//ここ最初にパス全部取得してその分だけループしたほうがいい?
		if (!openFile)break;

		auto readData = [&](auto* readValue)
		{
			openFile.read(static_cast<char*>(static_cast<void*>(readValue)), sizeof(*readValue));
		};

		//祠同士の距離読み込み
		readData(&targetDis);

		//プレイヤーとの距離読み込み
		readData(&playerDis);

		//祠数読み込み
		readData(&targetNum);

		//ブロック数読み込み
		readData(&blockSize);

		//ブロック読み込み
		int loopCount = 0;
		//祠セットするために座標取得する用
		Block* leftBlock;
		Block* downBlock;
		//一時的に入れるやつ
		Block* block;
		//リサイズ
		blockVector.resize(blockSize);
		for (auto& b : blockVector)
		{
			Vector3 blockPos;
			Vector3 blockScale;
			readData(&blockPos);
			readData(&blockScale);

			block = new Block(blockPos, blockScale);

			if (loopCount == 0)
				leftBlock = block;
			if (loopCount == 1)
				downBlock = block;

			ObjectManager::getInstance()->addObject(block);
			b = block;
			loopCount++;
		}

		openFile.close();


		//追加
		targetDistance.push_back(targetDis);
		playerDistance.push_back(playerDis);
		targetNumbers.push_back(targetNum);
		blocks.push_back(blockVector);
	}

}

StageSelect::~StageSelect()
{
}

void StageSelect::initialize()
{
}

void StageSelect::update()
{
}

void StageSelect::draw()
{
}

void StageSelect::end()
{
}

std::string StageSelect::getNextScene()
{
}

