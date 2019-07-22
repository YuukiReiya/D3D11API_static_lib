#pragma once
#include "AbstractRender.h"
#include "AbstractShader.h"
#include <memory>
#include <DirectXMath.h>
#include "MeshVertex.h"
#include <wrl/client.h>
#include "Transform.h"
#include <unordered_map>

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
		AnimationMesh() :animIndex({0,0}) { transform = std::make_shared<Transform>(); }
		~AnimationMesh() { m_VertexList.clear(); };

		HRESULT Init(std::string path);
		void Render()override;

		void AddAnimIndex() {
			animIndex.x = (size_t)animIndex.x + 1 < m_VertexList.size() ? ++animIndex.x : 0;
		}
		void AddFrameIndex() {
			animIndex.y = (size_t)animIndex.y + 1 < m_VertexList[animIndex.x].size() ? ++animIndex.y : 0;
		}
		std::shared_ptr<Transform>transform;
	private:
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

		HRESULT CreateVertexBuffer(std::vector<D3D11::Graphic::MeshVertex>verttices);
		HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);

		void SetupTopology();
		void SetupInputLayout();		
		void SetupIndexBuffer();
		void SetupVertexBuffer();
		HRESULT SetupConstantBuffer();
		void SetupBindShader();
		void SetupTexture();



		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		uint32_t m_IndexCount;
		uint32_t m_VertexCount;

		DirectX::XMINT2 animIndex;

		//std::unordered_map<size_t, std::unordered_map<size_t, std::vector<D3D11::Graphic::MeshVertex>>>m_VertexList;
		std::unordered_map<size_t, std::unordered_map<size_t, std::vector<DirectX::XMFLOAT3>>>m_VertexList;
		std::vector<DirectX::XMFLOAT2>m_UV;
	};



}
