/*!
	@file	FBXConverter.h
	@date	2019/04/19
	@author	番場 宥輝
	@brief	FBX_SDKのManager,Sceneを扱うヘルパークラス
	@detail	シングルトン
*/
#pragma once
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <fbxsdk/core/fbxmanager.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/scene/shading/fbxlayeredtexture.h>
#include <fbxsdk/scene/shading/fbxfiletexture.h>
#include <fbxsdk/scene/geometry/fbxskin.h>
#include <fbxsdk/core/math/fbxmatrix.h>
#include <fbxsdk/core/math/fbxaffinematrix.h>
#include <DirectXMath.h>
#include <string_view>
#include <AnimationClip.h>
#include <Joint.h>
#include <SkinnedVertex.h>
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
			@enum	OutputType
			@brief	書き出すメッシュのタイプ
		*/
		enum OutputType
		{
			/*!
				@var	STATIC
				@brief	静的メッシュ
			*/
			STATIC,

			/*!
				@var	SKIN
				@brief	スキンメッシュ
			*/
			SKIN,
		};

		/*!
			@fn			Execute
			@brief		実行処理
			@detail		FBXを読み込み外部ファイルに書き出す。
			@param[in]	書き出すメッシュのタイプ
			@param[in]	FBXのパス
			@param[in]	出力先のファイルパス
		*/
		static void Execute(OutputType type,std::string fbxPath,std::string outputName);
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
			@fn			SetupVertexIndices
			@brief		頂点インデックスのセットアップ
			@param[in]	FBXメッシュ
			@param[in]	頂点インデックス格納用の可変長配列
		*/
		static void SetupVertexIndices(fbxsdk::FbxMesh& mesh, std::vector<uint32_t>&indices);

		/*!
			@fn			SetupVertices
			@brief		頂点のセットアップ
			@param[in]	参照元のfbxメッシュ
			@param[in]	頂点情報格納用の可変長配列
		*/
		static void SetupVertices(fbxsdk::FbxMesh&mesh, std::vector<D3D11::Graphic::SkinnedVertex>&vertices);

		/*!
			@fn			SetupUV
			@brief		UVのセットアップ
			@detail		頂点インデックスに対応したUVが格納される。
			@param[in]	FBXメッシュ
			@param[in]	UV格納用の可変長配列
		*/
		static void SetupUV(fbxsdk::FbxMesh& mesh, std::vector<DirectX::XMFLOAT2>&uv);

		/*!
			@fn			AlignVerticesToUV
			@brief		頂点をUVに揃える。
			@detail		事前に他の情報をそろえておく必要がある
			@param[in]	対象のメッシュ
		*/
		static void AlignVerticesToUV(std::vector<uint32_t>&indices, std::vector<DirectX::XMFLOAT2>&uv, std::vector<D3D11::Graphic::SkinnedVertex>&vertices);

		/*!
			@fn			SetupAnimation
			@brief		アニメーション情報のセットアップ
			@param[in]	FBXメッシュ
			@param[in]	アニメーションの情報を格納するアニメーションクリップ
		*/
		static void SetupAnimation(fbxsdk::FbxMesh&mesh,std::vector<API::AnimationClip>& clip);

		/*!
			@fn			SetupCluster
			@brief		クラスタ(ジョイント)のセットアップ
			@param[in]	メッシュのスキン
			@param[in]	指定時間のグローバルオフセット行列
			@param[in]	メッシュのオフセット(TRS)
			@param[in]	アニメーションの時間
			@param[in]	スキン行列を格納する行列パレット
		*/
		static void SetupCluster(fbxsdk::FbxSkin&skin, fbxsdk::FbxMatrix evaluateGlobalTimeMatrix, fbxsdk::FbxAMatrix geometryOffset, fbxsdk::FbxTime animTime, API::MatrixPalette& matrixPalette);
		
		/*!
			@fn			ExportTextureName
			@brief		メッシュに使用されているテクスチャの出力
			@param[in]	Fbxメッシュ
			@param[in]	出力ファイルパス
		*/
		static void ExportTextureName(fbxsdk::FbxMesh&mesh,std::string filePath);
		
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