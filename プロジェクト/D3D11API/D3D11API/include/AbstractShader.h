/*
	@file	AbstractShader.h
	@date	2019/04/05
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�̒��ۃN���X
	@note	�`��N���X�݌v�������ɍۂ��A�X�V���Ă���
*/
#pragma once
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <string>
#include "CustomShaderBin.h"

/*! Direct3D11�֘A */
namespace D3D11 {

	/*! �`��֘A�̖��O��ԂɊ܂߂� */
	namespace Graphic {

		/*!
			@brief	�V�F�[�_�[�̒��ۃN���X
		*/
		class AbstractShader abstract {
		public:
			/*!
				@brief	�R���X�g���N�^
				@detail	�C���X�^���X�������ɃC���X�^���X�̃A�h���X���V�F�A�[�h�E�|�C���^�ɕێ�������
			*/
			explicit AbstractShader();

			/*!
				@fn		Setup
				@brief	�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p���ăZ�b�g�A�b�v���s��
				@detail	�������z�֐�
			*/
			virtual HRESULT Setup()abstract;

			/*!
				@fn		DynamicSetup
				@brief	���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
				@detail	�������z�֐�
			*/
			virtual HRESULT DynamicSetup()abstract;

			/*!
				@fn		GetInputLayout
				@brief	���_���C�A�E�g�̎擾
				@detail	�C�����C���֐�
			*/
			inline ID3D11InputLayout**GetInputLayout() { return m_pVertexLayout.GetAddressOf(); }

			/*!
				@fn		GetVertexShader
				@brief	���_�V�F�[�_�[�̎擾
				@detail	�C�����C���֐�
			*/
			inline ID3D11VertexShader**GetVertexShader(){ return m_pVertexShader.GetAddressOf(); }

			/*!
				@fn		GetPixelShader
				@brief	�s�N�Z���V�F�[�_�[�̎擾
				@detail	�C�����C���֐�

			*/
			inline ID3D11PixelShader**GetPixelShader() { return m_pPixelShader.GetAddressOf(); }
			
			/*!
				@fn		GetConstantBuffer
				@brief	�R���X�^���g�o�b�t�@�̎擾
				@detail	�C�����C���֐�
			*/
			inline ID3D11Buffer**GetConstantBuffer() { return m_pConstantBuffer.GetAddressOf(); }
			
			/*!
				@fn		GetSharedPtr
				@brief	AbstractShader�^�V�F�A�[�h�E�|�C���^�̎擾
				@detail	�C�����C���֐�
						�C���X�^���X�������Ɏ��M�̃A�h���X���X�}�[�g�|�C���^�ɕێ�������
				@note	Render�N���X����͎�Q�Ƃ̕������S�Ȃ��߁A�݌v���H�v	
			*/
			inline std::shared_ptr<AbstractShader*>GetSharedPtr() { return m_pShared; }

		protected:
			/*!
				@fn			DynamicCompile
				@brief		�V�F�[�_�[�t�@�C���̓��I�R���p�C��
				@param[in]	�V�F�[�_�[�t�@�C���̃p�X
				@param[in]	�G���g���[�|�C���g��
				@param[in]	�V�F�[�_�[�̃v���t�@�C���� ex)vs_5_0
				@param[in]	�R���p�C���p�̃u���u
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);

			/*!
				@var	m_pVertexLayout
				@brief	���_���C�A�E�g
			*/
			Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_pVertexLayout;

			/*!
				@var	m_pVertexShader
				@brief	���_�V�F�[�_�[
			*/
			Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_pVertexShader;

			/*!
				@var	m_pPixelShader
				@brief	�s�N�Z���V�F�[�_�[
			*/
			Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;

			/*!
				@var	m_pConstantBuffer
				@brief	�R���X�^���g�o�b�t�@
			*/
			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pConstantBuffer;

			/*!
				@var	m_pShared
				@brief	�C���X�^���X�̃A�h���X��ێ������V�F�A�[�h�E�|�C���^
			*/
			std::shared_ptr<AbstractShader*>			m_pShared;
			
			/*!
				@fn			CreateVertexShader
				@brief		���_�V�F�[�_�[�̍쐬
				@detail		���O�Ƀu���u���R���p�C�����Ă������ƁI
				@param[in]	�R���p�C���ς݃u���u
				@param[in]	���_�V�F�[�_�[
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT CreateVertexShader(ID3DBlob*pBlob, ID3D11VertexShader**pVertexShader);

			/*!
				@fn			CreateVertexShader
				@brief		���_�V�F�[�_�[�̍쐬
				@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
				@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
				@param[in]	���_�V�F�[�_�[
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT CreateVertexShader(CustomShaderBin* bin, ID3D11VertexShader**pVertexShader);

			/*!
				@fn			CreateInputLayout
				@brief		���_���C�A�E�g�쐬
				@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
				@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
				@param[in]	���_���C�A�E�g�̒�`
				@param[in]	���_���C�A�E�g
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT CreateInputLayout(CustomShaderBin* bin, D3D11_INPUT_ELEMENT_DESC desc[], ID3D11InputLayout**pInputLayout);

			/*!
				@fn			CreatePixelShader
				@brief		�s�N�Z���V�F�[�_�[�̍쐬
				@detail		���O�Ƀu���u���R���p�C�����Ă������ƁI
				@param[in]	�R���p�C���ς݃u���u
				@param[in]	�s�N�Z���V�F�[�_�[
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT CreatePixelShader(ID3DBlob*pBlob, ID3D11PixelShader**pPixelShader);

			/*!
				@fn			CreatePixelShader
				@brief		�s�N�Z���V�F�[�_�[�̍쐬
				@detail		�v���R���p�C���ς݃V�F�[�_�[�t�@�C�����g�p����
				@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
				@param[in]	�s�N�Z���V�F�[�_�[
				@return		S_OK:���� E_FAIL:���s
			*/
			static HRESULT CreatePixelShader(CustomShaderBin* bin, ID3D11PixelShader**pPixelShader);
		};
	}
}