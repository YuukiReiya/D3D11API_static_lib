/*
	@file	FBXUtility.h
	@date	2019/04/19
	@author	番場 宥輝
	@brief	FBX_SDKのManager,Sceneを扱うヘルパークラス
	@detail	シングルトン
*/
#pragma once
#include "AbstractMesh.h"

namespace FBX {
	class FBXUtility
	{
	public:

		/*!
			@fn		Setup
			@brief	初期化
			@detail	メンバのインスタンス化と初期化
		*/
		static void Setup();

		/*!
			@fn			Load
			@brief		.fbxのロード
			@detail		例外処理有り(Releaseビルドで戻り値)
						※Debugビルドではexit関数を使った強制終了
			@param[in]	読み込むファイルのパス
			@return		true:成功 false:失敗
		*/
		static bool Load(std::string path);

		/*!
			@fn			Load
			@brief		.fbxのロード
			@detail		例外処理有り(Releaseビルドで戻り値)
						※Debugビルドではexit関数を使った強制終了
			@param[in]	読み込むファイルのパス
			@return		true:成功 false:失敗
		*/
		static bool Load(std::string path, std::string outputPath, Abstract::AbstractMesh*mesh);

		static void SetupVertex(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupIndexBuffer(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupNormal(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupUV(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void Destroy();

		/*!
			@fn		Triangulate
			@brief	シーン内のモデルの三角ポリゴン化
			@return	true:成功 false:失敗
		*/
		static bool Triangulate();

	private:
		/*!
			@brief	コンストラクタ
		*/
		FBXUtility() = delete;

		/*!
			@brief	デストラクタ
		*/
		~FBXUtility() = delete;


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


		////
		static fbxsdk::FbxManager* pManager;
		static fbxsdk::FbxScene* pScene;
		static fbxsdk::FbxImporter* pImporter;
		static fbxsdk::FbxIOSettings*pSettings;

	};

}