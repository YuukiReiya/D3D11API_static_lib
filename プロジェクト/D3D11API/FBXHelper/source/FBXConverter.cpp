#include "Common.h"
#include "FBXConverter.h"
#include "WinConsoleExpansion.h"
#include "IOMesh.h"
#include <fbxsdk.h>
#include <iostream>
#if defined DEBUG || defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <fstream>
#include <algorithm>
#include <Windows.h>
/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	usingディレクティブ
	@using	Converter
*/
using namespace Converter;

/*!
	@brief	usingディレクティブ
	@using	ConsoleExpansion
*/
using namespace ConsoleExpansion;

/*!
	@brief		別名定義
	@typedef	wic
	@value		ConsoleExpansion::WinConsoleExpansion
*/
typedef ConsoleExpansion::WinConsoleExpansion wic;

/*!
	@var	m_pManager
	@brief	FbxManagerのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxManager*>		FBXConverter::m_pManager	= nullptr;

/*!
	@var	m_pIOsetting
	@brief	FbxIOSettingsのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxIOSettings*>	FBXConverter::m_pIOsetting	= nullptr;

/*!
	@var	m_pScene
	@brief	FbxSceneのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxScene*>		FBXConverter::m_pScene		= nullptr;

/*!
	@var	m_pImporter
	@brief	FbxImporterのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxImporter*>	FBXConverter::m_pImporter	= nullptr;

/*!
	@brief	コンストラクタ
*/
Converter::FBXConverter::FBXConverter()
{
	Setup();
}

/*!
	@brief	デストラクタ
*/
Converter::FBXConverter::~FBXConverter()
{
	Teardown();
}

/*!
	@fn		Setup
	@brief	初期化
	@detail	メンバのインスタンス化と初期化
*/
void FBXConverter::Setup()
{
	//	メンバ破棄
	Teardown();

	m_pManager		= make_shared<fbxsdk::FbxManager*>();
	m_pScene		= make_shared<fbxsdk::FbxScene*>();
	m_pIOsetting	= make_shared<fbxsdk::FbxIOSettings*>();
	m_pImporter		= make_shared<fbxsdk::FbxImporter*>();

	*m_pManager		= fbxsdk::FbxManager::Create();
	*m_pIOsetting	= fbxsdk::FbxIOSettings::Create(*m_pManager.get(), IOSROOT);
	(*m_pManager)->SetIOSettings(*m_pIOsetting);
	*m_pScene		= fbxsdk::FbxScene::Create(*m_pManager.get(), NULL);
	*m_pImporter	= fbxsdk::FbxImporter::Create(*m_pManager.get(), NULL);

}

/*!
	@fn		Teardown
	@brief	破棄処理。
*/
void Converter::FBXConverter::Teardown()
{
	if (m_pImporter) {
		TeardownImporter();
	}
	if (m_pIOsetting) {
		(*m_pIOsetting.get())->Destroy();
		m_pIOsetting.reset();
	}
	if (m_pScene) {
		(*m_pScene.get())->Destroy();
		m_pScene.reset();
	}
	if (m_pManager) {
		(*m_pManager.get())->Destroy();
		m_pManager.reset();
	}
}

/*!
	@fn			Execute
	@brief		実行処理
	@detail		FBXを読み込み外部ファイルに書き出す。
	@param[in]	FBXのパス
	@param[in]	出力先のファイルパス
*/
void Converter::FBXConverter::Execute(std::string fbxPath, std::string outName)
{
	//	FBXSDKのセットアップ
	try
	{
		//	インポーター
		if (!SetupImporter(fbxPath)) { throw "Could not read \"" + fbxPath + "\""; }

		//	シーン
		if (!SetupScene(fbxPath)) { throw "Could not output \"" + fbxPath + "\" to the scene"; }

		//	インポーターの破棄
		TeardownImporter();

	}
	catch (std::string& error)
	{
		wic::SetColor(Red);
		cout << error << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);
	}

	//	三角化
	if (!Triangulate()) { 
		wic::SetColor(Red);
		cout << "Failed to triangulate this scene." << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);
	}

	//	メッシュの分割
	if (!SplitMeshesPerMaterial()) { 
		wic::SetColor(Red);
		cout << "Failed to split mesh" << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);

	}


	//	メッシュデータの出力
	auto meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();
	wic::SetColor(Green);
	cout << "Mesh count:";
	wic::SetColor(White);
	cout << meshCount << endl;
	for (int i = 0; i < meshCount; ++i)
	{
		auto pMesh = (*m_pScene.get())->GetSrcObject<FbxMesh>(i);
		cout << endl;
		wic::SetColor(Green);
		cout << "Mesh name:";
		wic::SetColor(White);
		cout << pMesh->GetName() << endl;

		//	構成ポリゴン数
		auto polygonCount = pMesh->GetPolygonCount();
		cout << "mesh polygon count = " << polygonCount << endl;

		//	構成しているポリゴンがなければ処理しない
		if (polygonCount <= 0) { continue; }

		//	出力データ
		Utility::Mesh oMesh;

		//	メッシュ情報を格納
		LoadToStore(pMesh, &oMesh);

		//	外部ファイル出力
		auto oName = meshCount == 1 ? outName : outName + "-" + to_string(i);
		Utility::IOMesh::Output(c_OutputDirectory.data(), oName, oMesh);
		//Utility::IOMesh::Output("", oName, oMesh);

		wic::SetColor(Green);
		cout << "CreateMesh success!" << endl;
		wic::SetColor(White);
	}
	cout << "Quit the program." << endl;
}

