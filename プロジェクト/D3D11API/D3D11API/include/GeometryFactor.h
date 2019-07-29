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
			@brief	デストラクタ
		*/
		~GeometryFactor();

		std::shared_ptr<API::Mesh>CreateCube();

	private:
		/*!
			@brief	コンストラクタ
		*/
		explicit GeometryFactor();

		/*!
			@brief	シングルトンデザインパターンのテンプレート継承
		*/
		friend class Singleton<GeometryFactor>;
	};
}