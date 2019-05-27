#include "Common.h"
#include "FBXUtility.h"
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
	@using	FBX
*/
using namespace FBX;

/*!
	@var	m_pManager
	@brief	FbxManagerのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxManager*>		FBXUtility::m_pManager		= nullptr;

/*!
	@var	m_pIOsetting
	@brief	FbxIOSettingsのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxIOSettings*>	FBXUtility::m_pIOsetting	= nullptr;

/*!
	@var	m_pScene
	@brief	FbxSceneのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxScene*>		FBXUtility::m_pScene		= nullptr;

/*!
	@var	m_pImporter
	@brief	FbxImporterのポインタ
	@detail	スマートポインタでアドレス管理
*/
shared_ptr<fbxsdk::FbxImporter*>	FBXUtility::m_pImporter		= nullptr;

/*!
	@fn		Setup
	@brief	初期化
	@detail	メンバのインスタンス化と初期化
*/
void FBXUtility::Setup()
{
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

bool FBX::FBXUtility::Load(std::string path)
{
	//	fbx読み込み
	try
	{
		if(!(*m_pImporter.get())->Initialize(
			path.c_str(),
			-1,
			(*m_pManager.get())->GetIOSettings()
		)) {
			throw path;
		}
	}
	catch (string&str)
	{
		#pragma region 例外処理
#if defined DEBUG||_DEBUG
		std::cout << "Could not read \"" << str << "\"" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
		#pragma endregion
		return false;
	}

	//	シーンにインポート
	try
	{
		if (!(*m_pImporter.get())->Import(
			*m_pScene.get()
		)) {
			throw path;
		}
	}
	catch (string&str)
	{
#pragma region 例外処理
#if defined DEBUG||_DEBUG
		std::cout << "Could not output \"" << str << "\" to the scene" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}

	//	インポーターの破棄処理
	//(*m_pImporter.get())->Destroy();
	return true;
}

bool FBX::FBXUtility::Load(std::string path, Utility::Mesh * mesh)
{
#pragma region init

	try
	{
		//	インポーター
		if (!SetupImporter(path)) { throw "fbx importer"; }

		//	シーン
		if (!SetupScene(path)) { throw "fbx scene"; }

		//	三角化
		if (!Triangulate()) { throw "trianglate"; }

		//	インポーターの破棄
		TeardownImporter();

		//	メッシュ作成
		if (!CreateMesh(mesh)) { throw "create mesh"; }
	}
	catch (std::string&error)
	{
		cout << "Failed to setup \"" << error << "\"" << endl;
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);
	}
#pragma endregion
	//==================================================================
	//	読み込み
	//==================================================================

	return true;
}

void FBX::FBXUtility::Destroy()
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

bool FBX::FBXUtility::Triangulate()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

	try
	{
		if (!converter.Triangulate(*m_pScene.get(), true)) {
			throw;
		}

		//	メッシュから不要なポリゴンを削除する
		converter.RemoveBadPolygonsFromMeshes(*m_pScene.get());
	}
	catch (...)
	{
#pragma region 例外処理
#if defined DEBUG||_DEBUG
		std::cout << "Failed to triangulate this scene." << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}

	return true;
}

bool FBX::FBXUtility::SetupImporter(std::string fbxPath)
{
	try
	{
		if (!(*m_pImporter.get())->Initialize(
			fbxPath.c_str(),
			-1,
			(*m_pManager.get())->GetIOSettings()
		)) {
			throw fbxPath;
		}
	}
	catch (string&str)
	{
#pragma region 例外処理
#if defined DEBUG||_DEBUG
		std::cout << "Could not read \"" << str << "\"" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}
	return true;
}

void FBX::FBXUtility::TeardownImporter()
{
	if (m_pImporter)
	{
		(*m_pImporter.get())->Destroy();
		m_pImporter.reset();
	}
}

bool FBX::FBXUtility::SetupScene(std::string fbxPath)
{
	try
	{
		if (!(*m_pImporter.get())->Import(
			*m_pScene.get()
		)) {
			throw fbxPath;
		}
	}
	catch (string&str)
	{
#pragma region 例外処理
#if defined DEBUG||_DEBUG
		std::cout << "Could not output \"" << str << "\" to the scene" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}
	return true;
}

