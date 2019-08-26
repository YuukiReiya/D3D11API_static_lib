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
#include <MyGame.h>

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
	@param[in]	書き出すメッシュのタイプ
	@param[in]	FBXのパス
	@param[in]	出力先のファイルパス
*/
void Converter::FBXConverter::Execute(OutputType type, std::string fbxPath, std::string outputName)
{
	//	初期化
	Setup();

	//	出力カラー
	wic::SetColor(Green);

	try
	{
		//	インポーター
		if (!SetupImporter(fbxPath)) { throw runtime_error("Could not read \"" + fbxPath + "\"."); }
		cout << "SetupImporter success" << endl;

		//	シーン
		if (!SetupScene(fbxPath)) { throw runtime_error("Could not output \"" + fbxPath + "\" to the scene."); }
		cout << "SetupScene success" << endl;

		//	三角化
		if (!Triangulate()) { throw runtime_error("Failed to triangulate this scene."); }
		cout << "Triangulate success" << endl;

		//	メッシュの分割
		if (!SplitMeshesPerMaterial()) { throw runtime_error("Failed to split mesh"); }
		cout << "SplitMeshesPerMaterial success" << endl;

		//	メッシュの総数
		int meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();

		//	書き出し数
		uint32_t outputCount = 0;

		for (int i = 0; i < meshCount; ++i)
		{
			auto pMesh= (*m_pScene.get())->GetSrcObject<FbxMesh>(i);
		
			//	スキンメッシュ設定でスキンが無い場合、次
			if (type == OutputType::SKIN&&pMesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) <= 0) { continue; }

			//	書き出し数カウント
			outputCount++;

			//	書き出し名
			std::string outputFullPath = outputName + "/";
			outputFullPath += meshCount == 1 ? outputName : to_string(outputCount) + "/";

			//	頂点インデックス
			vector<uint32_t>indices;
			SetupVertexIndices(*pMesh, indices);

			//	UV
			vector<DirectX::XMFLOAT2>uv;
			SetupUV(*pMesh, uv);

			//	頂点座標
			vector<D3D11::Graphic::SkinnedVertex>vertices;
			SetupVertices(*pMesh, vertices);

			//	頂点の整合
			AlignVerticesToUV(indices, uv, vertices);

			//	アニメーション
			vector<API::AnimationClip> clips;
			SetupAnimation(*pMesh,clips);

			//	書き出し
			Utility::IOMesh::OutputSkinMesh(outputFullPath + outputName, indices, vertices);

			//	スキン設定じゃなければアニメーションは書き出さない
			if (type != OutputType::SKIN) { continue; }

			const bool isMultiAnimation = clips.size() != 1;
			if (isMultiAnimation)
			{
				for (size_t animCount = 0; animCount < clips.size(); ++animCount)
				{
					string fileName = outputFullPath + "anim-" + to_string(animCount);
					Utility::IOMesh::OutputAnimation(fileName, clips[animCount]);
				}

			}
			else {
				Utility::IOMesh::OutputAnimation(outputName + "/" + "anim", clips[0]);
			}
		}
	}
	catch (exception&e)
	{
		wic::SetColor(Red);
		cout << "ERROR" << endl;
		cout << e.what() << endl;
		return;
	}

	wic::SetColor(Green);
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
	@param[in]	FBXメッシュ
	@param[in]	頂点インデックス格納用の可変長配列
*/
void Converter::FBXConverter::SetupVertexIndices(fbxsdk::FbxMesh & mesh, std::vector<uint32_t>& indices)
{
	auto polygonCount = mesh.GetPolygonCount();
	for (int i = 0; i < polygonCount; ++i)
	{
		//	ポリゴンを構成する頂点数(三角化していれば必ず"3")
		auto polygonVertexCount = mesh.GetPolygonSize(i);
		
		//	頂点数"3"コになっていなければ以降の動作は未保障なのでエラー判定を仕込んどく
		constexpr uint32_t c_PolygonVertexCount = 3;
		if (polygonVertexCount != c_PolygonVertexCount)
		{
			throw runtime_error("頂点を三角化出来ていません。");
		}
		for (int j = 0; j < polygonVertexCount; ++j)
		{
			indices.push_back(
				{
					static_cast<uint32_t>(mesh.GetPolygonVertex(i,j))
				}
			);
		}
	}
}

