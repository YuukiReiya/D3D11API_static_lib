#include "stdafx.h"
#include "Direct3D11.h"
#include "MeshShader.h"
#include "MeshConstantBuffer.h"
#include "Mesh.h"
#include "MyGame.h"
#include <wrl/client.h>
#include "MeshCompVS.h"
#include "MeshCompPS.h"

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
MeshShader::MeshShader()
	:AbstractShader()
	, m_Directory("")
{
}

/*!
	@brief	デストラクタ
*/
MeshShader::~MeshShader()
{
}

/*!
	@fn		Setup
	@brief	プリコンパイル済みシェーダーファイルを利用してセットアップを行う
	@detail	オーバーライド
*/
HRESULT D3D11::Graphic::MeshShader::Setup()
{
	HRESULT hr;
	auto& dev = Direct3D11::GetInstance();
	//	頂点レイアウト
	{
		D3D11_INPUT_ELEMENT_DESC desc[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	{ "COLOR",	 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		hr = dev.GetDevice()->CreateInputLayout(
			desc,
			GetArraySize(desc),
			g_vs_main,
			sizeof(g_vs_main),
			m_pVertexLayout.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("\"MeshShader\" input layout is not create!");
			return E_FAIL;
		}
	}

	//	頂点シェーダー
	{
		hr = dev.GetDevice()->CreateVertexShader(
			&g_vs_main,
			sizeof(g_vs_main),
			NULL,
			m_pVertexShader.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("\"MeshShader\" is failed create vertex shader");
			return E_FAIL;
		}

	}

	//	ピクセルシェーダー
	{
		hr = dev.GetDevice()->CreatePixelShader(
			&g_ps_main,
			sizeof(g_ps_main),
			NULL,
			m_pPixelShader.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("\"MeshShader\" is failed create pixel shader");
			return E_FAIL;
		}


	}

	//	コンスタントバッファ
	{
		hr = CreateConstantBuffer();
		if (FAILED(hr)) {
			std::string error = "\"MeshShader\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}
	}
	return S_OK;
}

/*!
	@fn		DynamicSetup
	@brief	動的コンパイルを利用したセットアップを行う
	@detail	オーバーライド
*/
HRESULT D3D11::Graphic::MeshShader::DynamicSetup()
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
		ErrorLog("\"MeshShader\" for vertex shader is failed dynamic compilation");
		return E_FAIL;
	}

	//	頂点シェーダー作成
	hr = CreateVertexShader(pBlob.Get(), m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" is failed create vertex shader");
		return E_FAIL;
	}

	//	頂点レイアウト作成
	hr = CreateInputLayout(pBlob.Get());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" input layout is not create!");
		return E_FAIL;
	}

	//	ブロブの開放
	pBlob->Release();
	pBlob.Reset();

	//	ファイルをピクセルシェーダー用にコンパイル
	hr = DynamicCompile(path, c_PSEntryName.data(), c_PSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" for pixel shader is failed dynamic compilation");
		return E_FAIL;
	}
	//	ピクセルシェーダー作成
	hr = CreatePixelShader(pBlob.Get(), m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" is failed create pixel shader");
		return E_FAIL;
	}

	//	ブロブの開放
	pBlob->Release();
	pBlob.Reset();

	//	コンスタントバッファ作成
	hr = CreateConstantBuffer();
	if (FAILED(hr)) {
		std::string error = "\"MeshShader\" ConstantBuffer is not create!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn		CreateConstantBuffer
	@brief	コンスタントバッファの作成
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT D3D11::Graphic::MeshShader::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC pcb;
	pcb.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	pcb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	pcb.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	pcb.MiscFlags = 0;
	pcb.StructureByteStride = 0;
	pcb.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&pcb,
		NULL,
		m_pConstantBuffer.GetAddressOf()
	);
}

/*!
	@fn			CreateInputLayout
	@brief		頂点レイアウトの作成
	@param[in]	コンパイル済みブロブ
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT D3D11::Graphic::MeshShader::CreateInputLayout(ID3DBlob * pBlob)
{
	// 頂点インプットレイアウト定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "COLOR",	 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
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
