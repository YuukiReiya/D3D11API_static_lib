#pragma once
#include <wrl/client.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "AbstractRender.h"
#include "MeshVertex.h"
#include "MeshShader.h"
#include "Transform.h"
#include "Color.h"
#include "StructShaderBase.h"

namespace API {

	namespace Anim {

		static constexpr unsigned int c_AffectedBoneCount = 4;

		//	頂点
		struct alignas(16) AnimVertex
			:D3D11::Graphic::BaseVertex
		{
			//std::unordered_map<unsigned int,float> weights;
			float weights[c_AffectedBoneCount];
			BYTE bornIndex[c_AffectedBoneCount];

			//	コンストラクタ
			AnimVertex()
				:D3D11::Graphic::BaseVertex(),
				bornIndex()
			{}
			//	引数付きコンストラクタ
			inline AnimVertex(DirectX::XMFLOAT3 pos)
				: D3D11::Graphic::BaseVertex(pos),
				bornIndex()
			{}

			//	引数付きコンストラクタ
			inline AnimVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 weight,int index[])
				: D3D11::Graphic::BaseVertex(pos)
			{
				for (int i = 0; i < 4; ++i) { bornIndex[i] = index[i]; }
			}

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

			int animIndex = 0;
		private:
			std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

			HRESULT CreateVertexBuffer(std::vector<AnimVertex>verttices);
			HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);

			uint32_t m_IndexCount;
		};


	}
}
