#include "StageSelect.h"

#include<fstream>


#include"Play.h"
#include"Title.h"

#include"ObjectManager.h"

#include"Quaternion.h"
#include"LibMath.h"

#include"Player.h"
#include"XInputManager.h"
#include"Fade.h"

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
float StageSelect::mapRotateAngleMax = 0.0f;

Sprite2D StageSelect::returnTitleSpr;
Texture StageSelect::returnTitleTex;
Sprite2D StageSelect::selectSpr;
Texture StageSelect::selectTex;
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
	returnTitleSpr.CreateSprite();
	returnTitleSpr.SetPosition(Vector2(838, 610));
	returnTitleSpr.SetScale(Vector2(0.8f, 0.8f));
	returnTitleTex.LoadSpriteTexture("Resources/Texture/returnTitle.png");

	selectSpr.CreateSprite();
	selectSpr.SetPosition(Vector2(860, 520));
	selectSpr.SetScale(Vector2(0.8f, 0.8f));
	selectTex.LoadSpriteTexture("Resources/Texture/select.png");

	//マップ読み込み
	for (int i = 0; ; i++)
	{
		const std::wstring path = L"Resources/Map/ms_map" + std::to_wstring(i + 1) + L".msmap";
		std::ifstream openFile;
		openFile.open(path, std::ios_base::binary);

		bool loadTutorialMap = false;
		if (!openFile)
		{
			openFile.open(L"Resources/Map/ms_mapT.msmap", std::ios_base::binary);
			loadTutorialMap = true;
		}
		else 
		{
			maxStageNum++;
		}
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
			
		}

		openFile.close();

		if (loadTutorialMap)break;
	}

	
	//マップを中心からどのくらい動かすか(1マップ分だけ座標用意してクォータニオンで回す)
	const Vector3 mapMovePos = { 0,0,-550 };
	mapRotateAngleMax = 360.0f / maxStageNum;
	mapMovePositions.resize(maxStageNum);
	worldCenterToStageVectorAngle.resize(maxStageNum);
	for(int i = 0; i < maxStageNum;i++)
	{
		auto blockNum = blocks[i].size();
		Vector3 movePos = mapMovePos;
		float rotateAngle = mapRotateAngleMax * i;

		movePos = LibMath::RotateVector3(movePos, { 0,1,0 }, rotateAngle);
		for (int j = 0; j < blockNum; j++) 
		{
			blocks[i][j]->MovePosition(movePos);
		}
		mapMovePositions[i] = movePos;
	

	}

	Library::LoadSound("Resources/Sound/BGM/StageSelect.wav", "StageSelect", true);


}

void StageSelect::Initialize()
{
	int count = 0;
	for (const auto& b1 : blocks)
	{
		if(count == maxStageNum)
		{
			break;
		}

		for (const auto& b2 : b1) 
		{
			ObjectManager::GetInstance()->AddObject(b2);
		}
		count++;
	}

	Vector3 cameraPosition = { 0,1400,-400 };
	Vector3 cameraTarget = { 0 ,0, 0 };

	Vector3 upVector = LibMath::OtherVector(cameraPosition, cameraTarget);
	upVector = LibMath::RotateVector3(upVector, { 1,0,0 }, 90);
	Library::SetCamera(cameraPosition, cameraTarget, upVector);

	playerMoveVector = Vector3(-250,0, 270);
	Vector3 playerModelMoveVector = playerMoveVector + mapMovePositions[selectStageNum];


	stageSelectState = StageSelect::STAGE_SELECT_STATE_SELECT;
	nextSceneTimer.SetMaxTime(60 * 0.5);

	Library::PlayLoadSound("StageSelect");
}

