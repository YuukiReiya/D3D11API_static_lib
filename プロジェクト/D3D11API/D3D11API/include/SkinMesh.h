/*
	@file	SkinMesh.h
	@date	2019/08/14
	@author	�ԏ� �G�P
	@brief	�X�L���A�j���[�V�������郁�b�V���N���X
*/
#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include "SkinnedVertex.h"
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "Transform.h"
#include "Joint.h"
#include "AnimationClip.h"
#include <algorithm>

namespace API
{
	class SkinMesh
		: public API::AbstractRender
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		explicit SkinMesh();

		/*!
			@brief	�f�X�g���N�^
		*/
		~SkinMesh();

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	�ǂݍ��ݗp�̃t�@�C���p�X
			@return		S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	�`��
		*/
		void Render()override;

		/*!
			@var	transform
			@brief	�g�����X�t�H�[��
		*/
		std::shared_ptr<Transform>transform;

		/*!
			@fn			AddAnimationFrame
			@brief		�A�j���[�V�����t���[���̉��Z
			@detail		���ݎQ�Ƃ��Ă���t���[���ԍ������Z�B
						���Q�Ɛ؂�Ȃ珈�����Ȃ��B
			@param[in]	true:�A�j���[�V�����I�����ɍŏ�����B false:�A�j���[�V�����I�������̂܂�
		*/
		void AddAnimationFrame(bool isLoop = true)
		{
			if (m_AnimClip.expired()) { return; }
			auto ptr = m_AnimClip.lock();
			frameIndex = frameIndex < ptr->matrixPalette[0].size() - 1 ? ++frameIndex : isLoop ? 0 : ptr->matrixPalette[0].size() - 1;
		}

		/*!
			@fn			SetupAnimationFrame
			@brief		�A�j���[�V�����̎Q�Ƃ���t���[�����w��
			@detail		�͈͊O��ݒ肳�ꂽ��͈͓��Ɋۂ߂�B
						���Q�Ɛ؂�Ȃ珈�����Ȃ��B
			@param[in]	�Q�Ƃ���t���[���ԍ�
		*/
		void SetupAnimationFrame(uint32_t frame)
		{
			if (m_AnimClip.expired()) { return; }
			auto ptr = m_AnimClip.lock();
			frameIndex = std::clamp(frame, static_cast<uint32_t>(0), static_cast<uint32_t>(ptr->matrixPalette[0].size() - 1));
		}

		/*!
			@fn			SetupAnimation
			@brief		�A�j���[�V�����̐ݒ�
			@param[in]	�o�^����A�j���[�V�����N���b�v�̋��L�|�C���^
		*/
		void SetupAnimation(std::shared_ptr<API::AnimationClip> clip) {
			m_AnimClip = clip;
		};
	private:
		/*!
			@enum	SkinningMode
			@brief	�X�L�j���O���@
		*/
		enum SkinningMode:BYTE
		{
			/*!
				@var	SOFTWARE
				@brief	�\�t�g�E�F�A���ŃX�L�j���O
			*/
			SOFTWARE,

			/*!
				@var	SHADER
				@brief	�V�F�[�_�[���ŃX�L�j���O

				@note	������(���݂͑S�ă\�t�g�E�F�A�X�L�j���O)
			*/
			SHADER
		}
		/*!
			@var	m_eSkinningMode
			@brief	�X�L�j���O���@��ێ����郁���o
			@detail	���[�h������񂩂�X�L�j���O�̂�����ς���
		*/
		m_eSkinningMode;


		/*!
			@var	m_pIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		
		/*!
			@var	m_pVertexBuffer
			@brief	���_�o�b�t�@
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pShader
			@brief	Mesh�N���X�Ŏg�p����V�F�[�_�[�̎�Q��
			@detail	weak_ptr
		*/
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
		
		/*!
			@var	m_Indices
			@brief	���_�C���f�b�N�X
		*/
		std::vector<uint32_t>m_Indices;

		/*!
			@var	m_Vertices
			@brief	���_
		*/
		std::vector<D3D11::Graphic::SkinnedVertex>m_Vertices;

		/*!
			@var	m_AnimClip
			@brief	�A�j���[�V�����N���b�v�̎�Q��
		*/
		std::weak_ptr<API::AnimationClip>m_AnimClip;

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
			@fn		SetupVertexBuffer
			@brief	���_�o�b�t�@�̃Z�b�g�A�b�v
		*/
		void SetupVertexBuffer();

		/*!
			@fn		SetupIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
		*/
		void SetupIndexBuffer();

		/*!
			@fn			Load
			@brief		�t�@�C���̓ǂݍ���
			@param[in]	�o�C���h�惁�b�V��
			@param[in]	�t�@�C���̃p�X
			@return		true:���� false:���s
		*/
		static bool Load(SkinMesh*mesh,std::string filePath);

		/*!
			@fn		SoftwareSkinning
			@brief	�\�t�g�E�F�A�X�L�j���O
		*/
		void SoftwareSkinning();

		/*!
			@fn		ShaderSkinning
			@brief	�V�F�[�_�[���ł̃X�L�j���O
		*/
		void ShaderSkinning();

		/*!
			@fn		CreateIndexBuffer
			@brief	�C���f�b�N�X�o�b�t�@�̍쐬
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT CreateIndexBuffer();

		/*!
			@fn		CreateVertexBuffer
			@brief	���_�o�b�t�@�̍쐬
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT CreateVertexBuffer();

		/*!
			@var	frameIndex
			@brief	�A�j���[�V�����̃t���[��
		*/
		uint32_t frameIndex;
	};
}