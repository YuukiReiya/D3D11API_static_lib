/*!
	@file	AnimationMesh.h
	@date	2019/08/04
	@author	�ԏ� �G�P
	@brief	�t���[���A�j���[�V�������b�V��
*/

#pragma once
#include "AbstractRender.h"
#include "AbstractShader.h"
#include <memory>
#include <DirectXMath.h>
#include "MeshVertex.h"
#include <wrl/client.h>
#include "Transform.h"
#include <unordered_map>

namespace API {
#pragma region ���_�\����
	/*!
		@struct	VERTEX
		@brief	�t���[���A�j���[�V�������b�V���Ŏg�����_�\����

		NOTE:	UV���Ή�
		TODO:	UV�Ή��A�@�����ǉ��Aetc
	*/
	struct VERTEX {
		/*!
			@var	pos
			@brief	���_�̍��W
		*/
		DirectX::XMFLOAT3 pos;
	};
#pragma endregion

	/*!
		@class	AnimationMesh
		@brief	�t���[���A�j���[�V�������s�����b�V���N���X
	*/
	class AnimationMesh
		:public API::AbstractRender
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		AnimationMesh() :m_AnimIndex({0,0}) { transform = std::make_shared<Transform>(); }

		/*!
			@brief	�f�X�g���N�^
		*/
		~AnimationMesh() { m_VertexList.clear(); };

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	�ǂݍ��݃p�X
			@return		����:S_OK ���s:E_FAIL
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	�`��
		*/
		void Render()override;

		/*!
			@fn		AddAnimIndex
			@brief	�A�j���[�V�����̐؂�ւ�
			@detail	�t���[�����̓[���ɖ߂��B
		*/
		void AddAnimIndex() {
			m_AnimIndex.x = (size_t)m_AnimIndex.x + 1 < m_VertexList.size() ? ++m_AnimIndex.x : 0;
			m_AnimIndex.y = 0;
		}

		/*!
			@fn		AddFrameIndex
			@brief	�t���[���̐؂�ւ�
		*/
		void AddFrameIndex() {
			m_AnimIndex.y = (size_t)m_AnimIndex.y + 1 < m_VertexList[m_AnimIndex.x].size() ? ++m_AnimIndex.y : 0;
		}

		/*!
			@var	transform
			@brief	�g�����X�t�H�[��
		*/
		std::shared_ptr<Transform>transform;
	private:
		/*!
			@var	m_pShader
			@brief	�V�F�[�_�[
		*/
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

		/*!
			@fn			CreateVertexBuffer
			@brief		���_�o�b�t�@�̍쐬
			@param[in]	�ϒ��z��Ɋi�[�������_���
			@return		����:S_OK ���s:E_FAIL
		*/
		HRESULT CreateVertexBuffer(std::vector<D3D11::Graphic::MeshVertex>verttices);

		/*!
			@fn			CreateIndexBuffer
			@brief		�C���f�b�N�X�o�b�t�@�̍쐬
			@param[in]	�ϒ��z��Ɋi�[�����C���f�b�N�X���
			@return		����:S_OK ���s:E_FAIL
		*/
		HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);

		/*!
			@fn		SetupTopology
			@brief	�g�|���W�[�̐ݒ�
		*/
		void SetupTopology();

		/*!
			@fn		SetupInputLayout
			@brief	���_���C�A�E�g�̐ݒ�
		*/
		void SetupInputLayout();

		/*!
			@fn		SetupIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@�̐ݒ�
		*/
		void SetupIndexBuffer();

		/*!
			@fn		SetupVertexBuffer
			@brief	���_�o�b�t�@�̐ݒ�
		*/
		void SetupVertexBuffer();

		/*!
			@fn		SetupConstantBuffer
			@brief	�R���X�^���g�o�b�t�@�̐ݒ�
			@return	����:S_OK ���s:E_FAIL
		*/
		HRESULT SetupConstantBuffer();

		/*!
			@fn		SetupBindShader
			@brief	�V�F�[�_�[�̐ݒ�
		*/
		void SetupBindShader();

		/*!
			@var	m_pVertexBuffer
			@brief	���_�o�b�t�@
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

		/*!
			@var	m_IndexCount
			@brief	�C���f�b�N�X��
		*/
		uint32_t m_IndexCount;

		/*!
			@var	m_VertexCount
			@brief	���_��
		*/
		uint32_t m_VertexCount;

		/*!
			@var	m_AnimIndex
			@brief	�A�j���[�V�����̔ԍ��ƃt���[���̔ԍ�
		*/
		DirectX::XMINT2 m_AnimIndex;

		/*!
			@var	m_VertexList
			@brief	���_�̃��X�g
			@detail	[�A�j���[�V�����ԍ�][�t���[���ԍ�]
		*/
		std::unordered_map<size_t, std::unordered_map<size_t, std::vector<DirectX::XMFLOAT3>>>m_VertexList;

		/*!
			@var	m_UV
			@brief	UV�̃e�N�Z�����W

			NOTE:	���b�V���\���̂ɍ��킹�邽�߂Ɏ������Ă��邾���ŁA
					�����������_����UV�͊܂܂�Ă��Ȃ��̂Œ��g���Ȃ�ł��낤�Ɗ֌W�Ȃ�
		*/
		std::vector<DirectX::XMFLOAT2>m_UV;
	};



}