bool FBX::FBXUtility::CreateMesh(Utility::Mesh * mesh, bool isDebug)
{
	//	FBXを構成しているメッシュの数
	int meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();

	cout << "mesh count = " << meshCount << endl;

	try
	{
		for (int i = 0; i < meshCount; ++i)
		{
			cout << endl;

			FbxMesh* pMesh = (*m_pScene.get())->GetSrcObject<FbxMesh>(i);
			cout << "mesh name = " << pMesh->GetName() << endl;

			//	構成ポリゴン数
			auto polygonCount = pMesh->GetPolygonCount();
			cout << "mesh polygon count = " << polygonCount << endl;
			
			//	構成しているポリゴンがなければ処理しない
			if (polygonCount <= 0) { continue; }

			//	頂点生成
			SetupVertex(pMesh, mesh, isDebug);

			//	インデックス
			SetupIndex(pMesh, mesh, isDebug);

			//	UV
			SetupUV(pMesh, mesh, isDebug);

			//============================
			//TODO
			//============================
			Hoge(pMesh);
		}
	}
	catch (const string&error)
	{
		cout << "Failed to \"" << error << "\"" << endl;
		system("pause");
		return false;
	}
	cout << endl << endl;
	cout << "CreateMesh success!" << endl;
	cout << "Quit the program." << endl;

	return true;
}

void FBX::FBXUtility::SetupVertex(FbxMesh * mesh, Utility::Mesh * data, bool isShowValue)
{
	//	頂点数
	auto vertexCount = mesh->GetControlPointsCount();	

	//	頂点配列の先頭ポインタ
	FbxVector4* vertices = mesh->GetControlPoints();

	//	頂点インデックス
	int index = 0;
	while (true)
	{
		//	頂点分格納したら終了
		if (vertexCount <= index) { break; }

		Math::FLOAT4 vertex =
		{
			(float)vertices[index][0],
			(float)vertices[index][1],
			(float)vertices[index][2],
			(float)vertices[index][3],
		};
		index++;
		data->vertices.push_back(vertex);

		if (isShowValue)
		{
			cout << "x = " << vertex.x << ",y = " << vertex.y
				<< ",z = " << vertex.z << ",w = " << vertex.w << endl;
		}
	}

	//
	//	TODO:頂点の格納順がおかしい？
	//	頂点情報は順番に関係なくメッシュを構成するが、この時の格納順をもとにUVの順番を決めている
	//	http://shikemokuthinking.blogspot.com/ float posX = vertex[index[0]][0];←ココ

	//	ポリゴン数
	cout << "SetupVertex():Polygon count = " << mesh->GetPolygonCount() << endl;
	
	//	頂点数
	cout << "Vertex count = " << vertexCount << endl;

	//	実際に格納した頂点数
	cout << "Stored vertex count = " << data->vertices.size() << endl;

}

void FBX::FBXUtility::SetupIndex(FbxMesh * mesh, Utility::Mesh * data, bool isShowValue)
{
	//	ポリゴン数
	auto polygonCount = mesh->GetPolygonCount();

	//	既に格納されたインデックスの個数
	auto offset = data->index.size();

	for (int i = 0; i < polygonCount; ++i)
	{
		//	ポリゴンを構成する頂点数
		auto polygonVertexCount = mesh->GetPolygonSize(i);

		for (int j = 0; j < polygonVertexCount; ++j)
		{
			//	メッシュが複数の場合、インデックス番号 + オフセット
			auto index = mesh->GetPolygonVertex(i, j) + offset;
			data->index.push_back(index);
		}
	}

	//	ポリゴン数
	cout << "SetupIndex():Polygon count = " << mesh->GetPolygonCount() << endl;

	//	インデックス数
	cout << "index count = " << mesh->GetPolygonVertexCount() << endl;

	//	実際に格納した頂点数
	cout << "Stored index count = " << data->index.size() << endl;

}

