/*
	@file	Joint.h
	@date	2019/08/14
	@author	�ԏ� �G�P
	@brief	�֐߂̍\���̒�`
*/
#pragma once
#include <stdint.h>

namespace D3D11
{
	namespace Graphic {

		/*!
			@struct	Joint
			@brief	�֐�
		*/
		struct Joint
		{
			/*!
				@var	index
				@brief	�C���f�b�N�X
				@detail	�W���C���g�̃C���f�b�N�X
			*/
			uint32_t index;

			/*!
				@var	weight
				@brief	�d��
			*/
			float weight;

			/*!
				@operator ==
				@brief	�����Z�q
				@detail	���� == �E�� �Ń����o�̊��S��v���ǂ����Ԃ��B
				@return	true:�����o�����S��v false:�����o����ł��Ⴄ
				@note	STL�̃A���S���Y���p�ɒ�`
			*/
			bool operator==(Joint self)
			{
				return this->index == self.index&&this->weight == self.weight;
			}
		};
	}
}