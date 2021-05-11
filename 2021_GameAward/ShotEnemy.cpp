#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"
#include "Block.h"
#include "Player.h"
#include "PlayerBullet.h"

PrimitiveModel ShotEnemy::modelData;
int ShotEnemy::createCount;
const int ShotEnemy::CREATE_NUMBER = 1;

ShotEnemy::ShotEnemy()
{
	Initialize();
}

void ShotEnemy::Initialize()
{
	setHeapNum();
	hp = 2;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
}

void ShotEnemy::Update()
{
	//ƒvƒŒƒCƒ„[‚Ö‚Ì•ûŒüƒxƒNƒgƒ‹‚ð‹‚ß‚é
	velocity = { pPlayer->GetHeadPosition().x - position.x, pPlayer->GetHeadPosition().y - position.y, pPlayer->GetHeadPosition().z - position.z };
	//³‹K‰»
	velocity = Vector3Normalize(velocity);

	//ˆê’èŠÔŠuˆÈã‚È‚çÀ•WXV
	if (sqrt((pPlayer->GetHeadPosition().x - position.x) * (pPlayer->GetHeadPosition().x - position.x) +
		(pPlayer->GetHeadPosition().y - position.y) * (pPlayer->GetHeadPosition().y - position.y) +
		(pPlayer->GetHeadPosition().z - position.z) * (pPlayer->GetHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		if (attackAfterTimer == 60 * 2)
		{
			position = position + velocity * moveSpeed;
			setPosition(position);
		}
		else
		{
			attackAfterTimer--;
			if (attackAfterTimer < 0)
				attackAfterTimer = 60 * 2;
		}
	}
	//‹——£‚ªˆê’è–¢–ž‚¾‚Á‚½‚ç’âŽ~E’e‚ðŒ‚‚Â
	else
	{
		//‚±‚±‚É’e‚ðŒ‚‚Âˆ—
		if (shotWaitTimer == 60)
			ObjectManager::GetInstance()->AddObject(std::make_shared<EnemyBullet>(position, velocity));

		shotWaitTimer--;

		if (attackAfterTimer != 60 * 2)
		{
			attackAfterTimer--;
			if (attackAfterTimer < 0)
				attackAfterTimer = 60 * 2;
		}

	}

	if (shotWaitTimer < 0) shotWaitTimer = 60;

}

void ShotEnemy::Draw()
{
	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelData, heapNum);
	modelData.Draw(heapNum);

}

void ShotEnemy::LoadResource()
{
//	std::string mtl;
//
//	modelData.key = "shotenemy";
//	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
//	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
	modelData.CreateBox({ OBJSIZE,OBJSIZE,OBJSIZE }, { 200,112,28,255 }, CREATE_NUMBER);
}

void ShotEnemy::setHeapNum()
{
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CREATE_NUMBER ? 0 : createCount;
}

void ShotEnemy::setPosition(Vector3 pos)
{
	position = pos;
	sphereData[0].position = position;
	//Library::setPosition(position, modelData, heapNum);
	modelData.SetPosition(position, heapNum);
}