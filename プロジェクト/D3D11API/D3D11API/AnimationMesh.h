#pragma once
#include "include/AbstractRender.h"
#include "include/AbstractShader.h"
#include <memory>
#include <DirectXMath.h>
#include "include/MeshVertex.h"
#include <wrl/client.h>
#include "include/Transform.h"

namespace API {
#pragma region í∏ì_ç\ë¢ëÃ
	struct VERTEX {
		DirectX::XMFLOAT3 pos;
	};
#pragma endregion

	class AnimationMesh
		:public API::AbstractRender
	{
	public:
		AnimationMesh() {}
		~AnimationMesh() { delete[]m_pVertex; };

		HRESULT Init(std::string path);
		void Render()override;

		//void AddAnimIndex() {
		//	animIndex.x = (size_t)animIndex.x + 1 < m_VertexList.size() ? ++animIndex.x : 0;
		//}
		//void AddFrameIndex() {
		//	animIndex.y = (size_t)animIndex.y + 1 < m_VertexList[animIndex.x].size() ? ++animIndex.y : 0;
		//}
		Transform transform;
	private:
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		uint32_t m_IndexCount;
		uint32_t m_VertexCount;
		VERTEX* m_pVertex;
	};

}
