#include "stdafx.h"
#include "SpriteShader.h"
#include <wrl/client.h>
#include "MyGame.h"
#include "Direct3D11.h"
#include "SpriteConstantBuffer.h"
#include "SpriteCompVS.h"
#include "SpriteCompPS.h"

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

/*!
	@brief	�R���X�g���N�^
*/
D3D11::Graphic::SpriteShader::SpriteShader()
	: AbstractShader()
	, m_Directory("")
{
}

/*!
	@brief	�f�X�g���N�^
*/
D3D11::Graphic::SpriteShader::~SpriteShader()
{
}

/*!
	@fn		Setup
	@brief	�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p���ăZ�b�g�A�b�v���s��
	@detail	�I�[�o�[���C�h
*/
HRESULT D3D11::Graphic::SpriteShader::Setup()
{
	//	�n���h��
	HRESULT hr;

	//	�f�o�C�X
	auto device = Direct3D11::GetInstance().GetDevice();

	//	���_�V�F�[�_�[
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

	//	���_���C�A�E�g
	hr = CreateInputLayout();
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" input layout is not create!");
		return E_FAIL;
	}

	//	�s�N�Z���V�F�[�_�[
	hr = device->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create pixel shader");
	}

	//	�R���X�^���g�o�b�t�@�쐬
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
	@brief	���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
	@detail	�I�[�o�[���C�h
*/
HRESULT D3D11::Graphic::SpriteShader::DynamicSetup()
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
		ErrorLog("\"SpriteShader\" for vertex shader is failed dynamic compilation");
		return E_FAIL;
	}

	//	���_�V�F�[�_�[�쐬
	hr = CreateVertexShader(pBlob.Get(), m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create vertex shader");
		return E_FAIL;
	}

	//	���_���C�A�E�g�쐬
	hr = CreateInputLayout(pBlob.Get());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" input layout is not create!");
		return E_FAIL;
	}

	//	�u���u�̊J��
	pBlob->Release();
	pBlob.Reset();

	//	�t�@�C�����s�N�Z���V�F�[�_�[�p�ɃR���p�C��
	hr = DynamicCompile(path, c_PSEntryName.data(), c_PSProfile.data(), pBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" for pixel shader is failed dynamic compilation");
		return E_FAIL;
	}
	//	�s�N�Z���V�F�[�_�[�쐬
	hr = CreatePixelShader(pBlob.Get(), m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog("\"SpriteShader\" is failed create pixel shader");
	}

	//	�u���u�̊J��
	pBlob->Release();
	pBlob.Reset();

	//	�R���X�^���g�o�b�t�@�쐬
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
	@brief		�t�@�C���̊K�w���s���A���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
	@detail		�I�[�o�[���[�h
	@param[in]	�t�@�C���̊K�w�p�X
	@NOTE		�V�F�[�_�[�t�@�C���̔z�u�ʒu���l�ɂ���ĕς���Ă��܂����߁A�f�B���N�g�������ł��ݒ�o����悤�ɂ���
*/

HRESULT D3D11::Graphic::SpriteShader::DynamicSetup(std::string& directory)
{
	//	�f�B���N�g���ݒ�
	SetupDirectory(directory);

	//	���I�R���p�C��
	return DynamicSetup();
}

/*!
	@fn			SetupDirectory
	@brief		�t�@�C�����Q�Ƃ���f�B���N�g����ݒ�
	@param[in]	�Q�Ƃ���f�B���N�g��
*/
void D3D11::Graphic::SpriteShader::SetupDirectory(std::string& path)
{
	m_Directory = path;
}

/*!
	@fn		CreateConstantBuffer
	@brief	�R���X�^���g�o�b�t�@�̍쐬
	@return	S_OK:���� E_FAIL:���s
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
	@brief		���_���C�A�E�g�̍쐬
	@param[in]	�R���p�C���ς݃u���u
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT D3D11::Graphic::SpriteShader::CreateInputLayout(ID3DBlob*pBlob)
{
	// ���_�C���v�b�g���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

/*!
	@fn			CreateInputLayout
	@brief		���_���C�A�E�g�̍쐬
	@return		S_OK:���� E_FAIL:���s
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
