#include "stdafx.h"
#include "Texture.h"
#include "Direct3D11.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace D3D11;
using namespace DirectX;


/*!
	@brief	コンストラクタ
	@detail	生成時にITextureのコンストラクタを呼び出しデフォルト値を入れる
*/
API::Texture::Texture() :ITexture()
{

}

/*!
	@brief	デストラクタ
	@detail	Finalize呼び出し
*/
API::Texture::~Texture() 
{
	Finalize();
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@param[in]	画像のパス
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath)
{
	return Initialize(filePath, m_eTileMode, m_eFilterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@param[in]	画像のパス
	@param[in]	画像サイズ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const DirectX::XMINT2 size)
{
	return Initialize(filePath, size, m_eTileMode, m_eFilterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みとサンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	タイリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const TileMode tileMode)
{
	return Initialize(filePath, tileMode, m_eFilterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	画像サイズ
	@param[in]	タイリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const DirectX::XMINT2 size, const TileMode tileMode)
{
	return Initialize(filePath, size, tileMode, m_eFilterMode);
}


/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const FilteringMode filterMode)
{
	return Initialize(filePath, m_eTileMode, filterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	画像サイズ
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const DirectX::XMINT2 size, const FilteringMode filterMode)
{
	return Initialize(filePath, size, m_eTileMode, filterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	タイリングモードの設定
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const TileMode tileMode, const FilteringMode filterMode)
{
	HRESULT hr;

	/*! 画像のロード */
	hr = Load(filePath);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture!";
		ErrorLog(error);
		return E_FAIL;
	}

	/*! タイリングモード */
	m_eFilterMode = filterMode;

	/*! フィルタリングモード */
	m_eTileMode = tileMode;

	/*! タイリングとフィルタリングを設定し、サンプラーステートを作成 */
	hr = SetTileAndFiltering(m_eTileMode, m_eFilterMode);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture atlas!\n";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	画像サイズ
	@param[in]	タイリングモードの設定
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Texture::Initialize(std::string filePath, const DirectX::XMINT2 size, const TileMode tileMode, const FilteringMode filterMode)
{
	HRESULT hr;

	/*! 初期化 */
	hr = Initialize(filePath, tileMode, filterMode);
	if (FAILED(hr)) {
		return hr;
	}

	/*! サイズの設定 */
	hr = SetSize(size) ? S_OK : E_FAIL;
	return hr;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な開放
*/
void API::Texture::Finalize()
{
	/*! 抽象クラスのメンバの破棄 */
	ITexture::Finalize();
}
