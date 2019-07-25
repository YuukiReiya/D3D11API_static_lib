#include "stdafx.h"
#include "SpriteShader.h"
#include <wrl/client.h>
#include "MyGame.h"
#include "Direct3D11.h"
#include "SpriteConstantBuffer.h"
#include "SpriteCompVS.h"
#include "SpriteCompPS.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	usingディレクティブ
	@using	Microsoft::WRL
*/
using namespace Microsoft::WRL;

/*!
	@brief	コンストラクタ
*/
D3D11::Graphic::SpriteShader::SpriteShader()
	: AbstractShader()
	, m_Directory("")
{
}

/*!
	@brief	デストラクタ
*/
D3D11::Graphic::SpriteShader::~SpriteShader()
{
}

/*!
	@fn		Setup
	@brief	プロコンパイル済みシェーダーファイルを利用してセットアップを行う
	@detail	オーバーライド
*/
HRESULT D3D11::Graphic::SpriteShader::Setup()
{
	//	ハンドラ
	HRESULT hr;

	//	デバイス
	auto device = Direct3D11::GetInstance().GetDevice();

	//	頂点シェーダー
	hr = device->CreateVertexShader(
		&g_vs_main,
		sizeof(g_vs_main),
		NULL,
		m_pVertexShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create vertex shader");
		return E_FAIL;
	}

	//	頂点レイアウト
	hr = CreateInputLayout();
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" input layout is not create!");
		return E_FAIL;
	}

	//	ピクセルシェーダー
	hr = device->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create pixel shader");
	}

	//	コンスタントバッファ作成
	hr = CreateConstantBuffer();
	if (FAILED(hr)) {
		std::string error = "\"SpriteShader\" ConstantBuffer is not create!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn		DynamicSetup
	@brief	動的コンパイルを利用したセットアップを行う
	@detail	オーバーライド
*/
HRESULT D3D11::Graphic::SpriteShader::DynamicSetup()
{
	//	パス
	auto path = m_Directory + c_HLSL.data();

	//	ブロブ
	ComPtr<ID3DBlob>pBlob;

	//	ハンドラ
	HRESULT hr;


	//	ファイルを頂点シェーダー用にコンパイル
	hr = DynamicCompile(path, c_VSEntryName.data(), c_VSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" for vertex shader is failed dynamic compilation");
		return E_FAIL;
	}

	//	頂点シェーダー作成
	hr = CreateVertexShader(pBlob.Get(), m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create vertex shader");
		return E_FAIL;
	}

	//	頂点レイアウト作成
	hr = CreateInputLayout(pBlob.Get());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" input layout is not create!");
		return E_FAIL;
	}

	//	ブロブの開放
	pBlob->Release();
	pBlob.Reset();

	//	ファイルをピクセルシェーダー用にコンパイル
	hr = DynamicCompile(path, c_PSEntryName.data(), c_PSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" for pixel shader is failed dynamic compilation");
		return E_FAIL;
	}
	//	ピクセルシェーダー作成
	hr = CreatePixelShader(pBlob.Get(), m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create pixel shader");
	}

	//	ブロブの開放
	pBlob->Release();
	pBlob.Reset();

	//	コンスタントバッファ作成
	hr = CreateConstantBuffer();
	if (FAILED(hr)) {
		std::string error = "\"SpriteShader\" ConstantBuffer is not create!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn			DynamicSetup
	@brief		ファイルの階層を行い、動的コンパイルを利用したセットアップを行う
	@detail		オーバーロード
	@param[in]	ファイルの階層パス
	@NOTE		シェーダーファイルの配置位置が人によって変わってしまうため、ディレクトリだけでも設定出来るようにする
*/

HRESULT D3D11::Graphic::SpriteShader::DynamicSetup(std::string& directory)
{
	//	ディレクトリ設定
	SetupDirectory(directory);

	//	動的コンパイル
	return DynamicSetup();
}

/*!
	@fn			SetupDirectory
	@brief		ファイルを参照するディレクトリを設定
	@param[in]	参照するディレクトリ
*/
void D3D11::Graphic::SpriteShader::SetupDirectory(std::string& path)
{
	m_Directory = path;
}

/*!
	@fn		CreateConstantBuffer
	@brief	コンスタントバッファの作成
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT D3D11::Graphic::SpriteShader::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(Sprite::ConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, m_pConstantBuffer.GetAddressOf());
}

/*!
	@fn			CreateInputLayout
	@brief		頂点レイアウトの作成
	@param[in]	コンパイル済みブロブ
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT D3D11::Graphic::SpriteShader::CreateInputLayout(ID3DBlob*pBlob)
{
	// 頂点インプットレイアウト定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	//	頂点レイアウトの要素数
	uint32_t numElements = sizeof(layout) / sizeof(*layout);
	return Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
		layout,
		numElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		m_pVertexLayout.GetAddressOf()
	);
}

/*!
	@fn			CreateInputLayout
	@brief		頂点レイアウトの作成
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT D3D11::Graphic::SpriteShader::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	return Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
		//c_InputLayout,
		layout,
		GetArraySize(layout),
		//GetArraySize(c_InputLayout),
		g_vs_main,
		sizeof(g_vs_main),
		m_pVertexLayout.GetAddressOf()
	);
}
