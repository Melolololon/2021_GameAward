#include "StageSelect.h"
#include<fstream>
#include"ObjectManager.h"
StageSelect::StageSelect()
{
	//�}�b�v�ǂݍ���
	
	float targetDis;
	float playerDis;
	int targetNum;
	int blockSize;
	std::vector<Block*>blockVector;

	for (int i = 0; ; i++)
	{
		std::ifstream openFile;
		openFile.open(L"ms_map" + std::to_wstring(i) + L".msmap", std::ios_base::binary);
		
		//�S���ǂݍ��ݏI�������(�J���̎��s������)������
		//�����ŏ��Ƀp�X�S���擾���Ă��̕��������[�v�����ق�������?
		if (!openFile)break;

		auto readData = [&](auto* readValue)
		{
			openFile.read(static_cast<char*>(static_cast<void*>(readValue)), sizeof(*readValue));
		};

		//�K���m�̋����ǂݍ���
		readData(&targetDis);

		//�v���C���[�Ƃ̋����ǂݍ���
		readData(&playerDis);

		//�K���ǂݍ���
		readData(&targetNum);

		//�u���b�N���ǂݍ���
		readData(&blockSize);

		//�u���b�N�ǂݍ���
		int loopCount = 0;
		//�K�Z�b�g���邽�߂ɍ��W�擾����p
		Block* leftBlock;
		Block* downBlock;
		//�ꎞ�I�ɓ������
		Block* block;
		//���T�C�Y
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


		//�ǉ�
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

