/*
	@file	MeshReadHelper.h
	@date	2019/05/05
	@author	�ԏ� �G�P
	@brief	�O���t�@�C���̃��b�V���ǂݍ���
	@detail	�V���O���g��
*/
#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>
#include "MeshVertex.h"


#include <unordered_map>

namespace D3D11 {
	namespace Helper {
		class MeshReadHelper
		{
		public:
			/*!
				@struct	ReadBuffer
				@brief	�ǂݍ��񂾃o�b�t�@
			*/
			struct ReadBuffer
			{
				/*!
					@var	vertices
					@brief	���_
					@TODO	�O���t�@�C���ɂ�double�^�ŏ������܂�Ă��邽�߁Afloat�^�ɃL���X�g����
				*/
				std::vector<Graphic::MeshVertex>vertices;

				/*!
					@var	indices
					@brief	���_�C���f�b�N�X
				*/
				std::vector<uint32_t>indices;
			};

			struct AnimReadBuffer {
				unsigned int animCount;
				//n�Ԗڂ̃A�j���[�V�����̃t���[����
				std::unordered_map<unsigned int, unsigned int>frameCount;

				//n�Ԗڂ̃A�j���[�V�����̒��_
				std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Graphic::MeshVertex>>>vertices;
				std::vector<uint32_t>indices;
			};

			static ReadBuffer Read(std::string path);
			static AnimReadBuffer ReadAnim(std::string path);
		private:
			MeshReadHelper() = delete;
			~MeshReadHelper() = delete;

		};
	}
}