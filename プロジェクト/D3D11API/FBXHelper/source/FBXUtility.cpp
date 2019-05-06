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

bool FBX::FBXUtility::Load(std::string path, Abstract::AbstractMesh * mesh)
{	
	//	fbx読み込み
	try
	{
		if (!(*m_pImporter.get())->Initialize(
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


	auto fbxMesh = (*m_pScene.get())->GetSrcObject<fbxsdk::FbxMesh>(0);

	if (!fbxMesh) {
		cout << endl << "メッシュ情報がねーよ" << endl;
		return false;
	}

	//	頂点
	SetupVertex(fbxMesh, mesh);

	//	インデックス
	SetupIndexBuffer(fbxMesh, mesh);

	//	法線
	//SetupNormal(fbxMesh, mesh);

	return true;
}

void FBX::FBXUtility::SetupVertex(fbxsdk::FbxMesh * fbxMesh, Abstract::AbstractMesh * mesh)
{
	auto v = fbxMesh->GetControlPoints();
	mesh->vertex.resize(fbxMesh->GetControlPointsCount());
	int i = 0;
	while (i < fbxMesh->GetControlPointsCount())
	{
		//TODO:vector<Vertex>型のインスタンスを宣言し、push_backすることでresizeを使わなくて良くなる
		mesh->vertex[i].x = v[i][0];
		mesh->vertex[i].y = v[i][1];
		mesh->vertex[i].z = v[i][2];
		mesh->vertex[i].w = v[i][3];
		i++;
	}
}

void FBX::FBXUtility::SetupIndexBuffer(fbxsdk::FbxMesh * fbxMesh, Abstract::AbstractMesh * mesh)
{
	int index = 0;
	auto vertices= fbxMesh->GetPolygonVertices();
	while (index < fbxMesh->GetPolygonVertexCount())
	{
		mesh->indexBuffer.push_back(vertices[index++]);
	}
}

void FBX::FBXUtility::SetupNormal(fbxsdk::FbxMesh * fbxMesh, Abstract::AbstractMesh * mesh)
{
	auto layer = fbxMesh->GetLayer(0);
	auto normal = layer->GetNormals();

	bool isMapMode = normal->GetMappingMode() == FbxLayerElement::eByPolygonVertex || normal->GetMappingMode() == FbxLayerElement::eByControlPoint;
	bool isRefMode = normal->GetReferenceMode() == FbxLayerElement::eDirect;
	bool isGet = isMapMode && isRefMode;

	if (!isGet) { return; }

	int index = 0;
	while (index < normal->GetDirectArray().GetCount())
	{
		mesh->vertex[index].normal.x = normal->GetDirectArray().GetAt(index)[0];
		mesh->vertex[index].normal.y = normal->GetDirectArray().GetAt(index)[1];
		mesh->vertex[index].normal.z = normal->GetDirectArray().GetAt(index)[2];
		index++;
	}
}

void FBX::FBXUtility::Destroy()
{
	if (m_pImporter) {
		(*m_pImporter.get())->Destroy();
		m_pImporter.reset();
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
			throw - 1;
		}
	}
	catch (int&)
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
