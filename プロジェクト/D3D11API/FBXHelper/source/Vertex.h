#pragma once
#include "OriginalFormatMath.h"
#include <vector>
#include <unordered_map>

namespace FBX {
	namespace Utility{

		struct Vertex
		{
		public:
			inline Vertex() :position({ 0,0,0 }), uv({ -1,-1 }) {};
			~Vertex() {};

			Math::FLOAT3 position;
			Math::FLOAT2 uv;

			/*!
				NOTE:�Œ蒷�z��ł��ǂ������̂����A�{�[�����̑������f���̓ǂݍ��݂⓮��ۏ؂����������������B
				�܂��A�{�[���������Ȃ��ꍇ�͕`��R�X�g���팸�ł��錩���݁B
				
				���ʓr".ini"���ɔz��T�C�Y��f���o�������������B
			*/
			std::vector<unsigned int>indexOfBonesAffested;

			//	"indexOfBonesAffested"���L�[�ɑΉ�����d�݂��i�[
			std::unordered_map<unsigned int,float>weight;
		private:

		};
	}
}