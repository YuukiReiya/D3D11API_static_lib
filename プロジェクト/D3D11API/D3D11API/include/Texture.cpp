/*
	@file	Texture.cpp
	@date	2019/06/06
	@author	番場 宥輝
	@brief	テクスチャ
*/
#include "stdafx.h"
#include "Texture.h"
#include "Direct3D11.h"
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "WICTextureLoader.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	コンストラクタ
	@detail	生成時にITextureのコンストラクタを呼び出しデフォルト値を入れる
*/
API::Texture::Texture() :
	m_pSamplerState(nullptr),
	m_pShaderResourceView(nullptr)
{
	m_DivCount = { 1,1 };
	m_AtlasOffset = { 0,0 };
	//	宣言時に生ポインタで宣言されたときの対策でクラスメンバにシェアード・ポインタを持たせる
	m_pShared = std::make_shared<Texture*>(this);
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
	@fn			Initialize
	@brief		テクスチャの初期化
	@detail		サンプラーステートとSRVの作成を同時に行う
				デフォルト引数で一番メジャーな設定を渡している
	@param[in]	読み込むテクスチャのパス
	@param[in]	設定するフィルタリング(アドレッシングモード)
	@param[in]	設定するタイルモード
	@return		S_OK:成功	E_FAIL:失敗
*/
HRESULT API::Texture::Initialize(std::string filePath, FilteringMode fMode, TileMode tMode)
{
	try
	{
		if (FAILED(SetupShaderResourceView(filePath))) { throw std::runtime_error("SetupShaderResourceView"); }
		if (FAILED(SetupSamplerState(fMode, tMode))) { throw std::runtime_error("SetupSamplerState"); }
	}
	catch (const std::exception&e)
	{
		std::string error = "Failed to initialize texture.\n";
		error += "Failed process is \"" + std::string(e.what()) + "\".\n";
		error += "Texture path is \"" + filePath + "\"";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn			Initialize
	@brief		テクスチャの初期化
	@detail		サンプラーステートとSRVの作成を同時に行う
				デフォルト引数で一番メジャーな設定を渡している
	@param[in]	読み込むテクスチャのパス
	@param[in]	テクスチャのサイズ
	@param[in]	設定するフィルタリング(アドレッシングモード)
	@param[in]	設定するタイルモード
	@return		S_OK:成功	E_FAIL:失敗
*/
HRESULT API::Texture::Initialize(std::string filePath, DirectX::XMINT2 size, FilteringMode fMode, TileMode tMode)
{
	m_Size = size;
	return Initialize(filePath, fMode, tMode);
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な開放
*/
void API::Texture::Finalize()
{
	m_pSamplerState.Reset();
	m_pShaderResourceView.Reset();
}

/*!
	@fn			SetupSamplerState
	@brief		サンプラーステートの作成
	@param[in]	設定するフィルタリング(アドレッシングモード)
	@param[in]	設定するタイルモード
	@return		S_OK:成功	E_FAIL:失敗
*/
HRESULT API::Texture::SetupSamplerState(const FilteringMode fMode, const TileMode tMode)
{
	//	設定値の開放
	m_pSamplerState.Reset();

	// 引数の保持
	m_eTileMode = tMode;
	m_eFilterMode = fMode;

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
	@fn			SetupSRV
	@brief		SRVの作成
	@param[in]	読み込むテクスチャのパス
	@return		読み取り結果 S_OK:成功	E_FAIL:失敗
*/
HRESULT API::Texture::SetupShaderResourceView(std::string filePath)
{
	//	設定値の開放
	m_pShaderResourceView.Reset();

	HRESULT hr;

	/*!
		NOTE:D3D11CompileFromFileの引数はマルチバイトなので
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

/*!
	@fn			SetupAtlas
	@brief		アトラス情報のセットアップ
	@detail		デフォルト引数を適用すると既に設定した値を使う
	@param[in]	描画するアトラスのインデックス番号。
				(分割したアトラスの横(x)何番目で縦(y)何番目かを指定)
	@param[in]	横に何個分割するか: -1:設定済みの値を使う
	@param[in]	縦に何個分割するか: -1:設定済みの値を使う
	@note		アトラスを使うだけなのにオフセットと分割数でゲッターとセッターを用意すると手順が増え、
				ユーザビリティの低下が心配だったのでアトラスは一つにまとめた。
				分割数は負の値を指定できないように"unsigned"で指定させる
*/
void API::Texture::SetupAtlas(DirectX::XMINT2 atlasOffset, unsigned int div_x, unsigned int div_y)
{
	try
	{
		//	アトラスのオフセット
		m_AtlasOffset = atlasOffset;
		
		//	分割数
		m_DivCount =
		{
			div_x == c_ConfiguredDevCount ? m_DivCount.x : static_cast<int32_t>(div_x),
			div_y == c_ConfiguredDevCount ? m_DivCount.y : static_cast<int32_t>(div_y),
		};

		if (m_DivCount.x <= m_AtlasOffset.x) { throw std::runtime_error("offset.x = " + std::to_string(m_AtlasOffset.x) + "/ divCount.x = " + std::to_string(m_DivCount.x)); }
		if (m_DivCount.y <= m_AtlasOffset.y) { throw std::runtime_error("offset.y = " + std::to_string(m_AtlasOffset.y) + "/ divCount.y = " + std::to_string(m_DivCount.y)); }
	}
	catch (const std::exception&e)
	{
		std::string error = "Failed to set atlas.\n";
		error += "Offset exceeded the number of divisions.\n";
		error += std::string(e.what());
	}
}
