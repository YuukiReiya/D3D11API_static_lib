/*
	@file	IOMesh.cpp
	@date	2019/05/30
	@author	番場 宥輝
	@brief	メッシュデータのの入出力関係
	@detail	シングルトン
*/
#include "Common.h"
#include "IOMesh.h"

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

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
	ofstream ofs;
	ofs.open(path, ios::out);

	//	書き込み

	//頂点
	const string c_Space = " ";
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
	ofs << endl;

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
		ofs << "{" << it.x << c_Space << it.y << "}";// << endl;
	}

}