#pragma once
#include<unordered_map>
#include"Vector.h"
class ValueLoader
{

	ValueLoader();
	~ValueLoader();
public:
	ValueLoader(ValueLoader& v) = delete;
	ValueLoader operator=(ValueLoader& v) = delete;
	static ValueLoader* getInstance();

	void loadValue
	(
		const std::string& path,
		std::unordered_map<std::string, int>& iMap,
		std::unordered_map<std::string, float>& fMap,
		std::unordered_map<std::string, Vector3>& v3Map
	);
};

