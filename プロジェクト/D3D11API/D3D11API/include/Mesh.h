/*
	@file	Mesh.h
	@date	2019/05/17
	@author	�ԏ� �G�P
	@brief	���b�V��
*/
#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "AbstractRender.h"
#include "Transform.h"
#include "AbstractShader.h"

/*! API�̖��O��ԂɊ܂߂� */
namespace API{

	class Mesh
		: public AbstractRender
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		explicit Mesh();

		/*!
			@brief	�f�X�g���N�^	
		*/
		~Mesh();

		/*!
			@fn		Initialize
			@brief	������
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize();

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	�O���t�@�C���̃p�X
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn			SetupShader
			@brief		�V�F�[�_�[�̎Q�Ƃ̃Z�b�g
			@detail		�ÓI�֐��ō���Ă����΃��[�v���ŉ񂹂�
			@param[in]	�ݒ��̃��b�V��
			@param[in]	�ݒ茳�̃V�F�[�_�[
		*/
		static inline void SetupShader(Mesh*mesh, D3D11::Graphic::AbstractShader*shader)
		{
			mesh->m_pShader = shader->GetSharedPtr();
		}

		/*!
			@fn			SetupShader
			@brief		�V�F�[�_�[�̎Q�Ƃ̃Z�b�g
			@param[in]	�ݒ茳�̃V�F�[�_�[
		*/
		inline void SetupShader(D3D11::Graphic::AbstractShader*shader)
		{
			SetupShader(this, shader);
		}

		/*!	
			@fn		Finalize
			@brief	�j������
			@detail	�����o�̖����I�ȊJ��
		*/
		void Finalize();

		/*!
			@fn		Render
			@brief	�`�揈��
			@detail	�������z�֐����I�[�o�[���C�h
		*/
		void Render()override;

		/*!
			@var	transform
			@brief	�g�����X�t�H�[��
		*/
		Transform transform;
	private:
		/*!
			@fn			CreateIndexBuffer
			@brief		�C���f�b�N�X�o�b�t�@�쐬
			@detail		�����֐��͐ÓI�ł���
			@param[in]	�o�C���h���郁�b�V��
			@param[in]	�C���f�b�N�X���
			@return		S_OK:���� E_FAIL:���s
		*/
		static HRESULT CreateIndexBuffer(Mesh*mesh,std::vector<uint32_t>indices);

		/*!
			@fn			SetupIndexBuffer
			@brief		���������C���f�b�N�X�o�b�t�@�̃Z�b�g
			@param[in]	�o�C���h��̃��b�V��
		*/
		static void SetupIndexBuffer(Mesh*mesh);

		/*!
			@template	Vertex
			@brief		CreateVertexBuffer�֐��Ŏg�����_�\���̂̂��߂̃e���v���[�g
		*/
		template <class Vertex>

		/*!
			@fn			CreateVertexBuffer
			@brief		���_�o�b�t�@����
			@param[in]	������̃��b�V��
			@param[in]	���_�o�b�t�@���\�����钸�_���
			@return		S_OK:���� E_FAIL:���s
		*/
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<Vertex>verttices);

		/*!
			@var	m_IndexCount
			@brief	���_�C���f�b�N�X�̃C���f�b�N�X��
		*/
		uint32_t m_IndexCount;

		/*!
			@var	m_pShader
			@brief	Mesh�N���X�Ŏg�p����V�F�[�_�[�̎�Q��
			@detail	weak_ptr
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;

		/*!
			@var	m_pVertexBuffer
			@brief	���_�o�b�t�@
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

		/*!
			@var	m_pSamplerState
			@brief	�T���v���[�X�e�[�g
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;

		/*!
			@var	m_pSRV
			@brief	�V�F�[�_�[���\�[�X�r���[
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	};
}