/*!
	@fn		Triangulate
	@brief	シーン内のモデルの三角ポリゴン化。
	@note	try catchをいれた単純なラッパー
	@return	true:成功 false:失敗
*/
bool Converter::FBXConverter::Triangulate()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

//	try
//	{
//		if (!converter.Triangulate(*m_pScene.get(), true)) {
//			throw runtime_error("Failed to triangulate this scene.");
//		}
//
//		//	メッシュから不要なポリゴンを削除する
//		converter.RemoveBadPolygonsFromMeshes(*m_pScene.get());
//	}
//	catch (exception&error)
//	{
//#pragma region 例外処理
//		wic::SetColor(Red);
//		std::cout << error.what() << endl;
//		wic::SetColor(White);
//#if defined DEBUG||_DEBUG
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//
//	return true;
	return converter.Triangulate(*m_pScene.get(), true);
}

/*!
	@fn		SplitMeshesPerMaterial
	@brief	シーン内のモデルをマテリアル単位に分割
	@note	try catchをいれた単純なラッパー
	@return	true:成功 false:失敗
*/
bool Converter::FBXConverter::SplitMeshesPerMaterial()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

//	try
//	{
//		if (converter.SplitMeshesPerMaterial(*m_pScene.get(), true)) {
//			throw runtime_error("Failed to split mesh");
//		}
//	}
//	catch (const std::exception& error)
//	{
//#pragma region 例外処理
//		wic::SetColor(Red);
//		std::cout << error.what() << endl;
//		wic::SetColor(White);
//#if defined DEBUG||_DEBUG
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//
//	}
//	return true;

	return converter.SplitMeshesPerMaterial(*m_pScene.get(), true);
}

/*!
	@fn			SetupImporter
	@brief		fbxファイルをインポーターに読み込む。
	@param[in]	読み込む.fbxファイルのパス
*/
bool Converter::FBXConverter::SetupImporter(std::string fbxPath)
{
//	try
//	{
//		if (!(*m_pImporter.get())->Initialize(
//			fbxPath.c_str(),
//			-1,
//			(*m_pManager.get())->GetIOSettings()
//		)) {
//			throw fbxPath;
//		}
//	}
//	catch (string&str)
//	{
//#pragma region 例外処理
//#if defined DEBUG||_DEBUG
//		std::cout << "Could not read \"" << str << "\"" << endl;
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//	return true;

	return (*m_pImporter.get())->Initialize(
		fbxPath.c_str(),
		-1,
		(*m_pManager.get())->GetIOSettings()
	);
}

/*!
	@fn		TeardownImporter
	@brief	インポーターの破棄処理。
*/
void Converter::FBXConverter::TeardownImporter()
{
	if (m_pImporter)
	{
		(*m_pImporter.get())->Destroy();
		m_pImporter.reset();
	}
}

/*!
	@fn			SetupScene
	@brief		インポーターに読み込んだデータをシーンにインポート。
	@detail		読み込み後にインポーターは破棄している。
	@param[in]	読み込む.fbxファイルのパス
*/
bool Converter::FBXConverter::SetupScene(std::string fbxPath)
{
//	try
//	{
//		if (!(*m_pImporter.get())->Import(
//			*m_pScene.get()
//		)) {
//			throw fbxPath;
//		}
//	}
//	catch (string&str)
//	{
//#pragma region 例外処理
//#if defined DEBUG||_DEBUG
//		std::cout << "Could not output \"" << str << "\" to the scene" << endl;
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//	return true;
	return (*m_pImporter.get())->Import(*m_pScene.get());
}