void FBX::FBXUtility::SetupUV(FbxMesh * mesh, Utility::Mesh * data, bool isShowValue)
{
	//	レイヤー数
	auto layerCount = mesh->GetLayerCount();
	if (layerCount <= 0) { return; }

	cout << "layer count = " << layerCount << endl;
	
#pragma region sample1
	for (int i = 0; i < layerCount; ++i)
	{
		cout << "SetupIndex():layer No = " << i << endl;

		auto pLayer = mesh->GetLayer(i);
		auto uvElem = pLayer->GetUVs();
		auto uvCount = uvElem->GetDirectArray().GetCount();
		auto uvIndexCount = uvElem->GetIndexArray().GetCount();

		if (uvIndexCount < uvCount) {
			cout << endl;
			cout << "--------WARNING!!--------" << endl;
			cout << "uv count < uv index count" << endl;
			cout << "--------WARNING!!--------" << endl;
			cout << endl;
		}

		cout << "uv count = " << uvCount << endl;
		cout << "uv index count = " << uvIndexCount << endl;

		auto mapMode = uvElem->GetMappingMode();
		cout << "mapping mode = ";
		string cast;
		switch (mapMode)
		{
		case fbxsdk::FbxLayerElement::eNone:cast = "eNone"; break;
		case fbxsdk::FbxLayerElement::eByControlPoint:cast = "eByControlPoint"; break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:cast = "eByPolygonVertex"; break;
		case fbxsdk::FbxLayerElement::eByPolygon:cast = "eByPolygon"; break;
		case fbxsdk::FbxLayerElement::eByEdge:cast = "eByEdge"; break;
		case fbxsdk::FbxLayerElement::eAllSame:cast = "eAllSame"; break;
		default:cast = "invalid value"; break;
		}
		cout << cast << endl;

		if (mapMode != FbxLayerElement::eByPolygonVertex) { continue; }

		auto refMode = uvElem->GetReferenceMode();
		cout << "reference mode = ";
		switch (refMode)
		{
		case fbxsdk::FbxLayerElement::eDirect:cast = "eDirect"; break;
		case fbxsdk::FbxLayerElement::eIndex:cast = "eIndex"; break;
		case fbxsdk::FbxLayerElement::eIndexToDirect:cast = "eIndexToDirect"; break;
		default:cast = "invalid value"; break;
		}
		cout << cast << endl;

		FbxArray<FbxVector2> uvArray;
		uvElem->GetDirectArray().CopyTo(uvArray);

		switch (refMode)
		{
		case fbxsdk::FbxLayerElement::eDirect:
		{
			for (int j = 0; j < uvIndexCount; ++j)
			{
				Math::FLOAT2 uv=
				{
					static_cast<float>(uvArray.GetAt(j)[0]),
					static_cast<float>(uvArray.GetAt(j)[1]),
				};
				data->uv.push_back(uv);

				if (isShowValue)
				{
					cout << "u = " << uv.x << ",v = " << uv.y << endl;
				}
			}
		}
			break;
		case fbxsdk::FbxLayerElement::eIndex: break;
		case fbxsdk::FbxLayerElement::eIndexToDirect:
		{
			vector<Math::FLOAT2>uvAll;

			//TODO:インデックス⇒頂点数へ変換する処理
			auto offset = data->uv.size();
			for (int j = 0; j < uvIndexCount; ++j)
			{
				auto index = uvElem->GetIndexArray().GetAt(j);
				Math::FLOAT2 uv =
				{
					//static_cast<float>(uvArray.GetAt(index)[0]),
					//1.0f - static_cast<float>(uvArray.GetAt(index)[1]),

					//static_cast<float>(uvArray.GetAt(index)[0]),
					//1.0f - static_cast<float>(uvArray.GetAt(index)[1]),

					static_cast<float>(uvElem->GetDirectArray().GetAt(index)[0]),
					1.0f - static_cast<float>(uvElem->GetDirectArray().GetAt(index)[1]),

				};
				uvAll.push_back(uv);
				//data->uv.push_back(uv);

				if (isShowValue)
				{
					cout << "u = " << uv.x << ",v = " << uv.y << endl;
				}
			}
			
			int vertexCount = data->vertices.size();
			cout << "vertex count = " << vertexCount<<endl;

			//	格納したUVからメッシュを構成するUV情報をmeshのメンバに！
			for (int j = 0; j < vertexCount; ++j)
			{
				Math::FLOAT2 v = uvAll[uvElem->GetIndexArray()[j]];
				data->uv.push_back(v);
			}
		}
			break;
		default:
			break;
		}


		//	実際に格納したuv
		cout << "Stored uv vertex count = " << data->uv.size() << endl;

	}
#pragma endregion

#pragma region sample2
//FbxStringList uvSetNames;
//mesh->GetUVSetNames(uvSetNames);
//
//auto uvSetCount = uvSetNames.GetCount();
//cout << "uv set name count = " << uvSetCount << endl;
//
//if (uvSetCount <= 0)
//{
//	cout << endl << "Error" << endl;
//	return;
//}
//
//cout << "uv set name = " << uvSetNames.GetStringAt(0) << endl;
//
//FbxArray<FbxVector2>uvsets;
//mesh->GetPolygonVertexUVs(uvSetNames.GetStringAt(0), uvsets);
//
//cout << "uv set vertex count = " << uvsets.Size() << endl;
//
//for (int i = 0; i < uvsets.Size(); ++i)
//{
//	const FbxVector2&uv = uvsets[i];
//	Math::FLOAT2 set =
//	{
//		uv[0],
//		uv[1],
//	};
//	data->uv.push_back(set);
//}
//cout << "stored uv count = " << data->uv.size() << endl;
#pragma endregion

}

