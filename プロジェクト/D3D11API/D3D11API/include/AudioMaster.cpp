/*
	@file	AudioMaster.cpp
	@date	2019/04/18
	@author	�ԏ� �G�P
	@brief	�I�[�f�B�I�f�o�C�X�N���X
	@detail	XAudio���g����Audio�f�o�C�X���Ǘ�
			�V���O���g��
*/
#include "stdafx.h"
#include "AudioMaster.h"
#include "MyGame.h"
#include "MemoryLeaks.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Sound
*/
using namespace D3D11::Sound;

/*!
	@brief	�R���X�g���N�^
*/
AudioMaster::AudioMaster() :
	m_pXAudio2(nullptr),
	m_pMasteringVoice(nullptr)
{
}

/*!
	@brief	�f�X�g���N�^
*/
AudioMaster::~AudioMaster()
{
	Finalize();
}

/*!
	@fn		Initialize
	@brief	������
	@return	S_OK:���� E_FAIL:���s
*/
HRESULT AudioMaster::Initialize()
{
	// COM���C�u�����̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
			ErrorLog("COM not Initialize");
			return E_FAIL;
		}
	}

	// XAudio2�I�u�W�F�N�g����
	if (FAILED(XAudio2Create(&m_pXAudio2, 0))) {
		CoUninitialize();
		ErrorLog("IXAudio2 not instance");
		return E_FAIL;
	}

	// MasteringVoice�I�u�W�F�N�g����
	if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice))) {
		CoUninitialize();
		ErrorLog("IXaudio2MasteringVoice not instance");
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn		Finalize
	@brief	�j������
	@detail	�����o�̖����I�ȉ���ƃ����o�̏�����
*/
void AudioMaster::Finalize()
{
	if (m_pMasteringVoice) { m_pMasteringVoice->DestroyVoice(); m_pMasteringVoice = nullptr; }
	if (m_pXAudio2) { m_pXAudio2->Release(); m_pXAudio2 = nullptr; }
}
