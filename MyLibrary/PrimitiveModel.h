#pragma once
#include"Model.h"
#include"Color.h"
class PrimitiveModel :public Model
{
private:

	Color modelColor;
	std::vector<Vertex>vertices;

	static PipelineState defaultPipeline;

	void MapVertices();
	void CalcNormal();
public:
	PrimitiveModel();
	~PrimitiveModel();

#pragma region 開発者用関数
	static bool Initialize();
#pragma endregion


	void CreateBox
	(
		const Vector3& size,
		const Color& color,
		const int modelNum
	);
};

