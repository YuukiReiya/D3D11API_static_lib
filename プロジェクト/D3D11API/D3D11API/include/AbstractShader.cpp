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

/*!
	@brief	�R���X�g���N�^
	@detail	�C���X�^���X�������ɃC���X�^���X�̃A�h���X���V�F�A�[�h�E�|�C���^�ɕێ�������
*/
D3D11::Graphic::AbstractShader::AbstractShader()
{
	m_pShared = std::make_shared<AbstractShader*>(this);
}

/*!
	@fn			PreCompile
	@brief		�v���R���p�C���ς݃t�@�C����p�����V�F�[�_�[�t�@�C���̃R���p�C��
	@param[in]	�V�F�[�_�[�t�@�C���̃p�X
	@param[in]	�G���g���[�|�C���g��
	@param[in]	�V�F�[�_�[�̃v���t�@�C���� ex)vs_5_0
	@param[in]	�R���p�C���p�̃u���u
	@return		S_OK:���� E_FAIL:���s
*/
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

/*!
	@fn		DynamicCompile
	@brief	�V�F�[�_�[�t�@�C���̓��I�R���p�C��
	@param[in]	�V�F�[�_�[�t�@�C���̃p�X
	@param[in]	�G���g���[�|�C���g��
	@param[in]	�V�F�[�_�[�̃v���t�@�C���� ex)vs_5_0
	@param[in]	�R���p�C���p�̃u���u
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob ** ppBlob)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob>pError = nullptr;

	//	�ǂݍ��݃p�X�̓}���`�o�C�g����
	auto cast = To_WString(path);
	auto wPath = const_cast<LPCWSTR>(cast.c_str());

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

/*!
	@fn			CreateVertexShader
	@brief		���_�V�F�[�_�[�̍쐬
	@detail		���O�Ƀu���u���R���p�C�����Ă������ƁI
	@param[in]	�R���p�C���ς݃u���u
	@param[in]	���_�V�F�[�_�[
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::CreateVertexShader(ID3DBlob* pBlob, ID3D11VertexShader** pVertexShader)
{
	HRESULT hr;
	//TODO:�u���u�̃R���p�C��������s��

	//
	hr=D3D11::Direct3D11::GetInstance().GetDevice()->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		NULL,
		pVertexShader
	);
	if (FAILED(hr)) {
		std::string error = "Vertex Shader generation failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn			CreateVertexShader
	@brief		���_�V�F�[�_�[�̍쐬
	@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
	@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
	@param[in]	���_�V�F�[�_�[
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::CreateVertexShader(CustomShaderBin* bin, ID3D11VertexShader** pVertexShader)
{
	return Direct3D11::GetInstance().GetDevice()->CreateVertexShader(
		bin->GetPtr(),
		bin->GetSize(),
		NULL,
		pVertexShader
	);
}

/*!
	@fn			CreateInputLayout
	@brief		���_���C�A�E�g�쐬
	@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
	@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
	@param[in]	���_���C�A�E�g�̒�`
	@param[in]	���_���C�A�E�g
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::CreateInputLayout(CustomShaderBin * bin, D3D11_INPUT_ELEMENT_DESC desc[], ID3D11InputLayout ** pInputLayout)
{
	uint32_t numElements = sizeof(desc) / sizeof(*desc);
	return Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
		desc,
		numElements,
		bin->GetPtr(),
		bin->GetSize(),
		pInputLayout
	);
}
/*!
	@fn			CreatePixelShader
	@brief		�s�N�Z���V�F�[�_�[�̍쐬
	@detail		���O�Ƀu���u���R���p�C�����Ă������ƁI
	@param[in]	�R���p�C���ς݃u���u
	@param[in]	�s�N�Z���V�F�[�_�[
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::CreatePixelShader(ID3DBlob * pBlob, ID3D11PixelShader** pPixelShader)
{
	HRESULT hr;
	//TODO:�u���u�̃R���p�C��������s��

	//
	hr = D3D11::Direct3D11::GetInstance().GetDevice()->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		NULL,
		pPixelShader
	);
	if (FAILED(hr)) {
		std::string error = "Pixel Shader generation failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn			CreatePixelShader
	@brief		�s�N�Z���V�F�[�_�[�̍쐬
	@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
	@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
	@param[in]	�s�N�Z���V�F�[�_�[
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::AbstractShader::CreatePixelShader(CustomShaderBin * bin, ID3D11PixelShader ** pPixelShader)
{
	return Direct3D11::GetInstance().GetDevice()->CreatePixelShader(
		bin->GetPtr(),
		bin->GetSize(),
		NULL,
		pPixelShader
	);
}