void StageSelect::Update()
{
	//ステージセレクト
	if (XInputManager::GetPadConnectedFlag(1)
		&& stageRotateState == StageRotateState::STAGE_ROTATE_NOT_ROTATE
		&& stageSelectState == StageSelectState::STAGE_SELECT_STATE_SELECT)
	{
		if (XInputManager::LeftStickLeft(30, 1))
		{
			stageRotateState = StageRotateState::STAGE_ROTATE_LEFT;
			
			selectStageNum--;
			if (selectStageNum < 0)selectStageNum = maxStageNum - 1;
			
		}
		else if (XInputManager::LeftStickRight(30, 1))
		{
			stageRotateState = StageRotateState::STAGE_ROTATE_RIGHT;
			
			selectStageNum++;
			if (selectStageNum == maxStageNum)selectStageNum = 0;
		}

	}
	else if(stageRotateState != StageRotateState::STAGE_ROTATE_NOT_ROTATE)
	{
		//回転終了
		if (mapRotateAngleCount >= mapRotateAngleMax )
		{
			stageRotateState = StageRotateState::STAGE_ROTATE_NOT_ROTATE;
			mapRotateAngleCount = 0.0f;
		}
		else//回転
		{

			mapRotateAngleCount += MAP_ROTATE_SPEED;

			
			for (int i = 0; i < maxStageNum; i++)
			{
				auto blockNum = blocks[i].size();
				float rotSpeed = MAP_ROTATE_SPEED;
				if (stageRotateState == StageRotateState::STAGE_ROTATE_RIGHT)rotSpeed *= -1;

				Vector3 rotatePos = LibMath::RotateVector3(mapMovePositions[i], { 0,1,0 }, rotSpeed);
				Vector3 movePos = mapMovePositions[i] - rotatePos;
				movePos *= -1;
				for (auto& block:blocks[i])
				{
					block->MovePosition(movePos);
				}
				mapMovePositions[i] = rotatePos;
			}

		}
	}



	bool padStageSelect = (XInputManager::ButtonTrigger(XInputManager::XINPUT_X_BUTTON, 1)
		|| XInputManager::ButtonTrigger(XInputManager::XINPUT_A_BUTTON, 1))
		&& XInputManager::GetPadConnectedFlag(1)
		&& stageSelectState == StageSelect::STAGE_SELECT_STATE_SELECT
		&& stageRotateState == StageRotateState::STAGE_ROTATE_NOT_ROTATE;


	bool padReturnTitle = XInputManager::ButtonTrigger(XInputManager::XINPUT_B_BUTTON, 1)
		&& XInputManager::GetPadConnectedFlag(1)
		&& stageSelectState == StageSelect::STAGE_SELECT_STATE_SELECT
		&& stageRotateState == StageRotateState::STAGE_ROTATE_NOT_ROTATE;

	//ステージ決定処理
	if (padStageSelect) 
	{

		if (stageSelectState == StageSelect::STAGE_SELECT_STATE_SELECT
			&& Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT())
		{
			Library::PlaySoundEveryLoad("Resources/Sound/SE/SystemSE/SneakTitleSe.wav");
			nextSceneTimer.SetStopFlag(false);
			stageSelectState = StageSelect::STAGE_SELECT_STATE_SELECT_END;
		}
	}

	//タイトルに戻る処理
	if (padReturnTitle)
	{
		if (Fade::GetInstance()->GetFadeState() == Fade::FADE_NOT())
		{
			Library::PlaySoundEveryLoad("Resources/Sound/SE/SystemSE/SneakTitleSe.wav");
			Fade::GetInstance()->FadeStart();
			stageSelectState = StageSelect::STAGE_SELECT_STATE_RETURN_TITLE;
		}
	}


	if (nextSceneTimer.GetSameAsMaximumFlag()) 
	{
		Fade::GetInstance()->FadeStart();
	}
	if (Fade::GetInstance()->GetSceneChangeTimingFlag()) 
	{
		isEnd = true;
	}

	ObjectManager::GetInstance()->Update();
	
}

void StageSelect::Draw()
{
	ObjectManager::GetInstance()->Draw();

	returnTitleSpr.Draw(&returnTitleTex);
	selectSpr.Draw(&selectTex);

	Fade::GetInstance()->Draw();
}



void StageSelect::Finitialize()
{
	//情報セット
	if (stageSelectState == StageSelect::StageSelectState::STAGE_SELECT_STATE_SELECT_END)
	{
		Play::SetStageData
		(
			selectStageNum,
			blockPositions[selectStageNum],
			blockScales[selectStageNum],
			targetDistance[selectStageNum],
			playerDistance[selectStageNum],
			targetNumbers[selectStageNum],
			leftUpPositions[selectStageNum],
			rightDownPositions[selectStageNum]
		);
	}

	ObjectManager::GetInstance()->AllEraseObject();

	Library::StopLoadSound("StageSelect", false);
}

void StageSelect::SetTutorialData()
{
	//情報セット
	Play::SetStageData
	(
		maxStageNum,
		blockPositions[maxStageNum],
		blockScales[maxStageNum],
		targetDistance[maxStageNum],
		playerDistance[maxStageNum],
		targetNumbers[maxStageNum],
		leftUpPositions[maxStageNum],
		rightDownPositions[maxStageNum]
	);

	ObjectManager::GetInstance()->AllEraseObject();

}

Scene* StageSelect::GetNextScene()
{
	if (stageSelectState == StageSelect::StageSelectState::STAGE_SELECT_STATE_SELECT_END)
	{
		return new Play();
	}
	return new Title();
}

