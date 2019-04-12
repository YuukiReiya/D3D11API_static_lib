/*
	@file	RenderQueue.h
	@date	2019/04/08
	@author	�ԏ� �G�P
	@brief	�`��I�u�W�F�N�g�̃����_�����O���̐���
	@detail	�V���O���g��
*/
#pragma once
#include "Singleton.h"
#include <vector>
#include "AbstractRender.h"

/*! Direct3D11�֘A */
namespace D3D11 {

	/*! �`��֘A�̖��O��ԂɊ܂߂� */
	namespace Graphic {

		/*!
			@class	RenderQueue
			@brief	�`��I�u�W�F�N�g�̃����_�����O���𐧌䂷�邽�߂̃R���e�i�N���X
			@detail	�V���O���g��
		*/
		class RenderQueue
			:public Singleton<RenderQueue>
		{
		public:
			/*!
				@brief	�f�X�g���N�^
			*/
			~RenderQueue();

			/*!
				@fn		Render
				@brief	�`��I�u�W�F�N�g�̈ꊇ�`��
				@detail	�ς܂ꂽ�`��I�u�W�F�N�g��Draw()�֐����ꊇ�Ăяo��
			*/
			void Render();


			/*!
				@enum	Priority
				@brief	�D��x
			*/
			enum Priority
			{
				/*!
					@var	Opacity
					@brief	�s����
				*/
				Opacity,

				/*!
					@var	High
					@brief	�D��x:��
				*/
				High,

				/*!
					@var	Low
					@brief	�D��x:��
				*/
				Low,
			};

			/*!
				@fn			Add
				@brief		�`��R���e�i�ւ̒ǉ�
				@param[in]	�`��I�u�W�F�N�g
				@param[in]	�`��̗D��x
			*/
			void Add(API::AbstractRender* render,Priority priority = Priority::Low);

			/*!
				@fn		Clear
				@brief	�`��R���e�i�̃N���A
			*/
			void Clear();
		private:
			/*!
				@brief	�R���X�g���N�^
			*/
			RenderQueue();

			/*!
				@fn			Loop
				@brief		�`�惋�[�v
				@param[in]	�`��I�u�W�F�N�g�̃R���e�i
			*/
			void Loop(std::vector<API::AbstractRender*>&renders);

			/*!
				@brief	�V���O���g���f�U�C���p�^�[���̃e���v���[�g�p��
			*/
			friend class Singleton<RenderQueue>;

			/*!
				@var	m_pOpacityRenders
				@brief	�s�����ȕ`��I�u�W�F�N�g������R���e�i
			*/
			std::vector<API::AbstractRender*>m_pOpacityRenders;

			/*!
				@var	m_pHighRenders
				@brief	�D��x�̍����`��I�u�W�F�N�g������R���e�i
			*/
			std::vector<API::AbstractRender*>m_pHighRenders;

			/*!
				@var	m_pLowRenders
				@brief	�D��x�̒Ⴂ�`��I�u�W�F�N�g������R���e�i
			*/
			std::vector<API::AbstractRender*>m_pLowRenders;
		};
	}
}
