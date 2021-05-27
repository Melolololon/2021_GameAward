#pragma once
#include <Object.h>
#include"Sprite3D.h"
#include"Texture.h"
class EnemyParticle :
    public Object
{
private:

	Sprite3D parSpr;
	static std::vector<Texture> parTex;

public:
	EnemyParticle(const Vector3& pos);
	~EnemyParticle();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const Object* const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	static void LoadResources();
};

