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
		int index = 0;
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
				//Graphic::MeshVertex& tmp = ret.vertices[index++];
				//	x
				auto d = b.find(" ");
				auto e = b.substr(0, d);

				//tmp.position.x = stof(e);
				ret.vertices[index].position.x = stof(e);

				b = b.substr(d + 1);

				//	y
				auto f = b.find(" ");
				auto g = b.substr(0, f + 1);

				//tmp.position.y = stof(g);
				ret.vertices[index].position.x = stof(g);

				b = b.substr(f + 1);

				//	z
				//tmp.position.z = stof(b);
				ret.vertices[index].position.x = stof(b);
				//ret.vertices.push_back(tmp);
				index++;
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

MeshReadHelper::AnimReadBuffer D3D11::Helper::MeshReadHelper::ReadAnim(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return AnimReadBuffer();
	}

	AnimReadBuffer ret;
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
	//getline(ifs, buf);

	//	一時保存用のバッファ(UV未考慮)
	//auto uvbuffer = buf;

	//	UVのダミー
	getline(ifs, buf);

	//	アニメーションの数
	ret.animCount = stoi(buf);

	//	アニメーションの格納
	unsigned int animIndex = 0;

	for (size_t animIndex = 0; animIndex < ret.animCount; animIndex++)
	{
		//	1行読み取り
		getline(ifs, buf);
		if (buf.empty())break;

		//	フレーム数
		ret.frameCount[animIndex] = stoi(buf);

		//	フレーム数分ループ
		for (size_t count = 0; count < ret.frameCount[animIndex]; ++count)
		{
			//	番号
			getline(ifs, buf);
			if (buf.empty()) { break; }
			auto offset = buf.find(":");
			buf = buf.substr(0, offset);

			const int frameNo = stoi(buf);
			//	頂点が格納されているバッファに更新
			getline(ifs, buf);

			//	頂点
			while (true)
			{
				auto a = buf.find(t);
				if (a == string::npos) {
					break;
				}

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

					b = b.substr(d + 1);

					//	y
					auto f = b.find(" ");
					auto g = b.substr(0, f + 1);

					b = b.substr(f + 1);

					//	z
					tmp.position =
					{
						stof(e),
						stof(g),
						stof(b),
					};

					ret.vertices[animIndex].v[frameNo].push_back(tmp);

					//ret.vertices[animIndex].push_back(
					//	{
					//		frameNo,
					//	tmp
					//	}
					//);

				}

				buf = buf.substr(a + 1);
			}
		}


		//	フレーム番号
		//auto dummyPos = buf.find(":");
		//buf = buf.substr(0, dummyPos);

		//	頂点読み取り
		//getline(ifs, buf);
	}

	return ret;
}
