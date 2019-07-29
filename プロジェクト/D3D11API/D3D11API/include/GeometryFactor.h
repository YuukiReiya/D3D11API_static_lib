#pragma once
#include <memory>
#include "Singleton.h"
#include "MeshVertex.h"
#include "Mesh.h"

namespace API
{
	class GeometryFactor
		:public Singleton<GeometryFactor>
	{
	public:

		/*!
			@brief	�f�X�g���N�^
		*/
		~GeometryFactor();

		std::shared_ptr<API::Mesh>CreateCube();

	private:
		/*!
			@brief	�R���X�g���N�^
		*/
		explicit GeometryFactor();

		/*!
			@brief	�V���O���g���f�U�C���p�^�[���̃e���v���[�g�p��
		*/
		friend class Singleton<GeometryFactor>;
	};
}