void FBX::FBXUtility::Hoge(fbxsdk::FbxMesh * mesh)
{
	//	色
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h,
		FOREGROUND_GREEN);

	FbxStringList uvNameList;
	mesh->GetUVSetNames(uvNameList);
	auto uvsetCount = uvNameList.GetCount();
	cout << endl << "count = " << uvsetCount << endl;
	for (int i = 0; i < uvsetCount; ++i)
	{
		cout << i << ":" << uvNameList.GetStringAt(i) << endl;
	}

	//	色
	SetConsoleTextAttribute(h,
		FOREGROUND_RED);

	cout << endl << endl << "HOGE" << endl;

	auto uvLayerCount = mesh->GetElementUVCount();

	vector<string>uvSetNames;
	for (int i = 0; i < uvLayerCount; ++i)
	{

		auto uv = mesh->GetElementUV(i);
		auto mapMode = uv->GetMappingMode();
		cout << "mapping mode = ";
		string cast;
		switch (mapMode)
		{
		case fbxsdk::FbxLayerElement::eNone:cast = "eNone"; break;
		case fbxsdk::FbxLayerElement::eByControlPoint:cast = "eByControlPoint"; break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:cast = "eByPolygonVertex"; break;
		case fbxsdk::FbxLayerElement::eByPolygon:cast = "eByPolygon"; break;
		case fbxsdk::FbxLayerElement::eByEdge:cast = "eByEdge"; break;
		case fbxsdk::FbxLayerElement::eAllSame:cast = "eAllSame"; break;
		default:cast = "invalid value"; break;
		}
		cout << cast << endl;

		if (mapMode != FbxLayerElement::eByPolygonVertex) { continue; }

		auto refMode = uv->GetReferenceMode();
		cout << "reference mode = ";
		switch (refMode)
		{
		case fbxsdk::FbxLayerElement::eDirect:cast = "eDirect"; break;
		case fbxsdk::FbxLayerElement::eIndex:cast = "eIndex"; break;
		case fbxsdk::FbxLayerElement::eIndexToDirect:cast = "eIndexToDirect"; break;
		default:cast = "invalid value"; break;
		}
		cout << cast << endl;

		//	マッピングモード判別
		if (mapMode != FbxGeometryElement::eByPolygonVertex) { continue; }

		//	リファレンスモード判別
		if (refMode != FbxGeometryElement::eIndexToDirect) { continue; }

		auto uvIndex = &uv->GetIndexArray();
		auto uvIndexCount = uvIndex->GetCount();

		vector<Math::FLOAT2>uvBuffer;

		for (int i = 0; i < uvIndexCount; ++i)
		{
			int index = uvIndex->GetAt(i);
			Math::FLOAT2 temp =
			{
				(float)uv->GetDirectArray().GetAt(index)[0],
				1.0f - (float)uv->GetDirectArray().GetAt(index)[1],
			};
			uvBuffer.push_back(temp);
		}

		//	UVSet名
		auto name = uv->GetName();
		uvSetNames.push_back(name);
	}


	//-----------------------------------------------
	//マテリアル
	auto node = mesh->GetNode();

	auto matCount = node->GetMaterialCount();
	cout << "material count = " << matCount << endl;

	vector<string>vTexFilePathes;
	vector<string>matSetNames;
	for (int i = 0; i < matCount; ++i)
	{
		auto mat = node->GetMaterial(i);

		//	ディフューズ
		auto prop = mat->FindProperty(FbxSurfaceMaterial::sDiffuse);

		auto layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

		if (0 < layeredTextureCount)
		{
			//cout << "こっちは入らないよ!!" << endl;
			//system("pause");
			//exit(NULL);

			for (int j = 0; j < layeredTextureCount; ++j)
			{
				auto layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
				auto texCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

				for (int k = 0; k < texCount; ++k)
				{
					auto tex = prop.GetSrcObject<FbxFileTexture>(k);

					if (!tex) { continue; }

					auto texPath = tex->GetRelativeFileName();
					vTexFilePathes.push_back(texPath);
				}
			}

		}
		else
		{
			auto fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				for (int j = 0; j < fileTextureCount; ++j)
				{
					auto tex = prop.GetSrcObject<FbxFileTexture>(j);
					if (!tex) { continue; }

					auto texPath = tex->GetRelativeFileName();
					vTexFilePathes.push_back(texPath);

					auto name = tex->UVSet.Get().Buffer();
					matSetNames.push_back(name);
				}
			}
			else
			{
				cout << "ERROR" << endl;
				system("pause");
				//exit(NULL);
			}
		}
	}

	auto a = mesh->GetName();
	cout << "Mesh Name = " << a << endl;
	cout << "uv set name " << endl;
	int c = 0;
	for (auto it : uvSetNames)
	{
		c++;
		cout << c << ":" << it << endl;
	}
	c = 0;
	cout << "mat set name " << endl;
	for (auto it : matSetNames)
	{
		c++;
		cout << c << ":" << it << endl;
	}
	c = 0;
	cout << "uv path " << endl;
	for (auto it : vTexFilePathes)
	{
		c++;
		cout << c << ":" << it << endl;
	}
	cout << "=============================================" << endl;
	cout << "// End of Hoge" << endl;
	cout << "=============================================" << endl;
}

