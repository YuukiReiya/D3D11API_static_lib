#include "stdafx.h"
#include "Direct3D11.h"
#include "MeshShader.h"
#include "Mesh.h"
#include "MyGame.h"
#include <wrl/client.h>

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

MeshShader::MeshShader()
	:AbstractShader()
	, m_Directory("")
{
}


MeshShader::~MeshShader()
{
}

HRESULT D3D11::Graphic::MeshShader::Setup()
{
	return E_FAIL;
}

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
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(MeshShaderBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, m_pConstantBuffer.GetAddressOf());
}
