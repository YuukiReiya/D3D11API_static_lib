/*
	@file	IOMesh.cpp
	@date	2019/05/30
	@author	番場 宥輝
	@brief	メッシュデータのの入出力関係
	@detail	シングルトン
*/
#include "Common.h"
#include "IOMesh.h"
#include "WinConsoleExpansion.h"
#include <filesystem>
/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	usingディレクティブ
	@using	ConsoleExpansion
*/
using namespace ConsoleExpansion;

/*!
	@brief	エイリアス宣言
	@using	ConsoleExpansion::WinConsoleExpansion
	@var	wic
*/
using wic = ConsoleExpansion::WinConsoleExpansion;

/*!
	@fn			Delete
	@brief		yfmファイル削除
	@param[in]	ファイルのディレクトリ
	@param[in]	ファイル名(※拡張子除く)
*/
void Utility::IOMesh::Delete(std::string directoryPath, std::string fileName)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ifstream ifs;

	//	ファイルを開いてみる
	ifs.open(path);

	//	開くのに失敗
	if (ifs.fail()) {
		cout << "\""<<path<<"\" is open failed!" << endl;
		ifs.close();
		return; 
	}

	//	ファイルを忘れずに閉じておく
	ifs.close();

	//	該当ファイル削除
	remove(path.c_str());
	cout << "\"" << path << "\" is deleted!" << endl;
}

/*!
	@fn			Output
	@brief		独自形式の出力
	@param[in]	ファイルのディレクトリ
	@param[in]	ファイル名(※拡張子除く)
	@param[in]	メッシュ情報
*/
void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Utility::Mesh mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	
	try
	{
		if (!filesystem::exists(directoryPath)) {
			wic::SetColor(Red);
			cout << "Directory not found" << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directoryPath))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
	}
	catch (exception& error)
	{
		wic::SetColor(Red);
		cout << error.what() << endl;
		cout << "This program has failed." << endl;
		cout << "this program exit here!" << endl;
		wic::SetColor(White);
		system("pause");
		exit(NULL);
	}
	
	ofstream ofs;
	ofs.open(path, ios::out);

	//	書き込み
	const string c_Space = " ";

	//	頂点インデックス
	//ofs << "//i" << endl;
	for (auto it : mesh.vertexIndices)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	uv
	//ofs << "//uv" << endl;
	for (auto it : mesh.uv)
	{
		for (auto item : it.second)
			ofs << "{" << item.uv.x << c_Space << item.uv.y << "}";// << endl;
	}
	ofs << endl;

	//頂点
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
}

void Utility::IOMesh::Write(std::vector<DirectX::XMMATRIX> mat)
{
	ofstream ofs;
	ofs.open("フレーム時合成行列.md", ios::out);

	for (auto it : mat)
	{
		ofs << "{" <<
			it.r[0].m128_f32[0] << "," << it.r[0].m128_f32[1] << "," << it.r[0].m128_f32[2] << "," << it.r[0].m128_f32[3] << "," <<
			it.r[1].m128_f32[0] << "," << it.r[1].m128_f32[1] << "," << it.r[1].m128_f32[2] << "," << it.r[1].m128_f32[3] << "," <<
			it.r[2].m128_f32[0] << "," << it.r[2].m128_f32[1] << "," << it.r[2].m128_f32[2] << "," << it.r[2].m128_f32[3] << "," <<
			it.r[3].m128_f32[0] << "," << it.r[3].m128_f32[1] << "," << it.r[3].m128_f32[2] << "," << it.r[3].m128_f32[3] 
			<< "},";
	}
}

void Utility::IOMesh::WriteJ(std::vector<std::vector<Joint>> js)
{
	ofstream ofs;
	ofs.open("ジョイントW&M.md", ios::out);
	for (size_t i = 0; i < js.size(); i++)
	{
		ofs << "{";
		for (size_t j = 0; j < js[i].size(); j++)
		{
			auto joint = js[i][j];
			ofs << "{" << joint.index << "," << joint.weight << "},";
		}
		ofs << "},";
	}
}

void Utility::IOMesh::WriteM(std::vector<std::vector<DirectX::XMMATRIX>> mats)
{
	ofstream ofs;
	ofs.open("フレーム時スキン行列.md", ios::out);
	for (auto fm : mats)
	{
		ofs << "{";
		for (auto it : fm)
		{
			ofs << "{" <<
				it.r[0].m128_f32[0] << "," << it.r[0].m128_f32[1] << "," << it.r[0].m128_f32[2] << "," << it.r[0].m128_f32[3] << "," <<
				it.r[1].m128_f32[0] << "," << it.r[1].m128_f32[1] << "," << it.r[1].m128_f32[2] << "," << it.r[1].m128_f32[3] << "," <<
				it.r[2].m128_f32[0] << "," << it.r[2].m128_f32[1] << "," << it.r[2].m128_f32[2] << "," << it.r[2].m128_f32[3] << "," <<
				it.r[3].m128_f32[0] << "," << it.r[3].m128_f32[1] << "," << it.r[3].m128_f32[2] << "," << it.r[3].m128_f32[3]
				<< "},";
		}
		ofs << "},";
	}
}
