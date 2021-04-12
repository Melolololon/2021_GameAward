#include "Play.h"
#include "MoveEnemy.h"
#include"ObjectManager.h"
#include"XInputManager.h"

#pragma region オブジェクト


#pragma endregion


font font1;
pipeline mosaicPL;
pipeline superMosaicPL;

Play::Play()
{
	font1 = Library::loadSpriteFont(L"Resources/Texture/Font/font.png", { 14,7 });

	mosaicPL = Library::createUserPostEffectPipelineState({L"LittleMosaicPixelShader.hlsl","PSmain","ps_5_0"});
	superMosaicPL = Library::createUserPostEffectPipelineState({L"LostMosaicPixelShader.hlsl","PSmain","ps_5_0"});
}
Play::~Play(){}

ModelData m;
void Play::initialize()
{
	player = new Player();
	enemy = new MoveEnemy();
	enemy->setPPlayer(player);
	ObjectManager::getInstance()->addObject(player);
	ObjectManager::getInstance()->addObject(enemy);

	m.key = "fierd";
	Library::createBoard({ 1000,1000 }, m);
	Library::createHeapData2({ 0,255,0,255 }, 1, m);
	Library::setAngle({ 90,0,0 }, m, 0);

#pragma region カメラ
	cameraPosition = { 0,30,-2 };
	cameraTarget = { 0,0,0 };
#pragma endregion

}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

#pragma region カメラ移動
	Vector3 pHeapPos = player->getHeadPosition();
	cameraPosition = pHeapPos;
	cameraPosition += {0, 30, -2};
	cameraTarget = pHeapPos;
	Library::setCamera(cameraPosition, cameraTarget, { 0,1,0 });

#pragma endregion

}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	//Library::drawGraphic(m, 0);
	if(XInputManager::getPadConnectedFlag(1))
	Library::drawsSpriteFontString
	(
		{ 0,0 }, 
		{ 30,30 },
		"StickAngle " + std::to_string(static_cast<int>(XInputManager::leftStickAngle(1))),
		&font1
	);

	Library::drawsSpriteFontString
	(
		{ 0,40 },
		{ 30,30 },
		"Z_Key Mosaic",
		&font1
	);

	Library::drawsSpriteFontString
	(
		{ 0,80 },
		{ 30,30 },
		"X_Key SuperMosaic" ,
		&font1
	);

	Library::drawsSpriteFontString
	(
		{ 800,0 },
		{ 30,30 },
		"LeftStick Move",
		&font1
	);

	Library::drawsSpriteFontString
	(
		{ 800,40 },
		{ 30,30 },
		"RB Rotation",
		&font1
	);

	if(DirectInput::keyState(DIK_Z))
		Library::setPostEffectPipeline(mosaicPL);
	else
	if(DirectInput::keyState(DIK_X))
		Library::setPostEffectPipeline(superMosaicPL);
	else
		Library::setPostEffectPipeline(0);

}

void Play::end()
{
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::getNextScene()
{
	return "";
}
