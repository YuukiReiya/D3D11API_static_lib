/*!
	@file	FBXConverter.h
	@date	2019/04/19
	@author	番場 宥輝
	@brief	FBX_SDKのManager,Sceneを扱うヘルパークラス
	@detail	シングルトン
*/
#pragma once
#include "Mesh.h"
#include "OriginalFormatMath.h"
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <fbxsdk/core/fbxmanager.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/scene/shading/fbxlayeredtexture.h>
#include <fbxsdk/scene/shading/fbxfiletexture.h>
namespace Converter {
	class FBXConverter
	{
	private:
		/*!
			@brief	コンストラクタ
		*/
		FBXConverter();

		/*!
			@brief	デストラクタ
		*/
		~FBXConverter();

	public:
		/*!
			@fn		Setup
			@brief	初期化。
			@detail	メンバのインスタンス化と初期化
		*/
		static void Setup();

		/*!
			@fn		Teardown
			@brief	破棄処理。
		*/
		static void Teardown();

		/*!
			@fn			Execute
			@brief		実行処理
			@detail		FBXを読み込み外部ファイルに書き出す。
			@param[in]	FBXのパス
			@param[in]	出力先のファイルパス
		*/
		static void Execute(std::string fbxPath,std::string outName);

	private:
		/*!
			@fn		Triangulate
			@brief	シーン内のモデルの三角ポリゴン化。
			@note	try catchをいれた単純なラッパー
			@return	true:成功 false:失敗
		*/
		static bool Triangulate();

		/*!
			@fn		SplitMeshesPerMaterial
			@brief	シーン内のモデルをマテリアル単位に分割
			@note	try catchをいれた単純なラッパー	
			@return	true:成功 false:失敗
		*/
		static bool SplitMeshesPerMaterial();

		/*!
			@fn			SetupImporter
			@brief		fbxファイルをインポーターに読み込む。
			@param[in]	読み込む.fbxファイルのパス
		*/
		static bool SetupImporter(std::string fbxPath);

		/*!
			@fn		TeardownImporter
			@brief	インポーターの破棄処理。
		*/
		static void TeardownImporter();

		/*!
			@fn			SetupScene
			@brief		インポーターに読み込んだデータをシーンにインポート。
			@detail		読み込み後にインポーターは破棄している。
			@param[in]	読み込む.fbxファイルのパス
		*/
		static bool SetupScene(std::string fbxPath);

		/*!
			@fn			LoadToStore
			@brief		fbxのデータを格納
			@param[in]	抽出元のデータ
			@param[in]	格納先メッシュ
		*/
		static void LoadToStore(fbxsdk::FbxMesh*from, Utility::Mesh*to);

		static void SetupMaterial(fbxsdk::FbxSurfaceMaterial*material);
		static void SetupMaterial(fbxsdk::FbxMesh* from);

		static void SetupTextures(fbxsdk::FbxSurfaceMaterial * material);

		static void SetupLayerTextures(fbxsdk::FbxProperty*prop, fbxsdk::FbxLayeredTexture*layerdTexture);

		static void SetupTexture(fbxsdk::FbxFileTexture*texture);

		/*!
			@fn			SetupVertexIndices
			@brief		頂点インデックスのセットアップ
			@param[in]	参照元のfbxメッシュ
			@param[in]	バインド先のメッシュ
		*/
		static void SetupVertexIndices(fbxsdk::FbxMesh* from, Utility::Mesh*to);

		/*!
			@fn			SetupVertices
			@brief		頂点のセットアップ
			@param[in]	参照元のfbxメッシュ
			@param[in]	バインド先のメッシュ
		*/
		static void SetupVertices(fbxsdk::FbxMesh*from, Utility::Mesh*to);

		/*!
			@fn			SetupUV
			@brief		UVのセットアップ
			@detail		頂点インデックスに対応したUVが格納される。
			@param[in]	参照元のfbxメッシュ
			@param[in]	バインド先のメッシュ
		*/
		static void SetupUV(fbxsdk::FbxMesh* from, Utility::Mesh*to);

		/*!
			@fn			AlignVerticesToUV
			@brief		頂点をUVに揃える。
			@detail		事前に他の情報をそろえておく必要がある
			@param[in]	対象のメッシュ
		*/
		static void AlignVerticesToUV(Utility::Mesh*mesh);

		/*!
			@var	m_pManager
			@brief	FbxManagerのポインタ
			@detail	スマートポインタでアドレス管理
		*/
		static std::shared_ptr<fbxsdk::FbxManager*>		m_pManager;

		/*!
			@var	m_pIOsetting
			@brief	FbxIOSettingsのポインタ
			@detail	スマートポインタでアドレス管理
		*/
		static std::shared_ptr<fbxsdk::FbxIOSettings*>	m_pIOsetting;

		/*!
			@var	m_pScene
			@brief	FbxSceneのポインタ
			@detail	スマートポインタでアドレス管理
		*/
		static std::shared_ptr<fbxsdk::FbxScene*>		m_pScene;

		/*!
			@var	m_pImporter
			@brief	FbxImporterのポインタ
			@detail	スマートポインタでアドレス管理
		*/
		static std::shared_ptr<fbxsdk::FbxImporter*>	m_pImporter;

		/*!
			@var	c_OutputDirectory
			@brief	外部ファイルの主力先ディレクトリ
		*/
		static constexpr std::string_view c_OutputDirectory = "Output/";

	};

}