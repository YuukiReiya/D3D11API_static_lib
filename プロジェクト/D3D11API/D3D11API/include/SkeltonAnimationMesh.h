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

		//	頂点
		struct alignas(16) AnimVertex
			:D3D11::Graphic::BaseVertex
		{
			//	ボーン数
			static constexpr int c_AbsBoneCount = 7;

			//DirectX::XMFLOAT3 weight;
			//BYTE bornIndex[c_AbsBoneCount];

			////	コンストラクタ
			//AnimVertex()
			//	:D3D11::Graphic::BaseVertex(),
			//	weight({0,0,0}),
			//	bornIndex()
			//{}
			////	引数付きコンストラクタ
			//inline AnimVertex(DirectX::XMFLOAT3 pos)
			//	: D3D11::Graphic::BaseVertex(pos),
			//	weight({0,0,0}),
			//	bornIndex()
			//{}

			////	引数付きコンストラクタ
			//inline AnimVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 weight,int index[])
			//	: D3D11::Graphic::BaseVertex(pos),
			//	weight(weight)
			//{
			//	for (int i = 0; i < 4; ++i) { bornIndex[i] = index[i]; }
			//}

		};

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

			HRESULT CreateVertexBuffer(std::vector<AnimVertex>verttices);
			HRESULT CreateVertexBuffer(std::vector<D3D11::Graphic::MeshVertex>verttices);
			HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);



			uint32_t m_IndexCount;
		};


	}
}