/*!
	@fn			SetupVertices
	@brief		頂点のセットアップ
	@param[in]	参照元のfbxメッシュ
	@param[in]	頂点情報格納用の可変長配列
*/
void Converter::FBXConverter::SetupVertices(fbxsdk::FbxMesh & mesh, std::vector<D3D11::Graphic::SkinnedVertex>& vertices)
{
	auto vertexCount = mesh.GetControlPointsCount();
	FbxVector4*controlPoints = mesh.GetControlPoints();
	//	座標
	for (int i = 0; i < vertexCount; ++i)
	{
		D3D11::Graphic::SkinnedVertex v;
		v.position = {
			static_cast<float>(controlPoints[i][0]),
			static_cast<float>(controlPoints[i][1]),
			static_cast<float>(controlPoints[i][2]), 
		};
		v.uv = { 0,0 };
		vertices.push_back(v);
	}
	//	UV


	//	ジョイント情報(重み)
	auto skinDeformerCount = mesh.GetDeformerCount(FbxDeformer::EDeformerType::eSkin);
	//cout << "Skin Deformer Count = " << skinDeformerCount << endl;
	if (skinDeformerCount != 1) { throw runtime_error("Skin Deformer Count != 1"); }
	for (int skinDeformerIndex = 0; skinDeformerIndex < skinDeformerCount; ++skinDeformerIndex)
	{
		auto skinDeformer = static_cast<FbxSkin*>(mesh.GetDeformer(skinDeformerIndex, FbxDeformer::EDeformerType::eSkin));
		const int clusterCount = skinDeformer->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			auto cluster = skinDeformer->GetCluster(clusterIndex);
			auto indicesCount = cluster->GetControlPointIndicesCount();
			for (int i = 0; i < indicesCount; ++i)
			{
				auto index = cluster->GetControlPointIndices()[i];
				auto weight = cluster->GetControlPointWeights()[i];
				vertices[index].joints.push_back({ static_cast<uint32_t>(clusterIndex), static_cast<float>(weight) });
			}
		}
	}
}

/*!
	@fn			SetupUV
	@brief		UVのセットアップ
	@detail		頂点インデックスに対応したUVが格納される。
	@param[in]	FBXメッシュ
	@param[in]	UV格納用の可変長配列
*/
void Converter::FBXConverter::SetupUV(fbxsdk::FbxMesh & mesh, std::vector<DirectX::XMFLOAT2>& uv)
{
	//	UVセット数
	FbxStringList uvSetNameList;
	mesh.GetUVSetNames(uvSetNameList);
	const int c_UVSetCount = uvSetNameList.GetCount();
	if (c_UVSetCount < 0 || c_UVSetCount>1) {
		wic::SetColor(Yellow);
		cout << "Warning" << endl << "マテリアル分割が上手く行われていません。" << endl;
		cout << "※意図しないUV座標になる場合があります。" << endl;
	}
	FbxArray<FbxVector2>positions;
	const string c_UVSetName = uvSetNameList.GetStringAt(0);
	mesh.GetPolygonVertexUVs(c_UVSetName.c_str(), positions);
	for (int j = 0; j < positions.Size(); ++j)
	{
		uv.push_back(
			{
				static_cast<float>(positions[j][0]),
				1.0f - static_cast<float>(positions[j][1]),
			}
		);
	}
}

/*!
	@fn			AlignVerticesToUV
	@brief		頂点をUVに揃える。
	@detail		事前に他の情報をそろえておく必要がある
	@param[in]	対象のメッシュ
*/
void Converter::FBXConverter::AlignVerticesToUV(std::vector<uint32_t>& indices, std::vector<DirectX::XMFLOAT2>& uv, std::vector<D3D11::Graphic::SkinnedVertex>& vertices)
{
	//	頂点順に格納
	vector<D3D11::Graphic::SkinnedVertex>skinVertices;
	for (size_t i = 0; i < indices.size(); ++i)
	{
		D3D11::Graphic::SkinnedVertex val = vertices[indices[i]];
		val.uv = uv[i];
		skinVertices.push_back(val);
	}

	//	インデックスのCpyバッファ
	auto indicesCpy = indices;

	//	情報をクリア
	indices.clear();
	vertices.clear();

	//	一意な頂点
	vector<D3D11::Graphic::SkinnedVertex>uniqueSkinVertices;
	for (size_t i = 0; i < indicesCpy.size(); ++i)
	{
		auto v = skinVertices[i];
		auto itr = find(uniqueSkinVertices.begin(), uniqueSkinVertices.end(), v);
		if (uniqueSkinVertices.end() != itr)
		{
			indices.push_back(std::distance(uniqueSkinVertices.begin(), itr));
			continue;
		}
		uniqueSkinVertices.push_back(v);
		indices.push_back(uniqueSkinVertices.size() - 1);
	}

	//	コピー
	vertices = uniqueSkinVertices;
}

