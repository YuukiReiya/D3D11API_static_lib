/*
	@file	AbstractShader.cpp
	@date	2019/04/05
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�̒��ۃN���X
	@note	�`��N���X�݌v�������ɍۂ��A�X�V���Ă���
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

	//	�ǂݍ��݃p�X�̓}���`�o�C�g����
	auto wPath = const_cast<LPCWSTR>(To_WString(path).c_str());

	//	�R���p�C���I�v�V����
	DWORD compileOption = D3DCOMPILE_SKIP_VALIDATION;
#if defined DEBUG||_DEBUG
	compileOption = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_VALIDATION;
#endif

	//	�R���p�C��
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

	//	�G���[����
	if (FAILED(hr)) {
		std::string bufferError = static_cast<char*>(pError->GetBufferPointer());
		std::string error = "\"" + path + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}

	//	�����I�J��
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

	//	�ǂݍ��݃p�X�̓}���`�o�C�g����
	auto wPath = const_cast<LPCWSTR>(To_WString(path).c_str());

	//	�R���p�C���I�v�V����
	DWORD compileOption = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined DEBUG||_DEBUG
	compileOption = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	//	�R���p�C��
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

	//	�G���[����
	if (FAILED(hr)) {
		std::string bufferError = static_cast<char*>(pError->GetBufferPointer());
		std::string error = "\"" + path + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}

	//	�����I�J��
	if (pError != nullptr) {
		pError->Release();
		pError = nullptr;
	}

	return S_OK;
}

HRESULT D3D11::Graphic::AbstractShader::CreateVertexShader(ID3DBlob* pBlob, ID3D11VertexShader * pVertexShader)
{
	HRESULT hr;
	//TODO:�u���u�̃R���p�C��������s��

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
	//TODO:�u���u�̃R���p�C��������s��

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

