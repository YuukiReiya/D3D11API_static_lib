#include "stdafx.h"
#include "MeshReadHelper.h"
#include "MyGame.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11::Helper
*/
using namespace D3D11::Helper;

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

MeshReadHelper::ReadBuffer D3D11::Helper::MeshReadHelper::Read(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return ReadBuffer();
	}

	ReadBuffer ret;
	string buf;

	//	バッファに文字列獲得
	getline(ifs, buf);
	string t = "}";

	//	{1 1 1}{2 2 2}

	//頂点
	while (true)
	{
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1 1
		auto b = buf.substr(0, a);

		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}
		//	1 1 1
		{
			Graphic::MeshVertex tmp;

			//	x
			auto d = b.find(" ");
			auto e = b.substr(0, d);
			
			tmp.m_Pos.x = stof(e);
			b = b.substr(d + 1);

			//	y
			auto f = b.find(" ");
			auto g = b.substr(0, f + 1);

			tmp.m_Pos.y = stof(g);
			b = b.substr(f + 1);

			//	z
			tmp.m_Pos.z = stof(b);
			ret.vertices.push_back(tmp);
		}

		buf = buf.substr(a + 1);
	}

	getline(ifs, buf);
	//インデックス
	while (true)
	{
		auto a = buf.find(" ");
		if (a == string::npos) { break; }
		auto b = buf.substr(0, a);

		ret.indices.push_back(stoi(b));

		buf = buf.substr(a + 1);
	}

	return ret;
}
