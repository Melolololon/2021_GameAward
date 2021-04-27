#include "ShotEnemy.h"
#include "EnemyBullet.h"
#include "ObjectManager.h"

ModelData ShotEnemy::modelData;
int ShotEnemy::createCount;
const int ShotEnemy::CREATE_NUMBER = 1;

ShotEnemy::ShotEnemy()
{
	initialize();
}

void ShotEnemy::initialize()
{
	setHeapNum();
	hp = 2;

	collisionFlag.sphere = true;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = OBJSIZE / 2;
}

void ShotEnemy::update()
{
	//ƒvƒŒƒCƒ„[‚Ö‚Ì•ûŒüƒxƒNƒgƒ‹‚ð‹‚ß‚é
	velocity = { pPlayer->getHeadPosition().x - position.x, pPlayer->getHeadPosition().y - position.y, pPlayer->getHeadPosition().z - position.z };
	//³‹K‰»
	velocity = vector3Normalize(velocity);

	//ˆê’èŠÔŠuˆÈã‚È‚çÀ•WXV
	if (sqrt((pPlayer->getHeadPosition().x - position.x) * (pPlayer->getHeadPosition().x - position.x) +
		(pPlayer->getHeadPosition().y - position.y) * (pPlayer->getHeadPosition().y - position.y) +
		(pPlayer->getHeadPosition().z - position.z) * (pPlayer->getHeadPosition().z - position.z)) >= 20 && shotWaitTimer == 60)
	{
		position = position + velocity * moveSpeed;
	}
	//‹——£‚ªˆê’è–¢–ž‚¾‚Á‚½‚ç’âŽ~E’e‚ðŒ‚‚Â
	else
	{
		//‚±‚±‚É’e‚ðŒ‚‚Âˆ—
		if (shotWaitTimer == 60)
			ObjectManager::getInstance()->addObject(std::make_shared<EnemyBullet>(position, velocity));

		shotWaitTimer--;

	}
	if (shotWaitTimer < 0) shotWaitTimer = 60;
	setPosition(position);

}

void ShotEnemy::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::drawGraphic(modelData, heapNum);

}

void ShotEnemy::hit(const Object* const object, const CollisionType& collisionType, const int& arrayNum)
{
}

void ShotEnemy::loadModel()
{
	std::string mtl;

	modelData.key = "shotenemy";
	Library::create3DBox(Vector3{ OBJSIZE,OBJSIZE,OBJSIZE }, modelData);
	Library::createHeapData2({ 200,112,28,255 }, CREATE_NUMBER, modelData);
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
	Library::setPosition(position, modelData, heapNum);
}