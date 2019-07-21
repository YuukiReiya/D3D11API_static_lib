#include "Common.h"
#include <Skeleton.h>
#include <SkeletonMesh.h>
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
	@fn			ConvertRelativePathToFileName
	@brief		相対パスをファイル名に変換
	@param[in]	相対パス
	@return		変換後のファイル名
*/
static string ConvertRelativePathToFileName(string relativePath)
{
	string fileName = "";
	auto offset = relativePath.rfind("\\");
	if (offset == string::npos) {
		offset = relativePath.rfind("/");
	}
	fileName = relativePath.substr(offset + 1);
	return fileName;
}

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

	cout << "----Display dialog----" << endl;

#pragma region アニメーションメッシュ
	//	出力データ
	//SkeletonMesh*outMesh = new SkeletonMesh;

	SkeletonMesh outMesh;
	AnimationClip animClip;
	try
	{
		Setup();
		//	色指定
		wic::SetColor(Green);

		//	インポーター
		if (!SetupImporter(fbxPath)) { throw runtime_error("Could not read \"" + fbxPath + "\"."); }
		cout << "SetupImporter success" << endl;

		//	シーン
		if (!SetupScene(fbxPath)) { throw runtime_error("Could not output \"" + fbxPath + "\" to the scene."); }
		cout << "SetupScene success" << endl;

		//	インポーターの破棄
		TeardownImporter();

		//	三角化
		if (!Triangulate()) { throw runtime_error("Failed to triangulate this scene."); }
		cout << "Triangulate success" << endl;

		//	メッシュの分割
		if (!SplitMeshesPerMaterial()) { throw runtime_error("Failed to split mesh"); }
		cout << "SplitMeshesPerMaterial success" << endl;

		//	メッシュの総数
		int meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();

		//	メッシュが単体か複数か確認
		if (meshCount == 1) {
			cout << "メッシュが一つです" << endl;
		}
		else {
			cout << "メッシュが複数です" << endl;
		}

		//	単一仮定
		auto pMesh = (*m_pScene.get())->GetSrcObject<fbxsdk::FbxMesh>(0);
		auto pNode = pMesh->GetNode();

		//	アニメーション
		FbxArray<FbxString*>animNameArray;
		(*m_pScene)->FillAnimStackNameArray(animNameArray);

		//	アニメーションの確認
		if (animNameArray.GetCount() == 0) {
			wic::SetColor(Red);
			//	メッセージを吐いて処理を抜ける
			cout << "Failed to read animation!" << endl;
			return;
		}

		cout << "anim list" << endl;
		for (int i = 0; i < animNameArray.GetCount(); ++i) {
			auto it = animNameArray.GetAt(i);
			cout << it << endl;
		}
		cout << "anim list count = " << animNameArray.Size() << endl;

		auto setAnimData = animNameArray[0];
		//	抽出アニメーション情報
		auto animStack = (*m_pScene)->FindMember<FbxAnimStack>(setAnimData->Buffer());
		//	アニメーションの設定
		(*m_pScene)->SetCurrentAnimationStack(animStack);

		auto info = (*m_pScene)->GetTakeInfo(*setAnimData);
		auto start = info->mLocalTimeSpan.GetStart();
		auto stop = info->mLocalTimeSpan.GetStop();
		FbxTime frameTime, timeCount;
		frameTime.SetTime(0, 0, 0, 1, 0, (*m_pScene)->GetGlobalSettings().GetTimeMode());
		timeCount = start;

		auto skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
		int boneCount = skinDeformer->GetClusterCount();
#pragma region ジョイントのメモリ確保
		//outMesh->skeleton.jointCount = boneCount;
		//outMesh->skeleton.joints = new Joint[outMesh->skeleton.jointCount];

		//outMesh.skeleton.jointCount = boneCount;
		//outMesh.skeleton.joints = new Joint[outMesh.skeleton.jointCount];

		outMesh.skeleton.joints.resize(boneCount);
		animClip.bonesMatrix.resize(boneCount);

#pragma endregion

		wic::SetColor(Purple);
		cout << "BoneNum = " << boneCount << endl;


		//	アニメーションのフレーム数
		unsigned int c_Frame = static_cast<unsigned int>(stop.Get() / frameTime.Get());
		//outMesh->frameMatrix.resize(c_Frame);
		animClip.frameCount = c_Frame;
#pragma region 頂点
		//	頂点数
		auto vertexCount = pMesh->GetControlPointsCount();

		//	頂点配列の先頭ポインタ
		FbxVector4* vertices = pMesh->GetControlPoints();
		int vertexIndex = 0;
		while (vertexIndex < vertexCount)
		{
			SkinnedVertex sv;
			sv.position = {
				(float)vertices[vertexIndex][0],
				(float)vertices[vertexIndex][1],
				(float)vertices[vertexIndex][2],
			};
			outMesh.vertices.push_back(sv);
			++vertexIndex;
		}
		cout << "頂点数 = " << vertexCount << endl;
#pragma endregion

#pragma region 頂点インデックス
		//	ポリゴン数
		auto polygonCount = pMesh->GetPolygonCount();
		for (int i = 0; i < polygonCount; ++i)
		{
			//	ポリゴンを構成する頂点数(※三角化していれば"3"になる)
			auto polygonVertexCount = pMesh->GetPolygonSize(i);
			if (polygonVertexCount != 3) { cout << "ERROR:三角化出来ていません" << endl; }
			for (int j = 0; j < polygonVertexCount; ++j)
			{
				outMesh.indices.push_back(
					{
						static_cast<uint32_t>(pMesh->GetPolygonVertex(i,j))
					}
				);
			}
		}
		cout << "インデックス数 = " << outMesh.indices.size() << endl;
#pragma endregion



		//	ボーン分
		//	※事前に頂点,頂点インデックスを設定していないと、範囲外参照を起こす！
		for (int i = 0; i < boneCount; ++i)
		{
			//	クラスタ取得
			auto cluster = skinDeformer->GetCluster(i);

			//	クラスタが影響を与える頂点インデックスの数
			auto indicesCount = cluster->GetControlPointIndicesCount();

			//	代入用の使いまわし型
			FbxAMatrix fbxMat;

#pragma region 初期姿勢
			//	初期姿勢
			cluster->GetTransformLinkMatrix(fbxMat);
			//fbxMat = fbxMat.Transpose();
			Joint* joint = new Joint;
			joint->invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			joint->invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			joint->invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			joint->invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			joint->invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			joint->invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			joint->invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			joint->invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			joint->invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			joint->invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			joint->invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			joint->invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			joint->invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			joint->invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			joint->invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			joint->invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			outMesh.skeleton.joints[i] = *joint;
			
			//Joint* joint=new Joint;
			//joint->invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			//joint->invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			//joint->invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			//joint->invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			//joint->invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			//joint->invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			//joint->invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			//joint->invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			//joint->invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			//joint->invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			//joint->invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			//joint->invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			//joint->invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			//joint->invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			//joint->invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			//joint->invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			//outMesh.skeleton.joints[i] = *joint;

			//Joint joint;
			//joint.invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			//joint.invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			//joint.invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			//joint.invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			//joint.invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			//joint.invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			//joint.invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			//joint.invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			//joint.invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			//joint.invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			//joint.invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			//joint.invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			//joint.invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			//joint.invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			//joint.invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			//joint.invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			
			//outMesh.skeleton.joints.push_back(joint);
			//outMesh.skeleton.joints[i] = joint;
#pragma endregion

#pragma region フレーム時姿勢
			//	フレーム時姿勢
			for (size_t f = 0; f < c_Frame; ++f) {
				auto time = start + frameTime * f;
				fbxMat = cluster->GetLink()->EvaluateGlobalTransform(time);
				fbxMat = fbxMat.Transpose();
				DirectX::XMMATRIX m =
				{
					static_cast<float>(fbxMat.Get(0,0)),static_cast<float>(fbxMat.Get(0,1)),static_cast<float>(fbxMat.Get(0,2)),static_cast<float>(fbxMat.Get(0,3)),
					static_cast<float>(fbxMat.Get(1,0)),static_cast<float>(fbxMat.Get(1,1)),static_cast<float>(fbxMat.Get(1,2)),static_cast<float>(fbxMat.Get(1,3)),
					static_cast<float>(fbxMat.Get(2,0)),static_cast<float>(fbxMat.Get(2,1)),static_cast<float>(fbxMat.Get(2,2)),static_cast<float>(fbxMat.Get(2,3)),
					static_cast<float>(fbxMat.Get(3,0)),static_cast<float>(fbxMat.Get(3,1)),static_cast<float>(fbxMat.Get(3,2)),static_cast<float>(fbxMat.Get(3,3)),
				};
				animClip.bonesMatrix[i].push_back(m);
			}
#pragma endregion

#pragma region 重み
			//	影響を与えるインデックスの配列
			auto indices = cluster->GetControlPointIndices();

			//	影響度(重み)の配列
			auto weights = cluster->GetControlPointWeights();

			//	クラスタが影響を与える頂点分
			for (int j = 0; j < indicesCount; ++j)
			{
				//	参照するインデックス
				auto index = indices[j];

				//	重み
				auto weight = weights[j];

				outMesh.vertices[index].jointsIndex.push_back(i);
				outMesh.vertices[index].jointsWeight.push_back(weight);
			}
#pragma endregion
		}
#pragma region 重みの正規化
		//for (auto& it : outMesh.vertices)
		//{
		//	//	総和
		//	float totalWeight = 0;
		//	for (auto weight : it.jointsWeight) {
		//		totalWeight += weight;
		//	}
		//	if (totalWeight == 1) { continue; }
		//	
		//	//	係数
		//	float coefficient = 1.0f / (totalWeight);

		//	auto tes = coefficient * totalWeight;
		//	float tm = 0.0f;
		//	for (auto& weight : it.jointsWeight) {
		//		weight *= coefficient;
		//		tm += weight;
		//	}
		//	if (tm != 1.0f) {
		//		cout << "Error:" << static_cast<float>(tm) << endl;
		//	}
		//}

		//for (auto it : outMesh.vertices) {
		//	float totalWeight = 0;
		//	for (auto weight : it.jointsWeight) {
		//		totalWeight += weight;
		//	}
		//	if (totalWeight != 1.0f) {
		//		cout << "正規化失敗" << endl;
		//	}
		//}
#pragma endregion

		cout << "書き出し" << endl;

		Utility::IOMesh::Output(outName, outMesh,animClip);

		cout << "start:" << start.Get() << endl;
		cout << "stop:" << stop.Get() << endl;
		cout << "ft:" << frameTime.Get() << endl;

	}
	catch (...) {}

	//#endif
