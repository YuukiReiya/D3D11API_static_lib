#pragma once
#include <wrl/client.h>
#include <memory>
#include <vector>
#include "AbstractRender.h"
#include "MeshVertex.h"
#include "MeshShader.h"
#include "Transform.h"
#include "Color.h"
#include "StructShaderBase.h"
#include "MeshVertex.h"

namespace API {

	namespace Anim {

		class SkeltonAnimationMesh
			: public AbstractRender
		{
		public:
			SkeltonAnimationMesh() {};
			~SkeltonAnimationMesh() { Destroy(); };

			HRESULT Init();
			void Destroy();
			void Render()override;
			std::shared_ptr<Transform>transform;
			Color color;
		private:
			std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

			uint32_t m_IndexCount;

			std::vector<D3D11::Graphic::MeshVertex>m_Vertex;

			template<class Vertex>
			HRESULT CreateVertexBuffer(std::vector<Vertex>vertices);
			HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);
		};
	}
}
