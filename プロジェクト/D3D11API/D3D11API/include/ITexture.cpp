#include "stdafx.h"
#include "ITexture.h"
#include "WICTextureLoader.h"
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
	@var	c_DefaultSize
	@brief	デフォルトの画像サイズ
	@detail	デフォルトで与えられている画像サイズ
			100×100をデフォルトの画像サイズに指定
*/
const DirectX::XMINT2 API::ITexture::c_DefaultSize = { 100,100 };

/*!
	@brief	コンストラクタ
	@detail	メンバ変数の初期化を行う
*/
API::ITexture::ITexture() :
	m_pShaderResourceView(nullptr),
	m_pSamplerState(nullptr),
	m_eTileMode(TileMode::Clamp),
	m_eFilterMode(FilteringMode::Trilinear),
	m_Size(c_DefaultSize)
{
}

/*!
	@brief	仮想デストラクタ
	@detail	空デストラクタ
*/
API::ITexture::~ITexture()
{
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な開放
			派生先でメンバが増えることを踏まえ、仮想関数として実装

*/
void API::ITexture::Finalize()
{
	//m_pShaderResourceView.Reset();
	//m_pSamplerState.Reset();
}

/*!
	@fn			画像サイズのセッター
	@brief		読み込む画像のサイズの設定
	@param[in]	画像サイズ
	@return		成功:true 失敗:false
*/
bool API::ITexture::SetSize(const DirectX::XMINT2 size)
{
#ifdef DEBUG_TEXTURE
	/*!
	*	画像サイズは頂点生成に使用する。
	*	負数で生成した場合、UVが反転するのだが
	*	今回は反転して描画するための関数を用意するため
	*	サイズで0以下が指定した場合にエラーを吐くようにする。
	*/
	try
	{
		std::string error;

		// 画像の幅をチェック
		if (size.x <= 0) {
			error = "texture size.x is less than Zero!\n\
			size.x =" + std::to_string(size.x) + " <= 0";
			throw error;
		}

		// 画像の高さをチェック
		if (size.y <= 0) {
			error = "texture size.x is less than Zero!\n\
			size.x =" + std::to_string(size.x) + " <= 0";
			throw error;
		}
	}
	catch (std::string& error)
	{
		ErrorLog(error);
		return false;
	}
#endif
	m_Size = size;
	return true;
}

/*!
	@fn			タイリングモードとフィルタリングモードのセッター
	@brief		タイリングモードとフィルタリングモードの設定
	@param[in]	設定するタイルモード
	@param[in]	設定するフィルタリング(アドレッシングモード)
	@detail		サンプラーステートを作成
*/
HRESULT API::ITexture::SetTileAndFiltering(const TileMode tileMode, const FilteringMode filterMode)
{
	// 引数の保持
	m_eTileMode = tileMode;
	m_eFilterMode = filterMode;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = static_cast<D3D11_FILTER>(m_eFilterMode);				// フィルタリング
	sd.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// アドレッシングモード
	sd.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// アドレッシングモード
	sd.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// アドレッシングモード

	HRESULT hr;
	// サンプラー作成
	hr = Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
		&sd,
		m_pSamplerState.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "Sampler state is not created!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;

}

/*!
	@fn			画像の読み込み
	@brief		画像の読み込みを行いSRVを作成
	@detail		画像データからSRVを作成し、メンバ変数にバインド
	@param[in]	読み込むファイルのパス
	@return		読み取り結果 成功:S_OK  失敗:E_FAIL
*/
HRESULT API::ITexture::Load(std::string filePath)
{
	HRESULT hr;

	/*! 
		D3D11CompileFromFileの引数はマルチバイトなので
		受け取ったパスをマルチバイトに変換
	*/
	auto cast = To_WString(filePath);
	auto path = const_cast<LPCWSTR>(cast.c_str());

	//	ローカル変数
	Microsoft::WRL::ComPtr<ID3D11Resource> pResource = nullptr;

	// テクスチャ作成
	hr = CreateWICTextureFromFile(
		Direct3D11::GetInstance().GetDevice(),
		path,
		pResource.GetAddressOf(),
		m_pShaderResourceView.GetAddressOf()
	);

	// ローカル変数のメモリ開放
	if (pResource.Get() != nullptr) {
		pResource.Reset();
	}
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture!";
		ErrorLog(error);
		return E_FAIL;
	}

	// 読み込みの正常終了
	return S_OK;
}
