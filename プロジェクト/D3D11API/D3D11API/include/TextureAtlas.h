/*
	@file	TextureAtlas.h
	@date	2018/11/20
	@author	番場 宥輝
	@brief	アトラステクスチャ
*/
#pragma once
#include "ITexture.h"

/*!
	@namespace	API
	@brief		API関連の名前空間に含める
*/
namespace API {

	class TextureAtlas :
		public ITexture
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		explicit TextureAtlas();

		/*!
			@brief	デストラクタ
		*/
		~TextureAtlas()override;

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	分割数(x,y)
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定、分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	表示する画像サイズ
						※分割前の画像サイズでないことに注意
			@param[in]	分割数(x,y)
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	分割数(x,y)
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum,const FilteringMode filterMode);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定、分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	表示する画像サイズ
						※分割前の画像サイズでないことに注意
			@param[in]	分割数(x,y)
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum, const FilteringMode filterMode);

		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な開放
		*/
		void Finalize()override;

		/*!
			@fn			分割数のセット
			@brief		アトラステクスチャの分割数をxとy方向にどれだけ分割するか指定
			@param[in]	分割数(x:x方向の個数,y:y方向の個数)
		*/
		void SetDevNum(const DirectX::XMFLOAT2 divNum);

		/*!
			@fn			描画インデックスのセッター
			@brief		描画するアトラステクスチャのインデックスを設定。
			@detail		アトラステクスチャの分割数より多い値は設定できません。
			@param[in]	描画したいテクスチャ番号
		*/
		void SetAtlasIndex(const DirectX::XMFLOAT2 index);

		/*!
			@fn		分割数のゲッター
			@brief	分割数の取得
			@detail	設定した分割数を取得する
		*/
		inline DirectX::XMFLOAT2 GetDivNum()const {
			return m_DivNum;
		}

		/*!
			@fn		描画インデックスのゲッター
			@brief	描画するアトラステクスチャのインデックス取得
			@detail	分割したテクスチャの描画場所を取得
		*/
		inline DirectX::XMFLOAT2 GetAtlasIndex()const {
			return m_Index;
		}
	private:
		DirectX::XMFLOAT2 m_DivNum;	/*!< 分割数 */
		DirectX::XMFLOAT2 m_Index;	/*!< 描画するテクスチャ番号 */

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定、分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	表示する画像サイズ
						※分割前の画像サイズでないことに注意
			@param[in]	分割数(x,y)
			@param[in]	タイリングモードの設定
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum, const TileMode tileMode, const FilteringMode filterMode);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	分割数(x,y)
			@param[in]	タイリングモードの設定
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum, const TileMode tileMode, const FilteringMode filterMode);

	};

}
