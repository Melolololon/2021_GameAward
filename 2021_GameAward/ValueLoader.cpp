#include "ValueLoader.h"
#include<fstream>

ValueLoader::ValueLoader(){}
ValueLoader::~ValueLoader(){}

ValueLoader* ValueLoader::getInstance()
{
	static ValueLoader v;
	return &v;
}

void ValueLoader::loadValue
(
	const std::string& path,
	std::unordered_map<std::string, int>& iMap,
	std::unordered_map<std::string, float>& fMap,
	std::unordered_map<std::string, Vector3>& v3Map
)
{
	iMap.reserve(20);
	fMap.reserve(20);
	v3Map.reserve(20);

	std::ifstream valueFire;
	valueFire.open(path);

	std::string paramStr;
	unsigned int strNum;
	char formatType;
	while (1) 
	{
		//�l���̕������ǂݎ��
		valueFire.read((char*)&strNum, sizeof(strNum));
		
		//�l���ǂݎ��
		paramStr.resize(strNum);
		valueFire.read((char*)paramStr.data(), strNum);
		
		//end��ǂݎ������ǂݎ��I��
		if (paramStr == "end")break;

		//�^���m�F
		valueFire.read(&formatType, 1);

		//�l�ǂݎ��&map�ɒǉ�
		if (formatType == 'i') 
		{
			int value;
			valueFire.read((char*)&value, sizeof(value));
			iMap.emplace(paramStr, value);
		}
		if (formatType == 'f')
		{
			float value;
			valueFire.read((char*)&value, sizeof(value));
			fMap.emplace(paramStr, value);
		}
		if(formatType == '3')
		{
			Vector3 value;
			float num;
		
			valueFire.read((char*)&num, sizeof(num));
			value.x = num;
			valueFire.read((char*)&num, sizeof(num));
			value.y = num;
			valueFire.read((char*)&num, sizeof(num));
			value.z = num;

			v3Map.emplace(paramStr, value);
		}
	}

	valueFire.close();

	
}
