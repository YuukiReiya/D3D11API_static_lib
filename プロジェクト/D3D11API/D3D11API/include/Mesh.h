/*
	@file	Mesh.h
	@date	2019/04/29
	@author	�ԏ� �G�P
	@brief	���b�V���p�̃N���X
*/
#pragma once
#include <d3d11.h>
#include <string>
#include <memory>
#include <vector>
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "StructShaderBase.h"
#include "MeshVertex.h"
#include "Transform.h"
#include "Color.h"

namespace D3D11 {
	namespace Graphic {
		/*!
			@struct	MeshShaderBuffer
			@brief	���b�V���̃R���X�^���g�o�b�t�@
		*/
		struct alignas(16) MeshShaderBuffer
			:public BaseConstantBuffer
		{
			/*!
				@var	m_Color
				@brief	�F
			*/
			DirectX::XMFLOAT4 m_Color;
		};
	}
}

namespace API{
	class Mesh
		:public AbstractRender
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
			@fn			Initialize
			@brief		������
			@param[in]	�ǂݍ��ރt�@�C���̃p�X
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	�`��
		*/
		void Render()override final;

		/*!
			@fn			SetupShader
			@brief		�V�F�[�_�[�̐ݒ�
			@detail		��Q�ƂŃo�C���h����
			@param[in]	�o�^����V�F�[�_�[�̃|�C���^
		*/
		void SetupShader(D3D11::Graphic::AbstractShader* shader);

		/*!
			@var	transform
			@brief	�g�����X�t�H�[��
		*/
		std::shared_ptr<Transform> transform;

		/*!
			@var	color
			@brief	�F
		*/
		Color color;
	private:
		/*!
			@fn		SetupTopology
			@brief	�g�|���W�[�̐ݒ�
		*/
		void SetupTopology();

		/*!
			@fn		SetupConstantBuffer
			@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		*/
		void SetupConstantBuffer();

		/*!
			@fn		SetupBindShader
			@brief	�V�F�[�_�[�̐ݒ�
		*/
		void SetupBindShader();

		/*!
			@fn			CreateVertexBuffer
			@brief		���b�V���̒��_�𐶐�
			@detail		�ÓI�֐�
			@param[in]	�ݒ肷�郁�b�V���̃|�C���^
			@param[in]	�\�����钸�_���
			@return		����:S_OK ���s:E_FAIL
		*/
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<D3D11::Graphic::MeshVertex>vertex);

		/*!
			@fn		SetupVertexBuffer
			@brief	���_�o�b�t�@�ݒ�
		*/
		void SetupVertexBuffer();

		/*!
			@fn			CreateIndexBuffer
			@brief		�C���f�b�N�X�o�b�t�@�쐬
			@detail		�ÓI�֐�
			@param[in]	�ݒ肷�郁�b�V���̃|�C���^
			@param[in]	���_�̃C���f�b�N�X
			@return		����:S_OK ���s:E_FAIL
		*/
		static HRESULT CreateIndexBuffer(Mesh*mesh, std::vector<uint32_t>index);

		/*!
			@fn		SetupVertexBuffer
			@brief	�C���f�b�N�X�o�b�t�@�ݒ�
		*/
		void SetupIndexBuffer();

		/*!
			@var	m_VertexIndex
			@brief	���_�C���f�b�N�X
			@NOTE	���_�C���f�b�N�X�̐������_��
					�� MeshVertex��"uint"�^�Ŋ܂ނ��Ƃ��o���Ȃ�
		*/
		std::vector<uint32_t>m_VertexIndex;

		/*!
			@var	m_pVertexBuffer
			@brief	���_�o�b�t�@
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pIndexBuffer;

		/*!
			@var	m_pShader
			@brief	�V�F�[�_�[�I�u�W�F�N�g�̎�Q��
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;
	};
}