/*!
	@fn			LoadToStore
	@brief		fbxのデータを格納
	@param[in]	抽出元のデータ
	@param[in]	格納先メッシュ
*/
void Converter::FBXConverter::LoadToStore(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	try
	{
		//	頂点インデックス
		SetupVertexIndices(from, to);
		if (to->vertexIndices.empty())throw"SetupVertexIndices";

		//	頂点
		SetupVertices(from, to);
		if (to->vertices.empty())throw"SetupVertices";

		//	UV
		SetupUV(from, to);
		if (to->uv.empty())throw"SetupUV";

		//	頂点の整合化
		AlignVerticesToUV(to);
	}
	catch (const string&error)
	{
		wic::SetColor(Red);
		cout << "Failed to \"" << error << "\"" << endl;
		wic::SetColor(White);
	}
}

/*!
	@fn			SetupVertexIndices
	@brief		頂点インデックスのセットアップ
	@param[in]	参照元のConverterメッシュ
	@param[in]	バインド先のメッシュ
*/
void Converter::FBXConverter::SetupVertexIndices(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	ポリゴン数
	auto polygonCount = from->GetPolygonCount();
	for (int i = 0; i < polygonCount; ++i)
	{
		//	ポリゴンを構成する頂点数(※三角化していれば"3"になる)
		auto polygonVertexCount = from->GetPolygonSize(i);
		for (int j = 0; j < polygonVertexCount; ++j)
		{
			to->vertexIndices.push_back(
				{
					from->GetPolygonVertex(i,j)
				}
			);
		}
	}

	wic::SetColor(Green);
	cout << "Mesh vertex index size:";
	wic::SetColor(White);
	cout << to->vertexIndices.size() << endl;
}

/*!
	@fn			SetupVertices
	@brief		頂点のセットアップ
	@param[in]	参照元のfbxメッシュ
	@param[in]	バインド先のメッシュ
*/
void Converter::FBXConverter::SetupVertices(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	頂点数
	auto vertexCount = from->GetControlPointsCount();

	//	頂点配列の先頭ポインタ
	FbxVector4* vertices = from->GetControlPoints();

	int index = 0;
	while (index < vertexCount)
	{
		to->vertices.push_back(
			{
				{(float)vertices[index][0]},
				{(float)vertices[index][1]},
				{(float)vertices[index][2]},
				{(float)vertices[index][3]},
			}
		);
		++index;
	}
	wic::SetColor(Green);
	cout << "Mesh vertex size:";
	wic::SetColor(White);
	cout << to->vertices.size() << endl;
}

/*!
	@fn			SetupUV
	@brief		UVのセットアップ
	@detail		頂点インデックスに対応したUVが格納される。
	@param[in]	参照元のfbxメッシュ
	@param[in]	バインド先のメッシュ
*/
void Converter::FBXConverter::SetupUV(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	UVセット数
	FbxStringList uvSetNames;
	from->GetUVSetNames(uvSetNames);

	wic::SetColor(Green);
	cout << "UV set names count:";
	wic::SetColor(White);
	cout << uvSetNames.GetCount() << endl;
	for (int i = 0; i < uvSetNames.GetCount(); ++i)
	{
		wic::SetColor(Cyan);
		cout << "No" << i << ":";
		wic::SetColor(White);
		cout << uvSetNames.GetStringAt(i) << endl;
	}

	auto uvSetCount = uvSetNames.GetCount();
	int index = 0;
	while (index < uvSetCount)
	{
		//	UVSetNameを保存
		const string c_UVSetName = uvSetNames.GetStringAt(index);
		to->uvSetNamesList.push_back(c_UVSetName);

		wic::SetColor(Green);
		cout << endl << c_UVSetName << endl;
		wic::SetColor(White);

		FbxArray<FbxVector2>uvsets;
		from->GetPolygonVertexUVs(c_UVSetName.c_str(), uvsets);

		//	UVSetNameに対応したUVの格納
		for (int i = 0; i < uvsets.Size(); ++i)
		{
			to->uv[c_UVSetName].push_back(
				{
					c_UVSetName,
					{
								(float)uvsets[i][0],
						1.0f  - (float)uvsets[i][1],
					}
				}
			);
		}
		wic::SetColor(Cyan);
		cout << "UV set size:";
		wic::SetColor(White);
		cout << uvsets.Size() << endl;

		wic::SetColor(Cyan);
		cout << "UV count:";
		wic::SetColor(White);
		cout << to->uv[c_UVSetName].size() << endl;

		index++;
	}
}

