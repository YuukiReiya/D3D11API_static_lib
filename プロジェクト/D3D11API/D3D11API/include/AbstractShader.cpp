/*
	@file	AbstractShader.cpp
	@date	2019/04/05
	@author	番場 宥輝
	@brief	シェーダーの抽象クラス
	@note	描画クラス設計見直しに際し、更新していく
*/
#include "stdafx.h"
#pragma comment(lib,"d3dCompiler.lib")
#include <d3dcompiler.h>
#include <wrl/client.h>
#include "MyGame.h"
#include "AbstractShader.h"
#include "Direct3D11.h"

HRESULT D3D11::Graphic::AbstractShader::Setup()
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob>pCompileBlob = nullptr;


	return E_NOTIMPL;
}


HRESULT D3D11::Graphic::AbstractShader::PreCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob ** ppBlob)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob>pError = nullptr;

	//	読み込みパスはマルチバイト文字
	auto wPath = const_cast<LPCWSTR>(To_WString(path).c_str());

	//	コンパイルオプション
	DWORD compileOption = D3DCOMPILE_SKIP_VALIDATION;
#if defined DEBUG||_DEBUG
	compileOption = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_VALIDATION;
#endif

	//	コンパイル
	hr = D3DCompileFromFile(
		wPath,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(),
		profileName.c_str(),
		compileOption,
		NULL,
		ppBlob,
		pError.GetAddressOf()
	);

	//	エラー処理
	if (FAILED(hr)) {
		std::string bufferError = static_cast<char*>(pError->GetBufferPointer());
		std::string error = "\"" + path + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}

	//	明示的開放
	if (pError != nullptr) {
		pError->Release();
		pError = nullptr;
	}

	return S_OK;
}

HRESULT D3D11::Graphic::AbstractShader::DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob ** ppBlob)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob>pError = nullptr;

	//	読み込みパスはマルチバイト文字
	auto wPath = const_cast<LPCWSTR>(To_WString(path).c_str());

	//	コンパイルオプション
	DWORD compileOption = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined DEBUG||_DEBUG
	compileOption = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	//	コンパイル
	hr = D3DCompileFromFile(
		wPath,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(),
		profileName.c_str(),
		compileOption,
		NULL,
		ppBlob,
		pError.GetAddressOf()
	);

	//	エラー処理
	if (FAILED(hr)) {
		std::string bufferError = static_cast<char*>(pError->GetBufferPointer());
		std::string error = "\"" + path + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}

	//	明示的開放
	if (pError != nullptr) {
		pError->Release();
		pError = nullptr;
	}

	return S_OK;
}

HRESULT D3D11::Graphic::AbstractShader::CreateVertexShader(ID3DBlob* pBlob, ID3D11VertexShader * pVertexShader)
{
	HRESULT hr;
	//TODO:ブロブのコンパイル判定を行う

	//
	hr=D3D11::Direct3D11::GetInstance().GetDevice()->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		NULL,
		&pVertexShader
	);
	if (FAILED(hr)) {
		std::string error = "Vertex Shader generation failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT D3D11::Graphic::AbstractShader::CreatePixelShader(ID3DBlob * pBlob, ID3D11PixelShader * pPixelShader)
{
	HRESULT hr;
	//TODO:ブロブのコンパイル判定を行う

	//
	hr = D3D11::Direct3D11::GetInstance().GetDevice()->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		NULL,
		&pPixelShader
	);
	if (FAILED(hr)) {
		std::string error = "Pixel Shader generation failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

