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

void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, FBX::Utility::SkeltonMesh mesh)
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

	//	1.頂点インデックス
	for (auto it : mesh.indices)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	2.UV
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.uv.x << c_Space << it.uv.y << "}";
	}
	ofs << endl;

	//	3.頂点
	for (auto it : mesh.vertices) 
	{
		ofs << "{" << it.position.x << c_Space << it.position.y << c_Space << it.position.z << "}";
	}
	ofs << endl;

	//	4."1頂点"が関連するボーン番号の最大数(一番関連ボーン数が多い頂点のボーン数)
	ofs << mesh.maxBonesElementsCount << endl;

	//	5.重み
	for (auto v : mesh.vertices)
	{
		ofs << "{" << c_Space;
		for (auto it : v.weight) {
			ofs << it.first << ":" << it.second << c_Space;
		}
		ofs << "}";

		//	書き出し
		//	{ x y z }{...になるはず
	}
	ofs << endl;

	//	6.初期姿勢
	for (auto it : mesh.initialMatrix)
	{
		ofs << "{"
			<< it.elemnts[0].x << c_Space << it.elemnts[0].y << c_Space << it.elemnts[0].z << c_Space << it.elemnts[0].w << c_Space
			<< it.elemnts[1].x << c_Space << it.elemnts[1].y << c_Space << it.elemnts[1].z << c_Space << it.elemnts[1].w << c_Space
			<< it.elemnts[2].x << c_Space << it.elemnts[2].y << c_Space << it.elemnts[2].z << c_Space << it.elemnts[2].w << c_Space
			<< it.elemnts[3].x << c_Space << it.elemnts[3].y << c_Space << it.elemnts[3].z << c_Space << it.elemnts[3].w << c_Space
			<< "}";
	}
	ofs << endl;

	//	7.フレーム数
	ofs << mesh.frameMatrix.size() << endl;//	フレーム

	//	8.フレーム時姿勢
	for (size_t frame = 0; frame < mesh.frameMatrix.size(); ++frame)
	{
		for (auto it:mesh.frameMatrix[frame]) {

			ofs << "{"
				<< it.elemnts[0].x << c_Space << it.elemnts[0].y << c_Space << it.elemnts[0].z << c_Space << it.elemnts[0].w << c_Space
				<< it.elemnts[1].x << c_Space << it.elemnts[1].y << c_Space << it.elemnts[1].z << c_Space << it.elemnts[1].w << c_Space
				<< it.elemnts[2].x << c_Space << it.elemnts[2].y << c_Space << it.elemnts[2].z << c_Space << it.elemnts[2].w << c_Space
				<< it.elemnts[3].x << c_Space << it.elemnts[3].y << c_Space << it.elemnts[3].z << c_Space << it.elemnts[3].w << "}";
		}
	}
	ofs << endl;

}