#pragma endregion


	cout << "Completed this program." << endl;
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
	return converter.SplitMeshesPerMaterial(*m_pScene.get(), true);
}

/*!
	@fn			SetupImporter
	@brief		fbxファイルをインポーターに読み込む。
	@param[in]	読み込む.fbxファイルのパス
*/
bool Converter::FBXConverter::SetupImporter(std::string fbxPath)
{
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
		if (to->vertexIndices.empty())throw runtime_error("SetupVertexIndices");
		wic::SetColor(Green);
		cout << "SetupVertexIndices success" << endl;

		//	頂点
		SetupVertices(from, to);
		if (to->vertices.empty())throw runtime_error("SetupVertices");
		wic::SetColor(Green);
		cout << "SetupVertices success" << endl;

		//	UV
		SetupUV(from, to);
		if (to->uv.empty())throw runtime_error("SetupUV");
		wic::SetColor(Green);
		cout << "SetupUV success" << endl;

		//	頂点の整合化
		AlignVerticesToUV(to);
	}
	catch (exception&error)
	{
		wic::SetColor(Red);
		cout << "Failed to \"" << error.what() << "\"" << endl;
		wic::SetColor(White);
	}
}

void Converter::FBXConverter::SetupMaterial(fbxsdk::FbxSurfaceMaterial * material)
{

}

