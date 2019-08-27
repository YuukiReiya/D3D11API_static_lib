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
	@var	c_Comma
	@brief	コンマ
*/
static constexpr string_view c_Comma = ",";

/*!
	@var	c_Space
	@brief	スペース
*/
static constexpr string_view c_Space = " ";

/*!
	@var	c_Colon
	@brief	コロン
*/
static constexpr string_view c_Colon = ":";

/*!
	@var	c_Slash
	@brief	スラッシュ
*/
static constexpr string_view c_Slash = "/";

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
	@fn			OutputMesh
	@brief		独自形式のスキンメッシュを出力
	@param[in]	出力ファイルパス
	@param[in]	頂点インデックス
	@param[in]	頂点情報
*/
void Utility::IOMesh::OutputMesh(std::string filePath, std::vector<uint32_t> indices, std::vector<D3D11::Graphic::SkinnedVertex> vertices)
{
	string fileName = filePath;
	string buf = "";

	//	ディレクトリ生成
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_Extension.data(), ios::out);

	//	頂点インデックス
	for (auto it : indices) { ofs << it << c_Comma; }
	ofs << endl;

	//	UV
	for (auto it : vertices) { ofs << it.uv.x << c_Comma << it.uv.y << c_Space; }
	ofs << endl;

	//	頂点座標
	for (auto it : vertices) { ofs << it.position.x << c_Comma << it.position.y << c_Comma << it.position.z << c_Space; }
	ofs << endl;

	//	重み
	for (auto v : vertices) {
		for (auto joint : v.joints)
		{
			ofs << joint.index << c_Colon << joint.weight << c_Comma;
		}
		ofs << endl;
	}
}

/*!
	@fn			OutputAnimation
	@brief		独自形式のアニメーションを出力
	@param[in]	出力ファイルパス
	@param[in]	アニメーションクリップ
*/
void Utility::IOMesh::OutputAnimation(std::string filePath, API::AnimationClip clips)
{
	string fileName = filePath;
	string buf = "";
	//	ディレクトリ生成
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_AnimExtension.data(), ios::out);

	for (auto joint : clips.matrixPalette)
	{
		for (auto matrix : joint)
		{
			ofs <<
				matrix.r[0].m128_f32[0] << c_Comma << matrix.r[0].m128_f32[1] << c_Comma << matrix.r[0].m128_f32[2] << c_Comma << matrix.r[0].m128_f32[3] << c_Comma <<
				matrix.r[1].m128_f32[0] << c_Comma << matrix.r[1].m128_f32[1] << c_Comma << matrix.r[1].m128_f32[2] << c_Comma << matrix.r[1].m128_f32[3] << c_Comma <<
				matrix.r[2].m128_f32[0] << c_Comma << matrix.r[2].m128_f32[1] << c_Comma << matrix.r[2].m128_f32[2] << c_Comma << matrix.r[2].m128_f32[3] << c_Comma <<
				matrix.r[3].m128_f32[0] << c_Comma << matrix.r[3].m128_f32[1] << c_Comma << matrix.r[3].m128_f32[2] << c_Comma << matrix.r[3].m128_f32[3] << c_Space;
		}
		ofs << endl;
	}
}

/*!
	@fn			OutputTextureName
	@brief		使用されているテクスチャ名を.iniファイルとして書き出す
	@param[in]	出力ファイルパス
	@param[in]	使用されているテクスチャの可変長配列
*/
void Utility::IOMesh::OutputTextureName(std::string filePath, std::vector<std::string> textureNames)
{
	string fileName = filePath;
	string buf = "";
	//	ディレクトリ生成
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_IniExtension.data(), ios::out);

	ofs << "メッシュに使用されているテクスチャ名" << endl;
	for (auto it : textureNames)
	{
		ofs << it << endl;
	}
}