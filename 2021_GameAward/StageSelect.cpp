#include "StageSelect.h"
#include<fstream>
#include"ObjectManager.h"
#include"Play.h"

std::vector<std::vector<std::unique_ptr<Block>>>StageSelect::blocks;
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
	for (int i = 0; ; i++)
	{

		const std::wstring path = L"Resources/Map/ms_map" + std::to_wstring(i + 1) + L".msmap";
		std::ifstream openFile;
		openFile.open(path, std::ios_base::binary);

		//�S���ǂݍ��ݏI�������(�J���̎��s������)������
		//�����ŏ��Ƀp�X�S���擾���Ă��̕��������[�v�����ق�������?
		if (!openFile)break;


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

		//�K���m�̋����ǂݍ���
		readData(&targetDistance[i]);

		//�v���C���[�Ƃ̋����ǂݍ���
		readData(&playerDistance[i]);

		//�K���ǂݍ���
		readData(&targetNumbers[i]);

		//�p�̍��W�擾
		readData(&leftUpPositions[i]);
		readData(&rightDownPositions[i]);

		//�u���b�N���ǂݍ���
		int blockNum;
		readData(&blockNum);

		//�u���b�N�ǂݍ���
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

			blocks[i][j] = std::make_unique<Block>(blockPos, blockScale);

		}

		openFile.close();


		//�ǉ�
		/*targetDistance.push_back(targetDis);
		playerDistance.push_back(playerDis);
		targetNumbers.push_back(targetNum);
		blocks.push_back(blockVector);
		blockPositions.push_back(blockPosVector);
		blockScales.push_back(blockScaleVector);*/

	}

}

void StageSelect::Initialize()
{
	selectStageNum = 0;


}

void StageSelect::Update()
{
	/*for (auto& b : blocks)
	{
		for (auto& b2 : b)
			b2.get()->Update();
	}*/

	isEnd = true;
}

void StageSelect::Draw()
{
	/*for (auto& b : blocks)
	{
		for (auto& b2 : b)
			b2.get()->Draw();
	}*/
}

void StageSelect::Finitialize()
{
	//���Z�b�g
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



}

Scene* StageSelect::GetNextScene()
{
	return new Play();
}