void Converter::FBXConverter::SetupMaterial(fbxsdk::FbxMesh * from)
{
	//	メッシュのルートノード取得
	auto node = from->GetNode();
	if (node == 0) { return; }

	//	マテリアル数
	auto matCount = node->GetMaterialCount();
	if (matCount == 0) { return; }

	//	マテリアル情報を取得
	for (int i = 0; i < matCount; i++)
	{
		auto material = node->GetMaterial(i);
		if (material == 0) { continue; }

#pragma region //ダウンキャスト
		//	ここの設計はFBXSDKに合わせている

		FbxSurfaceLambert*lambert=nullptr;
		FbxSurfacePhong* phong = nullptr;

		//	lambert
		if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			lambert = (FbxSurfaceLambert*)material;
		}

		//	phong
		if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			phong = (FbxSurfacePhong*)material;
		}

		auto castMat = material->GetClassId().Is(FbxSurfacePhong::ClassId) ?
			(FbxSurfacePhong*)material :
			material->GetClassId().Is(FbxSurfaceLambert::ClassId) ?
			(FbxSurfaceLambert*)material : nullptr;
		if (castMat == nullptr) {
			cout << "やばいバグ" << endl;
			system("pause");
			exit(NULL);
		}
#pragma endregion
		////	アンビエント
		//g_pMat->ambient =
		//{
		//	(float)castMat->Ambient.Get()[0],
		//	(float)castMat->Ambient.Get()[1],
		//	(float)castMat->Ambient.Get()[2]
		//};

		////	ディフューズ
		//g_pMat->diffuse =
		//{
		//	(float)castMat->Diffuse.Get()[0],
		//	(float)castMat->Diffuse.Get()[1],
		//	(float)castMat->Diffuse.Get()[2],
		//};

		////	エミッシブ
		//g_pMat->emissive =
		//{
		//	(float)castMat->Emissive.Get()[0],
		//	(float)castMat->Emissive.Get()[1],
		//	(float)castMat->Emissive.Get()[2],
		//};

		////	バンプマップ
		//g_pMat->bumpMap =
		//{
		//	(float)castMat->Bump.Get()[0],
		//	(float)castMat->Bump.Get()[1],
		//	(float)castMat->Bump.Get()[2],
		//};

		////	透過度
		//g_pMat->transparent = static_cast<float>(castMat->TransparencyFactor.Get());

		////	マテリアルが"Phong"なら
		//if (castMat->GetClassId().Is(FbxSurfacePhong::ClassId)) {

		//	//	スペキュラー
		//	Math::FLOAT4 sp =
		//	{
		//		(float)phong->Specular.Get()[0],
		//		(float)phong->Specular.Get()[1],
		//		(float)phong->Specular.Get()[2],
		//	};
		//	g_pMat->specular = { &sp };

		//	//	光沢
		//	float sh = (float)phong->Shininess.Get();
		//	g_pMat->shiniess = { &sh };

		//	//	反射
		//	float refl = (float)phong->ReflectionFactor.Get();
		//	g_pMat->reflection = { &refl };
		//}

	}
}

