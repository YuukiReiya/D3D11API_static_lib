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
#include "AbstractShader.h"
#include "Transform.h"
#include "Color.h"
#include "Material.h"

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
			@brief	���[�u�R���X�g���N�^
		*/
		Mesh(const Mesh&inst);

		/*!
			@brief	�f�X�g���N�^	
		*/
		~Mesh();

		/*!
			@fn			Initialize
			@brief		������
			@detail		���f���݂̂̓ǂݍ��݂ŕʓr�e�N�X�`�����쐬����K�v������
			@param[in]	�O���t�@�C���̃p�X
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn			Initialize
			@brief		������
			@detail		���f���ƃe�N�X�`���𓯎��ɐ�������
			@param[in]	�O���t�@�C���̃p�X
			@param[in]	���蓖�Ă�e�N�X�`���̃p�X
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize(std::string meshPath, std::string texPath);

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

		static inline void SetupMaterial(Mesh*mesh, Material*material) {
			mesh->m_pMaterial = material->GetSharedPtr();
		}
		inline void SetupMaterial(Material*material) {
			SetupMaterial(this, material);
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
		std::shared_ptr<Transform> transform;

		/*!
			@var	color
			@brief	���b�V���̐F
		*/
		Color color;
	private:
		//	�t�����h�錾��p�����ȈՃe�X�g�P�[�X�̍쐬
		//�����t�@�C���ŏC������(�����܂œ���e�X�g���m�F�ł���܂�)
		friend class AnimationMesh;
		/*!
			@fn		SetupTopology
			@brief	�g�|���W�[�̃Z�b�g�A�b�v
		*/
		void SetupTopology();

		/*!
			@fn		SetupInputLayout
			@brief	���_���C�A�E�g�̃Z�b�g�A�b�v
		*/
		void SetupInputLayout();

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
		*/
		void SetupIndexBuffer();

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
			@fn		SetupVertexBuffer
			@brief	���_�o�b�t�@�̃Z�b�g�A�b�v
		*/
		void SetupVertexBuffer();

		/*!
			@fn		SetupConstantBuffer
			@brief	�R���X�^���g�o�b�t�@�̃Z�b�g�A�b�v
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT SetupConstantBuffer();

		/*!
			@fn		SetupBindShader
			@brief	ImmediateContext�ɃV�F�[�_�[���o�C���h
			@detail	�Z�b�g����V�F�[�_�[�͒��_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[
		*/
		void SetupBindShader();

		/*!
			@fn		SetupTexture
			@brief	�e�N�X�`���̃Z�b�g�A�b�v
			@detail	SRV�ƃT���v���[�̃Z�b�g�A�b�v���s��
			@note	�e�N�X�`���̖������f�����z�肵�AError�ɂ�郁�b�Z�[�W�{�b�N�X�͕\�����Ȃ�
		*/
		void SetupTexture();

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
		//Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;

		/*!
			@var	m_pSRV
			@brief	�V�F�[�_�[���\�[�X�r���[
			@detail	ComPtr
		*/
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;

		std::weak_ptr<Material*>m_pMaterial;
	};
}