/*!
	@fn			SetupAnimation
	@brief		アニメーション情報のセットアップ
	@param[in]	FBXメッシュ
	@param[in]	バインドするアニメーションクリップ
*/
void Converter::FBXConverter::SetupAnimation(FbxMesh&mesh, std::vector<API::AnimationClip>& clips)
{
	auto animStackCount = (*m_pScene.get())->GetSrcObjectCount<FbxAnimStack>();
	auto pNode = mesh.GetNode();
	for (int animIndex = 0; animIndex < animStackCount; ++animIndex)
	{
		auto pAnimStack = FbxCast<FbxAnimStack>((*m_pScene.get())->GetSrcObject<FbxAnimStack>(animIndex));
		(*m_pScene)->SetCurrentAnimationStack(pAnimStack);

		//	アニメーション情報
		auto info = (*m_pImporter)->GetTakeInfo(animIndex);
		
		//	ローカル時間
		//auto offset = info->mImportOffset;
		auto localStartTime = info->mLocalTimeSpan.GetStart();
		auto localStopTime = info->mLocalTimeSpan.GetStop();
		
		FbxTime timeSpan;
		timeSpan.SetTime(0, 0, 0, 1, 0,(*m_pScene)->GetGlobalSettings().GetTimeMode());
		
		//	60fps
		//float startTime = (offset.Get() + localStartTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::EMode::eFrames60);
		//float stopTime = (offset.Get() + localStopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::EMode::eFrames60);

		//	アニメーションフレーム
		const size_t c_Frame = static_cast<size_t>((localStopTime.Get() - localStartTime.Get()) / timeSpan.Get());

		//	行列パレット[フレーム][ジョイント]
		API::AnimationClip buf;
		buf.matrixPalette.resize(c_Frame);
		for (size_t frame = 0; frame < c_Frame; frame++)
		{
			auto skinDeformerCount = mesh.GetDeformerCount(FbxDeformer::EDeformerType::eSkin);
			//cout << "Skin Deformer Count = " << skinDeformerCount << endl;
			if (skinDeformerCount != 1) { throw runtime_error("Skin Deformer Count != 1"); }
			for (int skinDeformerIndex = 0; skinDeformerIndex < skinDeformerCount; ++skinDeformerIndex)
			{
				auto skinDeformer = static_cast<FbxSkin*>(mesh.GetDeformer(skinDeformerIndex, FbxDeformer::EDeformerType::eSkin));

				//
				SetupCluster(
					*skinDeformer,
					pNode->EvaluateGlobalTransform(frame*timeSpan.Get()),
					FbxAMatrix(
						pNode->GetGeometricTranslation(FbxNode::EPivotSet::eSourcePivot),
						pNode->GetGeometricRotation(FbxNode::EPivotSet::eSourcePivot),
						pNode->GetGeometricScaling(FbxNode::EPivotSet::eSourcePivot)
					),
					frame * timeSpan.Get(),
					buf.matrixPalette[frame]
				);

				
			}
		}

		//	ジョイント数
		const size_t c_JointCount = buf.matrixPalette[0].size();

		//	行列パレット[ジョイント][フレーム]に変換
		API::AnimationClip clip;
		clip.matrixPalette.resize(c_JointCount);
		for (auto& it : clip.matrixPalette) { it.resize(c_Frame); }
		for (size_t frame = 0; frame < c_Frame; frame++)
		{
			for (size_t jointIndex = 0; jointIndex < c_JointCount; jointIndex++)
			{
				clip.matrixPalette[jointIndex][frame] = buf.matrixPalette[frame][jointIndex];
			}
		}

		clips.push_back(clip);
	}
}

/*!
	@fn			SetupCluster
	@brief		クラスタ(ジョイント)のセットアップ
	@param[in]	メッシュのスキン
	@param[in]	指定時間のグローバルオフセット行列
	@param[in]	メッシュのオフセット(TRS)
	@param[in]	アニメーションの時間
	@param[in]	スキン行列を格納する行列パレット
*/
void Converter::FBXConverter::SetupCluster(fbxsdk::FbxSkin & skin, FbxMatrix evaluateGlobalTimeMatrix, FbxAMatrix geometryOffset, FbxTime animTime, API::MatrixPalette& matrixPalette)
{
	//	クラスタ数
	auto clusterCount = skin.GetClusterCount();
	for (int i = 0; i < clusterCount; ++i)
	{
		auto cluster = skin.GetCluster(i);

		//	スキニング行列
		FbxMatrix skinningMatrix;

		//
		FbxMatrix clusterGlobalCurrentPosition, clusterRelativeInitPosition, clusterRelativeCurrentPositionInverse;
		FbxAMatrix referenceGlobalInitPosition, clusterGlobalInitPosition;

		cluster->GetTransformMatrix(referenceGlobalInitPosition);
		referenceGlobalInitPosition *= geometryOffset;
		cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
		clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(animTime);
		clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse()*referenceGlobalInitPosition;
		clusterRelativeCurrentPositionInverse = evaluateGlobalTimeMatrix.Inverse()*clusterGlobalCurrentPosition;

		skinningMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
		DirectX::XMMATRIX mat = {
			static_cast<float>(skinningMatrix.Get(0,0)),static_cast<float>(skinningMatrix.Get(0,1)),static_cast<float>(skinningMatrix.Get(0,2)),static_cast<float>(skinningMatrix.Get(0,3)),
			static_cast<float>(skinningMatrix.Get(1,0)),static_cast<float>(skinningMatrix.Get(1,1)),static_cast<float>(skinningMatrix.Get(1,2)),static_cast<float>(skinningMatrix.Get(1,3)),
			static_cast<float>(skinningMatrix.Get(2,0)),static_cast<float>(skinningMatrix.Get(2,1)),static_cast<float>(skinningMatrix.Get(2,2)),static_cast<float>(skinningMatrix.Get(2,3)),
			static_cast<float>(skinningMatrix.Get(3,0)),static_cast<float>(skinningMatrix.Get(3,1)),static_cast<float>(skinningMatrix.Get(3,2)),static_cast<float>(skinningMatrix.Get(3,3)),
		};

		matrixPalette.push_back(mat);
	}
}