/*!
	@note	そのマテリアルに使用しているテクスチャデータのセットアップ
	
*/
void Converter::FBXConverter::SetupTextures(fbxsdk::FbxSurfaceMaterial * material)
{
	//	ディフューズ
	auto prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//	複数のテクスチャをブレンドしたもの(レイヤーテクスチャ)
	auto layerTexCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
	for (int i = 0; i < layerTexCount; ++i)
	{
		auto layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(i);
		wic::SetColor(Purple);
		SetupLayerTextures(&prop, layeredTexture);
	}

	//	単一のもの
	auto texCount = prop.GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texCount; i++)
	{
		auto tex = prop.GetSrcObject<FbxFileTexture>(i);
		wic::SetColor(Purple);
		if (!tex) { continue; }
		cout << "Texture " << i << ":";
		SetupTexture(tex);
	}

	//TODO: これが成り立つのが"layerTexCount = texCount = 0"の時のみ
	//		分かりにくいので"layerTexCount == 0 && texCount == 0"がいいかも...
	//NOTE:	ここに入るのは".tga"などテクスチャ読み込みに失敗したもの
	if (layerTexCount == texCount)
	{
		
	}
	
}

void Converter::FBXConverter::SetupLayerTextures(fbxsdk::FbxProperty * prop, fbxsdk::FbxLayeredTexture * layerdTexture)
{
	auto texCount = layerdTexture->GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texCount; ++i)
	{
		auto texture = prop->GetSrcObject<FbxFileTexture>(i);
		wic::SetColor(Purple);
		if (!texture) { continue; }
		cout << "Layered Texture " << i << ":";
		SetupTexture(texture);
	}
}

void Converter::FBXConverter::SetupTexture(fbxsdk::FbxFileTexture * texture)
{
	//TODO:マテリアル内のテクスチャ情報取得処理

	//FbxMaterialConverter
	//	ファイル名の表示
	wic::SetColor(White);
	cout << ConvertRelativePathToFileName(texture->GetRelativeFileName()) << endl;
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

	wic::SetColor(Cyan);
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
	wic::SetColor(Cyan);
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

	wic::SetColor(Cyan);
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

		wic::SetColor(Purple);
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
	wic::SetColor(Cyan);
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
	wic::SetColor(Cyan);
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
				cout << "Failed to reconfigure index." << endl;
				cout << "uv set name:";
				wic::SetColor(White);
				cout << hash << endl;
				wic::SetColor(Red);
				cout << "index number:";
				wic::SetColor(White);
				cout << i << endl;
				wic::SetColor(Red);
				cout << "iterator not found";
				wic::SetColor(White);
				continue;
			}
		}
	}

	//	コンソール出力
	cout << endl;
	wic::SetColor(Cyan);
	cout << "mesh vertex size:";
	wic::SetColor(White);
	cout << mesh->vertices.size() << endl;
	wic::SetColor(Cyan);
	cout << "mesh index size:";
	wic::SetColor(White);
	cout << mesh->vertexIndices.size() << endl;
	wic::SetColor(Cyan);
	cout << "mesh uv size" << endl;
	const auto c_UVSetNamesCount = mesh->uvSetNamesList.size();

	for (size_t i = 0; i < c_UVSetNamesCount; i++)
	{
		wic::SetColor(Cyan);
		cout << mesh->uvSetNamesList[i] << ":";
		wic::SetColor(White);
		cout << mesh->uv[mesh->uvSetNamesList[i]].size() << endl;
	}
}

