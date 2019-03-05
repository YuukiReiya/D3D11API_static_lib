#include "stdafx.h"
#include "TextureAtlas.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace API;
using namespace std;

/*!
	@brief	コンストラクタ
	@detail	生成時にITextureのコンストラクタを呼び出しデフォルト値を入れる
			分割数は描画前に設定されていることが前提なのでデフォルトでは参照外を設定しておく。
			逆にインデックスは指定なしでも描画できるように分割したした際の最小値を設定しておく
*/
TextureAtlas::TextureAtlas() :ITexture(), m_DivNum({ -1,-1 }), m_Index({ 0,0 })
{
}

/*!
	@brief	デストラクタ
	@detail	Finalize呼び出し
*/
TextureAtlas::~TextureAtlas()
{
	Finalize();
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@param[in]	読み込む画像のパス
	@param[in]	分割数(x,y)
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum)
{
	return Initialize(filePath, divNum, m_eTileMode, m_eFilterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	表示する画像サイズ
				※分割前の画像サイズでないことに注意
	@param[in]	分割数(x,y)
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum)
{
	HRESULT hr;

	/*! 初期化 */
	hr = Initialize(filePath, divNum);
	if (FAILED(hr)) {
		return hr;
	}

	/*! サイズの設定 */
	hr = SetSize(size) ? S_OK : E_FAIL;
	return hr;
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	分割数(x,y)
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum, const FilteringMode filterMode)
{
	return Initialize(filePath, divNum, m_eTileMode, filterMode);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	表示する画像サイズ
				※分割前の画像サイズでないことに注意
	@param[in]	分割数(x,y)
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum, const FilteringMode filterMode)
{
	HRESULT hr;

	/*! 初期化 */
	hr = Initialize(filePath, divNum, filterMode);
	if (FAILED(hr)) {
		return hr;
	}

	/*! サイズの設定 */
	hr = SetSize(size) ? S_OK : E_FAIL;
	return hr;
}

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
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMFLOAT2 divNum, const TileMode tileMode, const FilteringMode filterMode)
{
	HRESULT hr;

	/*! 画像のロード */
	hr = Load(filePath);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture atlas!";
		ErrorLog(error);
		return E_FAIL;
	}

	/*! タイリングモード */
	m_eFilterMode	= filterMode;

	/*! フィルタリングモード */
	m_eTileMode		= tileMode;

	/*! タイリングとフィルタリングを設定し、サンプラーステートを作成 */
	hr = SetTileAndFiltering(m_eTileMode, m_eFilterMode);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture atlas!\n";
		ErrorLog(error);
		return E_FAIL;
	}

	/*! 分割数の設定 */
	m_DivNum = divNum;

	return S_OK;
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		画像の読み込みと分割数の指定を行い、サンプラーステートの作成を行う
	@param[in]	画像のパス
	@param[in]	表示する画像サイズ
				※分割前の画像サイズでないことに注意
	@param[in]	分割数(x,y)
	@param[in]	タイリングモードの設定
	@param[in]	フィルタリングモードの設定
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::TextureAtlas::Initialize(std::string filePath, const DirectX::XMINT2 size, const DirectX::XMFLOAT2 divNum, const TileMode tileMode, const FilteringMode filterMode)
{
	HRESULT hr;

	/*! 画像のロード */
	hr = Load(filePath);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture atlas!";
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

	/*! 分割数の設定 */
	m_DivNum = divNum;

	/*! サイズの設定 */
	hr = SetSize(size) ? S_OK : E_FAIL;

	return hr;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な開放
*/
void API::TextureAtlas::Finalize()
{
	/*! 抽象クラスのメンバの破棄 */
	ITexture::Finalize();
}

/*!
	@fn			分割数のセット
	@brief		アトラステクスチャの分割数をxとy方向にどれだけ分割するか指定
	@param[in]	分割数(x:x方向の個数,y:y方向の個数)
*/
void API::TextureAtlas::SetDevNum(const DirectX::XMFLOAT2 divNum)
{
#ifdef DEBUG_TEXTURE
	try
	{
		std::string error;

		/*! xの範囲外チェック */
		if (divNum.x <= 0) {
			error = "divNum.x is less than Zero!\n\
			divNum.x = " + std::to_string(divNum.x) + " <= 0";
			throw error;/*!< xの文化通数が0以下 */
		}

		/*! yの範囲外チェック */
		if (divNum.y <= 0) {
			error = "divNum.y is less than Zero!\n\
			divNum.y = " + std::to_string(divNum.y) + " <= 0";
			throw error;/*!< yの文化通数が0以下 */
		}

	}
	catch (std::string error)
	{
		ErrorLog(error);
	}
#endif
	m_DivNum = divNum;
}

/*!
	@fn			描画インデックスのセッター
	@brief		描画するアトラステクスチャのインデックスを設定。
	@detail		アトラステクスチャの分割数より多い値は設定できません。
	@param[in]	描画したいテクスチャ番号
*/
void API::TextureAtlas::SetAtlasIndex(const DirectX::XMFLOAT2 index)
{
#ifdef DEBUG_TEXTURE
	std::string error;
	try
	{
		/*! xの範囲外チェック */
		if (index.x < 0) {
			error = "index.x is out-of-range\n\
			index.x = " + std::to_string(index.x) + "\n" +
			std::to_string(index.x) + " < 0";
			throw error;
		}

		/*! xの範囲外チェック */
		if (m_DivNum.x <= index.x) {
			error = "index.x is out-of-range\n\
			index.x = " + std::to_string(index.x) + "\n" +
			std::to_string(index.x) + " < DivNum.x-1 = " + std::to_string(m_DivNum.x - 1);
			throw error;
		}

		/*! yの範囲外チェック */
		if (index.y < 0) {
			error = "index.y is out-of-range\n\
			index.y = " + std::to_string(index.y) + "\n" +
			std::to_string(index.y) + " < 0";
			throw error;
		}

		/*! yの範囲外チェック */
		if (m_DivNum.y <= index.y) {
			error = "index.y is out-of-range\n\
			index.y = " + std::to_string(index.y) + "\n" +
			std::to_string(index.y) + " < DivNum.y-1 = " + std::to_string(m_DivNum.y - 1);
			throw error;
		}

	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return;
	}
#endif // DEBUG_TEXTURE
	m_Index = index;
}
