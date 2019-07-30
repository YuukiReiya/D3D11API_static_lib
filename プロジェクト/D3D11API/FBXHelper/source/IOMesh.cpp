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

void Utility::IOMesh::Output(std::string fileName, SkeletonMesh mesh,AnimationClip clip)
{
	string path = fileName + c_Delimiter.data() + c_Extension.data();

	ofstream ofs;
	ofs.open(path, ios::out);

	//	書き込み
	const string c_Space = " ";

	//	頂点インデックス
	for (auto it : mesh.indices)
	{
		//ofs << it << c_Space;
		ofs << it << ",";
	}
	ofs << endl;

	//	uv
	for (auto it : mesh.vertices)
	{
	//		ofs << "{" << it.uv.x << c_Space << it.uv.y << "}";// << endl;
	}
	ofs << endl;

	//頂点
	for (auto it : mesh.vertices)
	{
		//ofs << "{" << it.position.x << c_Space << it.position.y << c_Space << it.position.z << "}";// << endl;
		ofs << "{" << it.position.x << "," << it.position.y << "," << it.position.z << "},";// << endl;
	}
	ofs << endl;

	//	重み
	//for (auto it : mesh.vertices) 
	//{
	//	ofs << "{ "
	//		<< static_cast<int>(it.joijntIndex[0]) << ":" << it.jointWeights[0] << c_Space 
	//		<< static_cast<int>(it.joijntIndex[1]) << ":" << it.jointWeights[1] << c_Space
	//		<< static_cast<int>(it.joijntIndex[2]) << ":" << it.jointWeights[2] << c_Space
	//		<< "}";
	//}
	for (auto v : mesh.vertices)
	{
		//ofs << "{" << c_Space;
		//for (size_t i = 0; i < v.jointsIndex.size(); i++)
		//{
		//	ofs << static_cast<int>(v.jointsIndex[i]) << ":" << v.jointsWeight[i] << c_Space;
		//}
		//ofs << "}";
		

		//	書き出し
		//	{ x y z }{...になるはず
	}
	ofs << endl;

	//	初期姿勢(逆行列)
	//for (size_t i = 0; i < mesh.skeleton.jointCount; i++)
	for (size_t i = 0; i < mesh.skeleton.joints.size(); i++)
	{
		auto& m = mesh.skeleton.joints[i].invBindPose;
		ofs << "{" <<
			m._11 << c_Space << m._12 << c_Space << m._13 << c_Space <<
			m._21 << c_Space << m._22 << c_Space << m._23 << c_Space <<
			m._31 << c_Space << m._32 << c_Space << m._33 << c_Space <<
			m._41 << c_Space << m._42 << c_Space << m._43 << c_Space <<
			"}";
	}

	//	アニメフレーム
	ofs << endl;
	ofs << clip.frameCount << endl;

	//	フレーム行列
	//※1アニメ

	for (size_t i = 0; i < clip.bonesMatrix.size(); i++)
	{
		//auto frameCount = clip.frameCount;
		//for (size_t j = 0; j < frameCount; j++)

		{
			//auto it = clip.bonesMatrix[i][j];
			//
			//ofs << "{"
			//	<< it.r[0].m128_f32[0] << c_Space << it.r[0].m128_f32[1] << c_Space << it.r[0].m128_f32[2] << c_Space << it.r[0].m128_f32[3] << c_Space
			//	<< it.r[1].m128_f32[0] << c_Space << it.r[1].m128_f32[1] << c_Space << it.r[1].m128_f32[2] << c_Space << it.r[1].m128_f32[3] << c_Space
			//	<< it.r[2].m128_f32[0] << c_Space << it.r[2].m128_f32[1] << c_Space << it.r[2].m128_f32[2] << c_Space << it.r[2].m128_f32[3] << c_Space
			//	<< it.r[3].m128_f32[0] << c_Space << it.r[3].m128_f32[1] << c_Space << it.r[3].m128_f32[2] << c_Space << it.r[3].m128_f32[3] << 
			//	"}";
		}
	}
}
