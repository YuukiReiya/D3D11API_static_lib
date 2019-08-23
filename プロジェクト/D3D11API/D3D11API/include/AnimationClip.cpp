#include "stdafx.h"
#include "AnimationClip.h"
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

constexpr string_view c_Comma = ",";
constexpr string_view c_Space = " ";

bool API::AnimationClip::Load(AnimationClip * animClip, std::string filePath)
{
	ifstream ifs;
	ifs.open(filePath);
	
	string buf;

	//	ファイルが開けない
	if (ifs.fail()) {
		ifs.close();
		animClip = NULL;
		return false;
	}

	//	ボーン分
	while (true)
	{
		getline(ifs, buf);
		if (buf.empty()) { break; }
		API::MatrixPalette matrixPalette;

		//	フレーム分
		while (true)
		{
			DirectX::XMMATRIX mat;
			if (buf.empty()) { break; }

			//	行列
			constexpr int c_ElemCount = 15;//行列の要素数(16) - 1
			int index = 0;
			while (index < c_ElemCount)
			{
				auto commaOffset = buf.find(c_Comma);
				if (commaOffset == string::npos) { return false; }
				float elem = stof(buf.substr(0, commaOffset));
				int rowIndex = index / 4;
				int columIndex = index % 4;
				mat.r[rowIndex].m128_f32[columIndex] = elem;
				buf = buf.substr(commaOffset + 1);
				index++;
			}
			auto spaceOffset = buf.find(c_Space);
			if (spaceOffset == string::npos) { return false; }
			float lastElem = stof(buf.substr(0, spaceOffset));
			mat.r[3].m128_f32[3] = lastElem;

			matrixPalette.push_back(mat);
			buf = buf.substr(spaceOffset + 1);
		}
		animClip->matrixPalette.push_back(matrixPalette);
	}


	return true;
}
