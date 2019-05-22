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

#include "Vertex.h"
#include "Mesh.h"
#include "IOMesh.h"

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


//	生宣言
fbxsdk::FbxManager* FBXUtility::pManager;
fbxsdk::FbxScene* FBXUtility::pScene;
fbxsdk::FbxImporter* FBXUtility::pImporter;
fbxsdk::FbxIOSettings*FBXUtility::pSettings;


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

	//	生
	pManager = FbxManager::Create();
	pSettings = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(pSettings);
	pScene = FbxScene::Create(pManager, NULL);
	pImporter = FbxImporter::Create(pManager, NULL);

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

vector<string>g_vUVSetName;
vector<string>g_vTexPath;
bool FBX::FBXUtility::Load(std::string path, Abstract::AbstractMesh * mesh)
{
	//	fbx読み込み
//	try
//	{
//
//		if (!(*m_pImporter.get())->Initialize(
//			path.c_str(),
//			-1,
//			(*m_pManager.get())->GetIOSettings()
//		)) {
//			throw path;
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
//
//	//	シーンにインポート
//	try
//	{
//		if (!(*m_pImporter.get())->Import(
//			*m_pScene.get()
//		)) {
//			throw path;
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
//
//	ifstream is(path);
//	if (is.fail()) {
//		cout << endl << "そもそもメッシュがねーよ" << endl;
//	}
//
//	auto fbxMesh = (*m_pScene.get())->GetSrcObject<fbxsdk::FbxMesh>(0);
//	if (!fbxMesh) {
//		cout << endl << "メッシュ情報がねーよ" << endl;
//		return false;
//	}

	//	頂点
	//SetupVertex(fbxMesh, mesh);

	//	インデックス
	//SetupIndexBuffer(fbxMesh, mesh);

	//	法線
	//SetupNormal(fbxMesh, mesh);

	//	UV
	//SetupUV(fbxMesh, mesh);

	//参考:http://shikemokuthinking.blogspot.com/2013/08/fbx.html

	//	UVセット
	//{
	//	auto uvSetCount = fbxMesh->GetElementUVCount();
	//	for (int i = 0; i < uvSetCount; ++i)
	//	{
	//		//	buffer
	//		auto buf = fbxMesh->GetElementUV(i);

	//		//	mapping
	//		auto mapMode = buf->GetMappingMode();

	//		//	ref
	//		auto refMode = buf->GetReferenceMode();

	//		//	uv num
	//		auto uvCount = buf->GetDirectArray().GetCount();

	//		//	mapping switch
	//		switch (mapMode)
	//		{
	//		case fbxsdk::FbxLayerElement::eNone:break;
	//		case fbxsdk::FbxLayerElement::eByControlPoint:break;
	//		case fbxsdk::FbxLayerElement::eByPolygonVertex:
	//		{
	//			switch (refMode)
	//			{
	//			case fbxsdk::FbxLayerElement::eDirect:break;
	//			case fbxsdk::FbxLayerElement::eIndex:break;
	//			case fbxsdk::FbxLayerElement::eIndexToDirect:
	//			{
	//				auto uvIndex = &buf->GetIndexArray();
	//				auto uvIndexCount = uvIndex->GetCount();

	//				//	uv
	//				Abstract::AbstractMesh::UV temp;
	//				for (int j = 0; j < uvIndexCount; ++j)
	//				{
	//					temp.u = (float)buf->GetDirectArray().GetAt(uvIndex->GetAt(j))[0];

	//					//NOTE:BlenerのUVの原点位置は(0,0)が左下
	//					//※DirectXのUV原点位置は左上(0,0)。↓ではそのコンバートを行っている
	//					temp.v = 1.0f - (float)buf->GetDirectArray().GetAt(uvIndex->GetAt(j))[1];
	//					mesh->uv.push_back(temp);
	//				}

	//				//	uv set name
	//				auto name = buf->GetName();
	//				cout << "name:" << name << endl;
	//				g_vUVSetName.push_back(name);
	//			}
	//				break;
	//			default:
	//				break;
	//			}
	//		}

	//			break;
	//		case fbxsdk::FbxLayerElement::eByPolygon:break;
	//		case fbxsdk::FbxLayerElement::eByEdge:break;
	//		case fbxsdk::FbxLayerElement::eAllSame:break;
	//		default:
	//			break;
	//		}
	//	}
	//}

#pragma region Material
	//	マテリアル
	//{
	//	auto node = fbxMesh->GetNode();
	//	auto matCount = node->GetMaterialCount();

	//	for (int i = 0; i < matCount; ++i)
	//	{
	//		//	マテリアル
	//		auto mat = node->GetMaterial(i);

	//		//	プロパティ
	//		auto pro = mat->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//		//	テクスチャ数
	//		auto layerdTexCount = pro.GetSrcObjectCount();

	//		if (0 < layerdTexCount) {
	//			for (int j = 0; j < layerdTexCount; ++j)
	//			{
	//				//	レイヤードテクスチャ取得
	//				auto layeredTex = pro.GetSrcObject<FbxLayeredTexture>(j);

	//				//	レイヤードテクスチャNULL判定
	//				if (!layeredTex) { continue; }

	//				//	レイヤー数
	//				auto texCount = layeredTex->GetSrcObjectCount<FbxFileTexture>();

	//				for (int k = 0; k < texCount; ++k)
	//				{
	//					auto tex = pro.GetSrcObject<FbxFileTexture>(k);

	//					if (!tex)
	//					{
	//						cout << "テクスチャ取得失敗" << endl;
	//						continue;
	//					}

	//					//	tex name
	//					//auto name = tex->GetName();
	//					auto name = tex->GetRelativeFileName();
	//					g_vTexPath.push_back(name);
	//					//	uv set name
	//					auto uvsetName = tex->UVSet.Get().Buffer();
	//					g_vUVSetName.push_back(uvsetName);
	//					//	
	//				}
	//			}
	//		}
	//		else {
	//			//	テクスチャ取得
	//			auto fTexCount = pro.GetSrcObjectCount<FbxFileTexture>();

	//			if (0 < fTexCount) 
	//			{
	//				for (int j = 0; j < fTexCount; ++j)
	//				{
	//					auto tex = pro.GetSrcObject<FbxFileTexture>();

	//					if (!tex)
	//					{
	//						cout << "テクスチャ取得失敗" << endl;
	//						continue;
	//					}

	//					//	tex name 
	//					//auto texName = tex->GetName();
	//					auto texName = tex->GetRelativeFileName();
	//					g_vTexPath.push_back(texName);
	//					//	uv set name
	//					auto uvsetName = tex->UVSet.Get().Buffer();
	//					g_vUVSetName.push_back(uvsetName);
	//				}
	//			}
	//		}
	//	}
	//}
#pragma endregion

	//cout << "texpath" << endl;
	//for (auto it : g_vTexPath)
	//{
	//	cout << it << endl;
	//}
	//cout <<endl<< "uvsetname" << endl;
	//for (auto it : g_vUVSetName)
	//{
	//	cout << it << endl;
	//}

#pragma region init

	pImporter = FbxImporter::Create(pManager, "");
	pManager->SetIOSettings(pSettings);
	int fileformat = -1;

	//	インポーター
	if (!pImporter->Initialize(path.c_str(), fileformat, pManager->GetIOSettings()))
	{
		cout << "Importer false;";
		return false;
	}

	//	シーン
	FbxScene*scene = FbxScene::Create(pManager, path.c_str());
	pImporter->Import(scene);
	pImporter->Destroy();

	//	三角化
	FbxGeometryConverter cv(pManager);
	cv.Triangulate(scene, true);
	cv.RemoveBadPolygonsFromMeshes(scene);

	//==================================================================
	//	読み込み
	//==================================================================

	int meshCount = scene->GetSrcObjectCount<fbxsdk::FbxMesh>();
	cout << "mesh count = " << meshCount << endl << endl;

	//	書き出すメッシュ情報
	Utility::OutputMesh output;

	//	
	for (int i = 0; i < meshCount; ++i)
	{
		FbxMesh*pMesh = scene->GetSrcObject<FbxMesh>(i);

		cout << "mesh name =" << pMesh->GetName() << endl;

		//	polygon group
		{
			int matCount = pMesh->GetNode()->GetMaterialCount();

			cout << "matrial count = " << matCount << endl;

			if (matCount > 0)
			{

				//	コントロール点
				int controlPointsCount = pMesh->GetControlPointsCount();
				cout << "control point count = " << controlPointsCount << endl;

				//	ポリゴンインデックス
				vector<int>polygonIndex;

				//	vertex
				{
					//	ポリゴン数
					int polygonCount = pMesh->GetPolygonCount();

					cout << "polygon count = " << polygonCount << endl;

					if (polygonCount <= 0) { continue; }

					//	頂点座標
					//vector<FLOAT4>positions;

					//	UV
					//vector<FLOAT2>UVs;

					//	レイヤー数
					int layerCount = pMesh->GetLayerCount();

					cout << "layer count = " << layerCount << endl;

					//	ポリゴン頂点数
					int polygonVertexCount = pMesh->GetPolygonVertexCount();

					cout << "polygon vertex count = " << polygonVertexCount << endl;

					//	頂点格納
					{
						FbxVector4*vertex = pMesh->GetControlPoints();
						cout << "vertex" << endl << "No " << "x " << "y " << "z" << endl;
						for (int j = 0; j < controlPointsCount; ++j)
						{
							FLOAT4 pos;
							pos.x = (float)vertex[j][0];
							pos.y = (float)vertex[j][1];
							pos.z = (float)vertex[j][2];
							pos.w = (float)vertex[j][3];

							cout << j << ":" << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << endl;
							//positions.push_back(pos);
							output.vertexPos.push_back(pos);
						}
					}

					//	インデックス
					{
						auto polygonVertex = pMesh->GetPolygonVertices();

						//	頂点バッファのサイズ
						polygonIndex.resize(polygonCount * 3);

						//	インデックス格納用の配列の添え字(限定スコープ)
						int index = 0;

						cout << endl << "index buffer size = " << polygonCount * 3 << endl;

						cout << "index" << endl << "polygon No" << ", vertex index" << endl;

						for (int j = 0; j < polygonCount; ++j) {

							int polygonSize = pMesh->GetPolygonSize(j);
							for (int k = 0; k < polygonSize; ++k)
							{
								polygonIndex[index + k] = pMesh->GetPolygonVertex(j, k);
								cout << j << ":" << j << ", " << pMesh->GetPolygonVertex(j, k) << endl;
							}
							index += polygonSize;
						}
					}
					output.index = polygonIndex;

					//	法線ベクトル
					vector<FLOAT3>normals;

					//	法線
					{

						//	レイヤー
						FbxLayer*pLayer = pMesh->GetLayer(0);


						FbxLayerElementNormal*normalElem = pLayer->GetNormals();
						if (normalElem == NULL) { continue; }

						int normalCount = normalElem->GetDirectArray().GetCount();
						int normalIndexCount = normalElem->GetIndexArray().GetCount();

						cout << "normal count = " << normalCount << endl;
						cout << "normal index count = " << normalCount << endl;


						FbxLayerElement::EMappingMode	normalMapMode = normalElem->GetMappingMode();
						FbxLayerElement::EReferenceMode normalRefMode = normalElem->GetReferenceMode();

						switch (normalMapMode)
						{
						case fbxsdk::FbxLayerElement::eNone:
							break;
						case fbxsdk::FbxLayerElement::eByControlPoint:
						case fbxsdk::FbxLayerElement::eByPolygonVertex:
						{
							if (normalRefMode == FbxLayerElement::eDirect) {
								for (int j = 0; j < normalCount; ++j)
								{
									FLOAT3 n;
									n =
									{
										(float)normalElem->GetDirectArray().GetAt(j)[0],
										(float)normalElem->GetDirectArray().GetAt(j)[1],
										(float)normalElem->GetDirectArray().GetAt(j)[2],
									};
									normals.push_back(n);
								}

							}
						}
						break;
						case fbxsdk::FbxLayerElement::eByPolygon:
							break;
						case fbxsdk::FbxLayerElement::eByEdge:
							break;
						case fbxsdk::FbxLayerElement::eAllSame:
							break;
						default:
							break;
						}
					}

					//	ソート
					{
						vector<FLOAT3>sortNormal;
						sortNormal.resize(controlPointsCount);

						for (unsigned int j = 0; j < normals.size(); ++j)
						{
							sortNormal[polygonIndex[i]] = normals[i];
						}
					}
				}
			}

			else
			{
				//	コントロール点
				int controlPointsCount = pMesh->GetControlPointsCount();
				cout << "control point count = " << controlPointsCount << endl;


				int polygonCount = pMesh->GetPolygonCount();

				cout << "polygon count = " << polygonCount << endl;

				//	頂点
				vector<FLOAT4>vertex;

				auto vertics = pMesh->GetPolygonVertices();

				//	頂点格納
				{
					FbxVector4*vertex = pMesh->GetControlPoints();
					cout << "vertex" << endl << "No " << "x " << "y " << "z" << endl;
					for (int j = 0; j < controlPointsCount; ++j)
					{
						FLOAT4 pos;
						pos.x = (float)vertex[j][0];
						pos.y = (float)vertex[j][1];
						pos.z = (float)vertex[j][2];
						pos.w = (float)vertex[j][3];

						cout << j << ":" << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << endl;
						//positions.push_back(pos);
						output.vertexPos.push_back(pos);
					}
				}

				vector<int>index;

				cout << endl << "index buffer size = " << polygonCount * 3 << endl;

				cout << "index" << endl << "polygon No" << ", vertex index" << endl;

				//	インデックス
				for (int j = 0; j < polygonCount; ++j)
				{
					int polygonSize = pMesh->GetPolygonSize(j);

					for (int k = 0; k < polygonSize; ++k)
					{
						index.push_back(pMesh->GetPolygonVertex(j, k));
						cout << j << ":" << j << ", " << pMesh->GetPolygonVertex(j, k) << endl;
					}
				}
				output.index = index;

			}
			//	マテリアルインデックス
			//FbxLayerElementArrayTemplate<int>*matIndex;
		}
	}
#pragma endregion

	Utility::IOMesh::Output("", "abc", output);

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

void FBX::FBXUtility::SetupUV(fbxsdk::FbxMesh * fbxMesh, Abstract::AbstractMesh * mesh)
{
	auto layer = fbxMesh->GetLayer(0);
	auto uvs = layer->GetUVs();
	auto a = uvs->GetMappingMode();
	if (uvs->GetMappingMode() != FbxLayerElement::eByPolygonVertex) {
		cout << "uv取得失敗" << endl;
		return;
	}
	auto refMode = uvs->GetReferenceMode();

	FbxArray<FbxVector2>uvArray;
	uvs->GetDirectArray().CopyTo(uvArray);

	switch (refMode)
	{
	case fbxsdk::FbxLayerElement::eDirect:
		for (int i = 0; i < uvArray.Size(); ++i)
		{
			float u = (float)uvArray.GetAt(i)[0];
			float v = (float)uvArray.GetAt(i)[1];
			mesh->uv.push_back({ u,v });
		}
		break;
	case fbxsdk::FbxLayerElement::eIndex:
		cout << "設定なし" << endl;
		break;
	case fbxsdk::FbxLayerElement::eIndexToDirect:
		//NOTE:参考は "y:1.0f-uvArray.Get(index)[1]"を代入している
		for (int i = 0; i < uvArray.Size(); ++i)
		{
			auto index = uvs->GetIndexArray().GetAt(i);
			float u = (float)uvArray.GetAt(index)[0];
			float v = (float)uvArray.GetAt(index)[1];
			mesh->uv.push_back({ u, v });
		}
		break;
	default:
		break;
	}
	vector<FbxVector2>vf;
	for (auto it : mesh->uv)
	{
		vf.push_back({ (double)it.u,(double)it.v });
	}
	//ソート前
	vf.erase(unique(vf.begin(), vf.end()), vf.end());
	auto tmp = vf;
	int bNum = tmp.size();
	//ソート後
	sort(vf.begin(),vf.end());
	//	重複削除
	int aNum = vf.size();
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
