#include "stdafx.h"
#include "Direct3D11.h"
#include "MeshShader.h"
#include "Mesh.h"
#include "MyGame.h"
#include <wrl/client.h>

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	using�f�B���N�e�B�u
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
	//	�p�X
	auto path = m_Directory + c_HLSL.data();

	//	�u���u
	ComPtr<ID3DBlob>pBlob;

	//	�n���h��
	HRESULT hr;


	//	�t�@�C���𒸓_�V�F�[�_�[�p�ɃR���p�C��
	hr = DynamicCompile(path, c_VSEntryName.data(), c_VSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" for vertex shader is failed dynamic compilation");
		return E_FAIL;
	}

	//	���_�V�F�[�_�[�쐬
	hr = CreateVertexShader(pBlob.Get(), m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" is failed create vertex shader");
		return E_FAIL;
	}

	//	���_���C�A�E�g�쐬
	hr = CreateInputLayout(pBlob.Get());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" input layout is not create!");
		return E_FAIL;
	}

	//	�u���u�̊J��
	pBlob->Release();
	pBlob.Reset();

	//	�t�@�C�����s�N�Z���V�F�[�_�[�p�ɃR���p�C��
	hr = DynamicCompile(path, c_PSEntryName.data(), c_PSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" for pixel shader is failed dynamic compilation");
		return E_FAIL;
	}
	//	�s�N�Z���V�F�[�_�[�쐬
	hr = CreatePixelShader(pBlob.Get(), m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"MeshShader\" is failed create pixel shader");
	}

	//	�u���u�̊J��
	pBlob->Release();
	pBlob.Reset();

	//	�R���X�^���g�o�b�t�@�쐬
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
	@brief	�R���X�^���g�o�b�t�@�̍쐬
	@return	S_OK:���� E_FAIL:���s
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

HRESULT D3D11::Graphic::MeshShader::CreateInputLayout(ID3DBlob * pBlob)
{
	// ���_�C���v�b�g���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	//	���_���C�A�E�g�̗v�f��
	uint32_t numElements = sizeof(layout) / sizeof(*layout);
	return Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
		layout,
		numElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		m_pVertexLayout.GetAddressOf()
	);
}
