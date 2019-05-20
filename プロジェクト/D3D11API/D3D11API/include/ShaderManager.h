///*
//	@file	ShaderManager.h
//	@date	2018/03/04
//	@author	�ԏ� �G�P
//	@brief	�V�F�[�_�[�֘A
//	@detail	vector,map�̒���:http://msty.hatenablog.jp/entry/2016/08/18/144934
//*/
//#pragma once
//#include <D3D11.h>
//#include <unordered_map>
//#include <wrl/client.h>
//#include "Singleton.h"
//
//#if defined DEBUG||_DEBUG
//# define DEBUG_SHADER
//#endif
//
///*! Direct3D11�֘A */
//namespace D3D11 {
//
//	/*! �`��֘A�̖��O��ԂɊ܂߂� */
//	namespace Graphic {
//
//		/*!
//			@brief	�V�F�[�_�[���\������\����
//		*/
//		struct ShaderData
//		{
//		public:
//			/*!
//				@brief	�R���X�g���N�^
//			*/
//			ShaderData(){
//				SecureZeroMemory(this, sizeof(this));
//				m_pVertexLayout		= nullptr;
//				m_pVertexShader		= nullptr;
//				m_pPixelShader		= nullptr;
//				m_pConstantBuffer	= nullptr;
//			}
//
//			/*!
//				@brief	�f�X�g���N�^
//			*/
//			~ShaderData()
//			{
//				m_pVertexLayout.Reset();
//				m_pVertexShader.Reset();
//				m_pPixelShader.Reset();
//				m_pConstantBuffer.Reset();
//			};
//
//			Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_pVertexLayout;	/*< �C���v�b�g���C�A�E�g */
//			Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_pVertexShader;	/*< ���_�V�F�[�_�[ */
//			Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;		/*< �s�N�Z���V�F�[�_�[ */
//			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pConstantBuffer;	/*< �R���X�^���g�o�b�t�@�[ */
//		};
//
//		/*!
//			@brief	�V�F�[�_�[�Ǘ��N���X
//			@detail	�V�F�[�_�[���쐬�����Ƃ��`��o����f�t�H���g�V�F�[�_�[��p�ӂ���
//		*/
//		class ShaderManager
//			:public Singleton<ShaderManager>
//		{
//		public:
//			/*!
//				@brief	�f�X�g���N�^
//			*/
//			~ShaderManager();
//
//			/*!
//				@var	c_szSimpleTextureShader
//				@brief	�ʏ�e�N�X�`���Ŏg���V�F�[�_�[�̃n�b�V��
//				@detail	unordered_map�ɒǉ������V�F�[�_�[�f�[�^�̎Q�Ɨp�̃n�b�V��
//			*/
//			static const std::string c_szSimpleTextureShader;
//
//			/*!
//				@var	c_szTextureAtlasShader
//				@brief	�A�g���X�e�N�X�`���Ŏg���V�F�[�_�[�̃n�b�V��
//				@detail	unordered_map�ɒǉ������V�F�[�_�[�f�[�^�̎Q�Ɨp�̃n�b�V��
//			*/
//			static const std::string c_szTextureAtlasShader;
//
//			/*!
//				@var	c_DefaultMeshShader
//				@brief	�f�t�H���g�ݒ�̃��b�V���Ŏg���V�F�[�_�[�̃n�b�V��
//				@detail	unordered_map�ɒǉ������V�F�[�_�[�f�[�^�̎Q�Ɨp�̃n�b�V��
//			*/
//			static const std::string c_DefaultMeshShader;
//
//			/*!
//				@fn			�V�F�[�_�[�f�[�^�̒ǉ�
//				@brief		unordered_map�ɍ쐬�����V�F�[�_�[�f�[�^��ǉ��o�^
//				@param[in]	�n�b�V����
//				@param[in]	�o�^����V�F�[�_�[�f�[�^�̎Q��
//				@return		S_OK:���� E_FAIL:���s
//			*/
//			HRESULT AddNewShaderData(std::string szKeyName, ShaderData* pNewShaderData);	
//
//			/*!
//				@fn			�C�j�V�����C�Y
//				@brief		������
//				@detail		�ʏ�e�N�X�`��/�A�g���X�e�N�X�`���Ŏg���V�F�[�_�[�̐���
//				@param[in]	�V�F�[�_�[(HLSL)�̊܂܂�Ă���f�B���N�g���̃p�X
//				@return		S_OK:���� E_FAIL:���s
//			*/
//			HRESULT Initialize(std::string directoryPath = "");
//
//			/*!
//				@fn		�t�@�C�i���C�Y
//				@brief	�j������
//				@detail	�����I�ȃ����o�̊J������
//			*/
//			void Finalize();
//
//			/*!
//				@fn			�V�F�[�_�[�쐬
//				@detail		ShaderData��ID3D11Shader���쐬
//				@param[in]	�ݒ肵���f�B���N�g������̃V�F�[�_�[�t�@�C���̃p�X(.hlsl)
//				@param[in]	�V�F�[�_�[�̃G���g���[�|�C���g
//				@param[in]	�V�F�[�_�[�̃v���t�@�C�����(������_�o�[�W����) ex) vs_5_0
//				@param[in]	ID3D11*****Shader�̃|�C���^���L���X�g���ēn�� ex)(void**)pVertexShader
//				@param[in]	�R���p�C���p�̃u���u���|�C���^��
//				@return		S_OK:���� E_FAIL:���s
//			*/
//			HRESULT MakeShader(std::string szFileName, std::string szFuncName, std::string szProfileName, void** ppShader, ID3DBlob** ppBlob);
//
//			/*!
//				@fn			�V�F�[�_�[�f�[�^�̎擾
//				@brief		unorderd_map�Ɋi�[�����V�F�[�_�[�f�[�^���n�b�V���L�[����擾
//				@param[in]	�o�^�n�b�V��
//				@return		�n�b�V���ɓo�^���ꂽ�����V�F�[�_�[�f�[�^��Ԃ�
//							�T�������񂩂�����擾���邱�ƂɎ��s�����ꍇNULL��Ԃ�
//			*/
//			ShaderData* GetShaderData(std::string szKeyName);
//
//		private:
//			/*!
//				@brief	�R���X�g���N�^
//			*/
//			ShaderManager();
//
//			/*!
//				@brief	�V���O���g���f�U�C���p�^�[���̃e���v���[�g�p��
//			*/
//			friend class Singleton<ShaderManager>;
//
//			/*!
//				@var	m_DirectoryPath;
//				@brief	�V�F�[�_�[(.hlsl)���܂܂�Ă���f�B���N�g���̃p�X
//				@detail	MakeShader�͂��̃p�X+�t�@�C���̖��O�̃p�X�ɑ��݂���HLSL�t�@�C���ŃV�F�[�_�[�̍쐬���s��
//			*/
//			std::string m_DirectoryPath;
//
//			/*!
//				@var	m_pAddDataRef
//				@brief	�f�[�^�ǉ��p�̎Q�ƃ����o
//			*/
//			ShaderData* m_pAddDataRef;
//
//			/*!
//				@var	m_pShaderDataUMap;
//				@brief	�V�F�[�_�[�f�[�^�̘A�z�z��
//			*/
//			std::unordered_map<std::string, ShaderData*>m_pShaderDataUMap;
//
//		};
//
//	}
//}
