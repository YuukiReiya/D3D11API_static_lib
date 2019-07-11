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
#include <unordered_map>
#include <fstream>
#include "MeshVertex.h"


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
			static ReadBuffer Read(std::string path);

			struct SkeltonMesh
			{
				SkeltonMesh() :vertices(), indices(0), maxBonesElementsCount(0), frame(0) {};
				~SkeltonMesh() = default;

				int frame;
				struct Vertex
				{
				public:
					inline Vertex() :position({ 0,0,0 }), uv({ -1,-1 }) {};
					~Vertex() {};

					DirectX::XMFLOAT3 position;
					DirectX::XMFLOAT2 uv;

					/*!
						NOTE:�Œ蒷�z��ł��ǂ������̂����A�{�[�����̑������f���̓ǂݍ��݂⓮��ۏ؂����������������B
						�܂��A�{�[���������Ȃ��ꍇ�͕`��R�X�g���팸�ł��錩���݁B

						���ʓr".ini"���ɔz��T�C�Y��f���o�������������B
					*/
					std::vector<unsigned int>indexOfBonesAffested;

					//	"indexOfBonesAffested"���L�[�ɑΉ�����d�݂��i�[
					std::unordered_map<unsigned int, float>weight;
				};


				std::vector<Vertex> vertices;
				std::vector<unsigned int>indices;

				//	�{�[�����H
				std::vector<DirectX::XMFLOAT4X4> initialMatrix;
				std::vector<std::vector<DirectX::XMFLOAT4X4>> frameMatrix;
				unsigned int maxBonesElementsCount;//�ێ����钸�_�̍ő�"�֘A�{�[��"��
			};

			static SkeltonMesh ReadAnim(std::string path);
		private:
			MeshReadHelper() = delete;
			~MeshReadHelper() = delete;

		};
	}
}