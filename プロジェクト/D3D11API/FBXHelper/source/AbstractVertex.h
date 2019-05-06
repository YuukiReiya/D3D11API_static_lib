/*
	@file	AbstractVertex.h
	@date	2019/04/25
	@author	�ԏ� �G�P
	@brief	���_�̒��ۃN���X
	@detail	�Œ���K�v�Ȓ��_����ێ����Ă���
*/
#pragma once
namespace Abstract{
	 class AbstractVertex abstract
	{
	public:

		/*!
			@brief	�R���X�g���N�^
		*/
		AbstractVertex() :x(0), y(0), z(0), w(0), normal(Normal()), uv(UV()) {}

		/*!
			@brief	�f�X�g���N�^
		*/
		virtual ~AbstractVertex() { normal = Normal(), uv = UV(); };

		/*!
			@var	x,y,z,w
			@brief	3������Ԃ̍��W�ɊY��
			@detail	'w'�͍s��ɍ��킹�Đ錾�B
					��{�I�� "w = 1"
		*/
		double x, y, z, w;

		/*!
			@struct	Normal
			@brief	�@�������܂ލ\����
		*/
		struct Normal
		{
			/*!
				@var	x,y,z
				@brief	�@���x�N�g��
			*/
			double x, y, z;

			/*!
				@brief	�R���X�g���N�^
			*/
			Normal():x(0),y(0),z(0){}
		}normal;

		/*!
			@struct	UV
			@brief	UV�����܂ލ\����
		*/
		struct UV
		{
			/*!
				@var	x,y
				@brief	UV���W
			*/
			float x, y;

			/*!
				@brief	�R���X�g���N�^
			*/
			UV() :x(0), y(0) {}
		}uv;
	};
}
