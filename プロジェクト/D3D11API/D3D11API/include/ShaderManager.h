///*
//	@file	ShaderManager.h
//	@date	2018/03/04
//	@author	番場 宥輝
//	@brief	シェーダー関連
//	@detail	vector,mapの注意:http://msty.hatenablog.jp/entry/2016/08/18/144934
//*/
//#pragma once
//#include <D3D11.h>
//#include <unordered_map>
//#include <wrl/client.h>
//#include "Singleton.h"
//
//#if defined DEBUG||_DEBUG
//# define DEBUG_SHADER
//#endif
//
///*! Direct3D11関連 */
//namespace D3D11 {
//
//	/*! 描画関連の名前空間に含める */
//	namespace Graphic {
//
//		/*!
//			@brief	シェーダーを構成する構造体
//		*/
//		struct ShaderData
//		{
//		public:
//			/*!
//				@brief	コンストラクタ
//			*/
//			ShaderData(){
//				SecureZeroMemory(this, sizeof(this));
//				m_pVertexLayout		= nullptr;
//				m_pVertexShader		= nullptr;
//				m_pPixelShader		= nullptr;
//				m_pConstantBuffer	= nullptr;
//			}
//
//			/*!
//				@brief	デストラクタ
//			*/
//			~ShaderData()
//			{
//				m_pVertexLayout.Reset();
//				m_pVertexShader.Reset();
//				m_pPixelShader.Reset();
//				m_pConstantBuffer.Reset();
//			};
//
//			Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_pVertexLayout;	/*< インプットレイアウト */
//			Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_pVertexShader;	/*< 頂点シェーダー */
//			Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;		/*< ピクセルシェーダー */
//			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pConstantBuffer;	/*< コンスタントバッファー */
//		};
//
//		/*!
//			@brief	シェーダー管理クラス
//			@detail	シェーダーを作成せずとも描画出来るデフォルトシェーダーを用意する
//		*/
//		class ShaderManager
//			:public Singleton<ShaderManager>
//		{
//		public:
//			/*!
//				@brief	デストラクタ
//			*/
//			~ShaderManager();
//
//			/*!
//				@var	c_szSimpleTextureShader
//				@brief	通常テクスチャで使うシェーダーのハッシュ
//				@detail	unordered_mapに追加したシェーダーデータの参照用のハッシュ
//			*/
//			static const std::string c_szSimpleTextureShader;
//
//			/*!
//				@var	c_szTextureAtlasShader
//				@brief	アトラステクスチャで使うシェーダーのハッシュ
//				@detail	unordered_mapに追加したシェーダーデータの参照用のハッシュ
//			*/
//			static const std::string c_szTextureAtlasShader;
//
//			/*!
//				@var	c_DefaultMeshShader
//				@brief	デフォルト設定のメッシュで使うシェーダーのハッシュ
//				@detail	unordered_mapに追加したシェーダーデータの参照用のハッシュ
//			*/
//			static const std::string c_DefaultMeshShader;
//
//			/*!
//				@fn			シェーダーデータの追加
//				@brief		unordered_mapに作成したシェーダーデータを追加登録
//				@param[in]	ハッシュ名
//				@param[in]	登録するシェーダーデータの参照
//				@return		S_OK:成功 E_FAIL:失敗
//			*/
//			HRESULT AddNewShaderData(std::string szKeyName, ShaderData* pNewShaderData);	
//
//			/*!
//				@fn			イニシャライズ
//				@brief		初期化
//				@detail		通常テクスチャ/アトラステクスチャで使うシェーダーの生成
//				@param[in]	シェーダー(HLSL)の含まれているディレクトリのパス
//				@return		S_OK:成功 E_FAIL:失敗
//			*/
//			HRESULT Initialize(std::string directoryPath = "");
//
//			/*!
//				@fn		ファイナライズ
//				@brief	破棄処理
//				@detail	明示的なメンバの開放処理
//			*/
//			void Finalize();
//
//			/*!
//				@fn			シェーダー作成
//				@detail		ShaderDataのID3D11Shaderを作成
//				@param[in]	設定したディレクトリからのシェーダーファイルのパス(.hlsl)
//				@param[in]	シェーダーのエントリーポイント
//				@param[in]	シェーダーのプロファイル情報(小文字_バージョン) ex) vs_5_0
//				@param[in]	ID3D11*****Shaderのポインタをキャストして渡す ex)(void**)pVertexShader
//				@param[in]	コンパイル用のブロブをポインタで
//				@return		S_OK:成功 E_FAIL:失敗
//			*/
//			HRESULT MakeShader(std::string szFileName, std::string szFuncName, std::string szProfileName, void** ppShader, ID3DBlob** ppBlob);
//
//			/*!
//				@fn			シェーダーデータの取得
//				@brief		unorderd_mapに格納したシェーダーデータをハッシュキーから取得
//				@param[in]	登録ハッシュ
//				@return		ハッシュに登録されたしたシェーダーデータを返す
//							探索文字列から情報を取得することに失敗した場合NULLを返す
//			*/
//			ShaderData* GetShaderData(std::string szKeyName);
//
//		private:
//			/*!
//				@brief	コンストラクタ
//			*/
//			ShaderManager();
//
//			/*!
//				@brief	シングルトンデザインパターンのテンプレート継承
//			*/
//			friend class Singleton<ShaderManager>;
//
//			/*!
//				@var	m_DirectoryPath;
//				@brief	シェーダー(.hlsl)が含まれているディレクトリのパス
//				@detail	MakeShaderはこのパス+ファイルの名前のパスに存在するHLSLファイルでシェーダーの作成を行う
//			*/
//			std::string m_DirectoryPath;
//
//			/*!
//				@var	m_pAddDataRef
//				@brief	データ追加用の参照メンバ
//			*/
//			ShaderData* m_pAddDataRef;
//
//			/*!
//				@var	m_pShaderDataUMap;
//				@brief	シェーダーデータの連想配列
//			*/
//			std::unordered_map<std::string, ShaderData*>m_pShaderDataUMap;
//
//		};
//
//	}
//}