/*!
	@fn			AlignVerticesToUV
	@brief		頂点をUVに揃える。
	@detail		事前に他の情報をそろえておく必要がある
	@param[in]	対象のメッシュ
*/
void Converter::FBXConverter::AlignVerticesToUV(Utility::Mesh * mesh)
{
	/*!
		@struct	Vertex
		@brief	頂点とUVを紐づけさせるための一時構造体
	*/
	struct Vertex
	{
		/*!
			@var	v
			@brief	頂点
		*/
		Math::FLOAT4 v;

		/*!
			@var	uv
			@brief	UV
		*/
		Math::FLOAT2 uv;

		static inline bool Compare(Vertex other, Vertex self) {
			return self.uv == other.uv&&self.v == other.v;
		}

		/*!
			@var	uvsetName
			@brief	関連付けるuvsetの名前
		*/
		string uvsetName;

	};

	//	インデックスでソートした重複のある全データ
	wic::SetColor(Green);
	cout << "Index sort vertices:";
	vector<Vertex>indexSortVertices;
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < mesh->vertexIndices.size(); ++i)
		{
			indexSortVertices.push_back(
				{
					{mesh->vertices[mesh->vertexIndices[i]]},
					{mesh->uv[hash][i].uv},
					{hash},
				}
			);
		}
	}
	wic::SetColor(White);
	cout << indexSortVertices.size() << endl;

	//	重複のない一意な頂点情報(頂点 + UV)
	vector<Vertex>uniqueVertices;
	wic::SetColor(Green);
	cout << "Unique vertices:";
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < mesh->vertexIndices.size(); i++)
		{

			//	uvset名は考慮しなくてもよいので未設定
			Vertex findItem =
			{
				{mesh->vertices[mesh->vertexIndices[i]]},
				{mesh->uv[hash][i].uv},
				{hash}
			};

			//	頂点とUV情報が一致するイテレータを取得
			auto it = find_if(
				uniqueVertices.begin(),
				uniqueVertices.end(),
				[&](Vertex&v){
				return Vertex::Compare(v,findItem);
			}
			);
			if (it != uniqueVertices.end()) { continue; }

			//	"頂点 + UV"の情報を保存 
			uniqueVertices.push_back(findItem);
		}
	}
	wic::SetColor(White);
	cout << uniqueVertices.size() << endl;

	//	メッシュ情報を再構成するため初期化
	mesh->vertexIndices.clear();
	mesh->vertices.clear();
	mesh->uv.clear();

	//	頂点情報(座標 + UV)格納
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < uniqueVertices.size(); i++)
		{
			//	i番目のインデックスが参照する頂点情報
			auto vertex = uniqueVertices[i];

			//	頂点
			mesh->vertices.push_back(vertex.v);

			//	uvset名に合わせて設定
			if (hash != vertex.uvsetName) { continue; }

			//	Vertex型をUV型にキャストして可変長配列に詰める
			Utility::Mesh::UV uv=
			{
				{vertex.uvsetName},
				{vertex.uv}
			};
			mesh->uv[hash].push_back(uv);
		}
	}

	//	インデックスの再構成
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < indexSortVertices.size(); ++i)
		{
			//	i番目の頂点情報
			Vertex findItem = indexSortVertices[i];

			try
			{
				//	探している頂点の情報が格納されているインデックスを取得
				auto it = find_if(
					uniqueVertices.begin(),
					uniqueVertices.end(),
					[&](Vertex&v) {
					return v.v == findItem.v&&v.uv == findItem.uv;
				}
				);

				if (it == uniqueVertices.end())throw runtime_error("could not find a unique vertex");
				
				//	イテレータをインデックス(配列の添え字)に変換
				auto index = distance(uniqueVertices.begin(), it);
				mesh->vertexIndices.push_back(index);
			}
			catch (exception&error)
			{
				wic::SetColor(Red);
				cout << "Error" << endl;
				cout << error.what() << endl;
				wic::SetColor(Purple);
				cout << "Failed to reconfigure index." << endl;
				wic::SetColor(Red);
				cout << "uv set name:";
				wic::SetColor(White);
				cout << hash << endl;
				wic::SetColor(Red);
				cout << "index number:";
				wic::SetColor(White);
				cout << i << endl;
				wic::SetColor(Purple);
				cout << "iterator not found";
				wic::SetColor(White);
				continue;
			}
		}
	}

	//	コンソール出力
	cout << endl;
	wic::SetColor(Yellow);
	cout << "mesh vertex size:";
	wic::SetColor(White);
	cout << mesh->vertices.size() << endl;
	wic::SetColor(Yellow);
	cout << "mesh index size:";
	wic::SetColor(White);
	cout << mesh->vertexIndices.size() << endl;
	wic::SetColor(Yellow);
	cout << "mesh uv size" << endl;
	const auto c_UVSetNamesCount = mesh->uvSetNamesList.size();

	for (size_t i = 0; i < c_UVSetNamesCount; i++)
	{
		auto it = mesh->uv[mesh->uvSetNamesList[i]][i];
		wic::SetColor(Yellow);
		cout << it.uvSetName << ":";
		wic::SetColor(White);
		cout << mesh->uv[mesh->uvSetNamesList[i]].size() << endl;
	}
}

