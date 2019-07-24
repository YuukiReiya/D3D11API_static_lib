/*
	@file	Texture.cpp
	@date	2019/06/06
	@author	�ԏ� �G�P
	@brief	�e�N�X�`��
*/
#include "stdafx.h"
#include "Texture.h"
#include "Direct3D11.h"
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "WICTextureLoader.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	�R���X�g���N�^
	@detail	��������ITexture�̃R���X�g���N�^���Ăяo���f�t�H���g�l������
*/
API::Texture::Texture() :
	m_pSamplerState(nullptr),
	m_pShaderResourceView(nullptr)
{
	m_DivCount = { 1,1 };
	m_AtlasOffset = { 0,0 };
	//	�錾���ɐ��|�C���^�Ő錾���ꂽ�Ƃ��̑΍�ŃN���X�����o�ɃV�F�A�[�h�E�|�C���^����������
	m_pShared = std::make_shared<Texture*>(this);
}

/*!
	@brief	�f�X�g���N�^
	@detail	Finalize�Ăяo��
*/
API::Texture::~Texture() 
{
	Finalize();
}

/*!
	@fn			Initialize
	@brief		�e�N�X�`���̏�����
	@detail		�T���v���[�X�e�[�g��SRV�̍쐬�𓯎��ɍs��
				�f�t�H���g�����ň�ԃ��W���[�Ȑݒ��n���Ă���
	@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
	@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
	@param[in]	�ݒ肷��^�C�����[�h
	@return		S_OK:����	E_FAIL:���s
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
	@brief		�e�N�X�`���̏�����
	@detail		�T���v���[�X�e�[�g��SRV�̍쐬�𓯎��ɍs��
				�f�t�H���g�����ň�ԃ��W���[�Ȑݒ��n���Ă���
	@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
	@param[in]	�e�N�X�`���̃T�C�Y
	@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
	@param[in]	�ݒ肷��^�C�����[�h
	@return		S_OK:����	E_FAIL:���s
*/
HRESULT API::Texture::Initialize(std::string filePath, DirectX::XMINT2 size, FilteringMode fMode, TileMode tMode)
{
	m_Size = size;
	return Initialize(filePath, fMode, tMode);
}

/*!
	@fn		�t�@�C�i���C�Y
	@brief	�j������
	@detail	�����o�̖����I�ȊJ��
*/
void API::Texture::Finalize()
{
	m_pSamplerState.Reset();
	m_pShaderResourceView.Reset();
}

/*!
	@fn			SetupSamplerState
	@brief		�T���v���[�X�e�[�g�̍쐬
	@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
	@param[in]	�ݒ肷��^�C�����[�h
	@return		S_OK:����	E_FAIL:���s
*/
HRESULT API::Texture::SetupSamplerState(const FilteringMode fMode, const TileMode tMode)
{
	//	�ݒ�l�̊J��
	m_pSamplerState.Reset();

	// �����̕ێ�
	m_eTileMode = tMode;
	m_eFilterMode = fMode;

	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = static_cast<D3D11_FILTER>(m_eFilterMode);				// �t�B���^�����O
	sd.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// �A�h���b�V���O���[�h
	sd.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// �A�h���b�V���O���[�h
	sd.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	// �A�h���b�V���O���[�h

	HRESULT hr;
	// �T���v���[�쐬
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
	@brief		SRV�̍쐬
	@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
	@return		�ǂݎ�茋�� S_OK:����	E_FAIL:���s
*/
HRESULT API::Texture::SetupShaderResourceView(std::string filePath)
{
	//	�ݒ�l�̊J��
	m_pShaderResourceView.Reset();

	HRESULT hr;

	/*!
		NOTE:D3D11CompileFromFile�̈����̓}���`�o�C�g�Ȃ̂�
			 �󂯎�����p�X���}���`�o�C�g�ɕϊ�
	*/
	auto cast = To_WString(filePath);
	auto path = const_cast<LPCWSTR>(cast.c_str());

	//	���[�J���ϐ�
	Microsoft::WRL::ComPtr<ID3D11Resource> pResource = nullptr;

	// �e�N�X�`���쐬
	hr = CreateWICTextureFromFile(
		Direct3D11::GetInstance().GetDevice(),
		path,
		pResource.GetAddressOf(),
		m_pShaderResourceView.GetAddressOf()
	);

	// ���[�J���ϐ��̃������J��
	if (pResource.Get() != nullptr) {
		pResource.Reset();
	}
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture!";
		ErrorLog(error);
		return E_FAIL;
	}

	// �ǂݍ��݂̐���I��
	return S_OK;
}

/*!
	@fn			SetupAtlas
	@brief		�A�g���X���̃Z�b�g�A�b�v
	@detail		�f�t�H���g������K�p����Ɗ��ɐݒ肵���l���g��
	@param[in]	�`�悷��A�g���X�̃C���f�b�N�X�ԍ��B
				(���������A�g���X�̉�(x)���Ԗڂŏc(y)���Ԗڂ����w��)
	@param[in]	���ɉ��������邩: -1:�ݒ�ς݂̒l���g��
	@param[in]	�c�ɉ��������邩: -1:�ݒ�ς݂̒l���g��
	@note		�A�g���X���g�������Ȃ̂ɃI�t�Z�b�g�ƕ������ŃQ�b�^�[�ƃZ�b�^�[��p�ӂ���Ǝ菇�������A
				���[�U�r���e�B�̒ቺ���S�z�������̂ŃA�g���X�͈�ɂ܂Ƃ߂��B
				�������͕��̒l���w��ł��Ȃ��悤��"unsigned"�Ŏw�肳����
*/
void API::Texture::SetupAtlas(DirectX::XMINT2 atlasOffset, unsigned int div_x, unsigned int div_y)
{
	try
	{
		//	�A�g���X�̃I�t�Z�b�g
		m_AtlasOffset = atlasOffset;
		
		//	������
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
