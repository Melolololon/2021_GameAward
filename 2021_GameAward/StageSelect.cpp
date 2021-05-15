#include "StageSelect.h"

#include<fstream>

#include"ObjectManager.h"
#include"Play.h"
#include"Quaternion.h"
#include"LibMath.h"

#include"Player.h"
#include"XInputManager.h"

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
std::vector<Vector3>StageSelect::mapMovePositions;
std::vector<float>StageSelect::worldCenterToStageVectorAngle;
StageSelect::StageSelectState StageSelect::stageSelectState = StageSelect::STAGE_SELECT_STATE_SELECT;

//const UINT StageSelect::playerRotateTime = 60 * 2;
//const UINT StageSelect::nextFromSelectionTime = 60 * 2;

StageSelect::StageSelect()
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::LoadResources()
{
	//�}�b�v�ǂݍ���
	for (int i = 0; ; i++)
	{
		const std::wstring path = L"Resources/Map/ms_map" + std::to_wstring(i + 1) + L".msmap";
		std::ifstream openFile;
		openFile.open(path, std::ios_base::binary);

		//�S���ǂݍ��ݏI�������(�J���̎��s������)������
		//�����ŏ��Ƀp�X�S���擾���Ă��̕��������[�v�����ق�������?
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

			blocks[i][j] = std::make_shared<Block>(blockPos, blockScale);
			
		}

		openFile.close();
	}

	
	//�}�b�v�𒆐S����ǂ̂��炢��������(1�}�b�v���������W�p�ӂ��ăN�H�[�^�j�I���ŉ�)
	const Vector3 mapMovePos = { 0,0,550 };
	const float mapRotateAngle = 360.0f / maxStageNum;
	mapMovePositions.resize(maxStageNum);
	worldCenterToStageVectorAngle.resize(maxStageNum);
	for(int i = 0; i < maxStageNum;i++)
	{
		auto blockNum = blocks[i].size();
		Vector3 movePos = mapMovePos;
		float rotateAngle = mapRotateAngle * i * -1;

		movePos = LibMath::RotateVector3(movePos, { 0,1,0 }, rotateAngle);
		for(int j = 0; j < blockNum;j++)
			blocks[i][j]->MovePosition(movePos);

		mapMovePositions[i] = movePos;
		
		Vector3 worldCenterToStageVector = LibMath::OtherVector({ 0,0,0 }, mapMovePositions[i]);
		worldCenterToStageVectorAngle[i] = LibMath::Vecto2ToAngle
		(
			Vector2(worldCenterToStageVector.x, worldCenterToStageVector.z),
			true
		);

	}

}

void StageSelect::Initialize()
{
	for (const auto& b1 : blocks)
	{
		for(const auto& b2 : b1)
		ObjectManager::GetInstance()->AddObject(b2);
	}
	Library::SetCamera({ 0,1400,0 }, { 0 ,0,  2 }, { 0,0,1 });

	playerMoveVector = Vector3(-250,0, 270);
	Vector3 playerModelMoveVector = playerMoveVector + mapMovePositions[selectStageNum];
	player = std::make_shared<Player>(playerModelMoveVector);
	ObjectManager::GetInstance()->AddObject(player);
	player->SetTargetPosition(mapMovePositions);
	//playerRotateTimer = 0;

	stageSelectState = StageSelect::STAGE_SELECT_STATE_SELECT;
	nextSceneTimer.SetMaxTime(60 * 2);

}

void StageSelect::Update()
{
	//�X�e�[�W�Z���N�g
	float inputAngle;
	if (XInputManager::GetPadConnectedFlag(1))
		inputAngle = XInputManager::LeftStickAngle(1);
	else
		inputAngle = DirectInput::ArrowKeyAngle();

	auto worldCenterToStageVectorAngleSize = worldCenterToStageVectorAngle.size();
	for (int i = 0; i < worldCenterToStageVectorAngleSize;i++)
	{
		if(LibMath::AngleDifference(inputAngle, worldCenterToStageVectorAngle[i],20.0f))
		{
			selectStageNum = i;
		
			Vector3 playerModelMoveVector = playerMoveVector + mapMovePositions[i];
			player->SetModelMoveVector(playerModelMoveVector);
			break;
		}
	}

	bool padSelect = XInputManager::GetPadConnectedFlag(1)
		&& XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1);
	if (DirectInput::KeyTrigger(DIK_Z)
		|| padSelect) 
	{
		nextSceneTimer.SetStopFlag(false);
		stageSelectState = StageSelect::STAGE_SELECT_STATE_SELECT_END;
	}


	if (nextSceneTimer.GetSameAsMaximumFlag())
		isEnd = true;

	ObjectManager::GetInstance()->Update();
	
	//�f�o�b�O�p
	//isEnd = true;
}

void StageSelect::Draw()
{
	ObjectManager::GetInstance()->Draw();
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

	ObjectManager::GetInstance()->AllEraseObject();

}

Scene* StageSelect::GetNextScene()
{
	return new Play();
}

