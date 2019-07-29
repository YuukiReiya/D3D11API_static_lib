#include "stdafx.h"
#include "MeshReadHelper.h"
#include "MyGame.h"
#include <DirectXMath.h>

/*!
	@brief	usingディレクティブ
	@using	D3D11::Helper
*/
using namespace D3D11::Helper;

/*!
	@brief	usingディレクティブ
	@using	DirectX
*/
using namespace DirectX;

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
	string t = "}";

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

	//uv
	getline(ifs, buf);
	while (true)
	{
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1
		auto b = buf.substr(0, a);

		//	 1 1
		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}

		//	保存用の一時バッファ
		Graphic::MeshVertex tmp;

		//	x
		auto d = b.find(" ");
		auto e = b.substr(0, d);

		b = b.substr(d + 1);

		//	y
		tmp.uv =
		{
			stof(e),
			stof(b)
		};

		ret.vertices.push_back(tmp);
		//it.uv =
		//{
		//	stof(e),
		//	stof(b)
		//};

		buf = buf.substr(a + 1);
	}

	//	バッファに文字列獲得
	getline(ifs, buf);


	//	{1 1 1}{2 2 2}

	if (!ret.vertices.empty()) {

		//頂点
		//int index = 0;
		//while (true)
		for (size_t i = 0; i < ret.vertices.size(); i++)
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
				//Graphic::MeshVertex& tmp = ret.vertices[index++];
				//	x
				auto d = b.find(" ");
				auto e = b.substr(0, d);

				//tmp.position.x = stof(e);
				ret.vertices[i].position.x = stof(e);

				b = b.substr(d + 1);

				//	y
				auto f = b.find(" ");
				auto g = b.substr(0, f + 1);

				//tmp.position.y = stof(g);
				ret.vertices[i].position.y = stof(g);

				b = b.substr(f + 1);

				//	z
				//tmp.position.z = stof(b);
				ret.vertices[i].position.z = stof(b);
				//ret.vertices.push_back(tmp);
			}

			buf = buf.substr(a + 1);
		}
	}
	else {
		//頂点
		while (true)
		{
			auto a = buf.find(t);
			if (a == string::npos) { break; }

			Graphic::MeshVertex tmp;

			//	{1 1 1
			auto b = buf.substr(0, a);

			auto c = b.find("{");
			if (c != string::npos) {
				b = b.substr(c + 1);
			}
			//	1 1 1
			{
				//Graphic::MeshVertex& tmp = ret.vertices[index++];
				//	x
				auto d = b.find(" ");
				auto e = b.substr(0, d);

				tmp.position.x = stof(e);

				b = b.substr(d + 1);

				//	y
				auto f = b.find(" ");
				auto g = b.substr(0, f + 1);

				tmp.position.y = stof(g);
				//ret.vertices[index].position.x = stof(g);

				b = b.substr(f + 1);

				//	z
				tmp.position.z = stof(b);
				//ret.vertices[index].position.x = stof(b);
				ret.vertices.push_back(tmp);
			}

			buf = buf.substr(a + 1);
		}
	}

	return ret;
}

D3D11::Graphic::FrameAnimationMesh D3D11::Helper::MeshReadHelper::ReadFrameAnim(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return D3D11::Graphic::FrameAnimationMesh();
	}

	D3D11::Graphic::FrameAnimationMesh ret;
	string buf;
	const string_view c_Space = " ";
	const string_view c_Comma = ",";
	const string_view c_Slash = "/";

	//	頂点インデックス
	getline(ifs, buf);
	while (true)
	{
		auto spaceOffset = buf.find(" ");
		if (spaceOffset == string::npos) { break; }
		auto value = buf.substr(0, spaceOffset);
		ret.indices.push_back(stoi(value));
		buf = buf.substr(spaceOffset + 1);
	}

	//	UV
	getline(ifs, buf);
	if (!buf.empty()) {

		//TODO:UVの読み込み処理
	}

	//	座標
	while (getline(ifs, buf))
	{
		unsigned int animIndex = 0;
		unsigned int frameIndex = 0;

		//	
		while (!buf.empty())
		{
			auto commaOffset = buf.find(c_Comma);
			float x = stof(buf.substr(0, commaOffset));
			buf = buf.substr(commaOffset + 1);
			commaOffset = buf.find(c_Comma);
			float y= stof(buf.substr(0, commaOffset));
			buf = buf.substr(commaOffset + 1);
			auto spaceOffset = buf.find(c_Space);
			float z = stof(buf.substr(0, spaceOffset));
			buf = buf.substr(spaceOffset + 1);

			//	リストに追加
			ret.vertices[animIndex][frameIndex].push_back({ x,y,z });
			frameIndex++;


			if (buf.find(c_Slash) == 0) {
				animIndex++;
				frameIndex = 0;
				buf = buf.substr(1);
			}
		}


	}

	return ret;
}