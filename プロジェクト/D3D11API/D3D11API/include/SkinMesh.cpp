#include "stdafx.h"
#include "SkinMesh.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "SkinnedVertex.h"
#include "SimpleCompVertex.h"
#include "SimpleCompPixel.h"
#include "MeshConstantBuffer.h"
#include "Camera.h"
#include "SkinnedVertex.h"
#include "MeshReadHelper.h"

using namespace API;
using namespace std;
using namespace D3D11;
using namespace DirectX;

#pragma region シェーダー
class Shader
	:public Graphic::AbstractShader
{
public:
	explicit Shader() :AbstractShader() {}
	~Shader() {}
	HRESULT Setup()override final;
	HRESULT DynamicSetup()override { return E_FAIL; }
private:

};
HRESULT Shader::Setup()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT	hr = E_FAIL;
#pragma region 頂点レイアウト

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	hr = dev.GetDevice()->CreateInputLayout(
		desc,
		GetArraySize(desc),
		g_vs_main,
		sizeof(g_vs_main),
		m_pVertexLayout.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("input layout"); }
#pragma endregion

#pragma region 頂点シェーダー
	hr = dev.GetDevice()->CreateVertexShader(
		&g_vs_main,
		sizeof(g_vs_main),
		NULL,
		m_pVertexShader.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("vertex shader"); }
#pragma endregion

#pragma region ピクセルシェーダー
	hr = dev.GetDevice()->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("pixel shader"); }
#pragma endregion

#pragma region 定数バッファ
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	hr = dev.GetDevice()->CreateBuffer(
		&bd,
		NULL,
		m_pConstantBuffer.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("constant buffer"); }
#pragma endregion

	return S_OK;
}


#pragma endregion

API::SkinMesh::SkinMesh()
{
	transform = make_shared<Transform>();
}

API::SkinMesh::~SkinMesh()
{
}

void API::SkinMesh::Init()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr = E_FAIL;

	m_pShader = make_shared<Shader>();
	m_pShader->Setup();

#pragma region 定数読み込み
#if 1
#define CONST_READ
#pragma region インデックスバッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*GetArraySize(c_Indices);
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = NULL;

		//	サブリソースの仕様	
		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = c_Indices;
		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pIndexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("index buffer"); }
	}
#pragma endregion

#pragma region 頂点バッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SkinnedVertex) * GetArraySize(c_VerticesPosition);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = NULL;

		//	サブリソースの仕様
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = c_VerticesPosition;
 
		hr =dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pVertexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("vertex"); }
	}
#pragma endregion
#endif // 0
#pragma endregion

#pragma region 外部ファイル読み込み
#if 0
	auto data = Helper::MeshReadHelper::ReadSkin("test.yfm");
#pragma region インデックスバッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*data.indices.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = NULL;

		//	サブリソースの仕様	
		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = data.indices.data();
		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pIndexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("index buffer"); }
	}
#pragma endregion

#pragma region 頂点バッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SkinnedVertex) * data.vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = NULL;

		//	サブリソースの仕様
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data.vertices.data();

		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pVertexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("vertex"); }
	}
#pragma endregion

#pragma region 合成行列
	m_CompositeMatrix.resize(data.vertices.size());
	//	フレーム数
	for (size_t i = 0; i < data.vertices[0].compMat.size(); i++)
	{
		//	頂点数
		for (size_t j = 0; j < data.vertices.size(); j++)
		{
			m_CompositeMatrix[i].push_back(data.vertices[j].compMat[i]);
		}
	}
#pragma endregion
#endif // 0
#pragma endregion

}

void API::SkinMesh::Render()
{
	HRESULT hr = E_FAIL;
	auto& dev = Direct3D11::GetInstance();

#pragma region トポロジー
	dev.GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
#pragma endregion

#pragma region 定数バッファ
	auto&camera = Camera::GetInstance();
	XMMATRIX w, v, p;
	w = XMMatrixTranspose(transform->GetWorldMatrix());
	v = XMMatrixTranspose(camera.GetViewMatrix());
	p = XMMatrixTranspose(camera.GetProjMatrix());

	D3D11_MAPPED_SUBRESOURCE ms;
	D3D11::Graphic::MeshConstantBuffer cb;

	hr = dev.GetImmediateContext()->Map(
		*m_pShader->GetConstantBuffer(),
		NULL,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		NULL,
		&ms
	);
	if (FAILED(hr)) { ErrorLog("map"); }
	cb.m.world = w;
	cb.m.view = v;
	cb.m.proj = p;
	cb.color = { 1,1,1,1 };

	memcpy_s(ms.pData, ms.RowPitch, (void*)(&cb), sizeof(cb));
	dev.GetImmediateContext()->Unmap(*m_pShader->GetConstantBuffer(), NULL);
#pragma endregion

#pragma region シェーダー
	dev.GetImmediateContext()->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->VSSetShader(*m_pShader->GetVertexShader(), 0, 0);
	dev.GetImmediateContext()->PSSetShader(*m_pShader->GetPixelShader(), 0, 0);
#pragma endregion

#pragma region 頂点レイアウト
	dev.GetImmediateContext()->IASetInputLayout(*m_pShader->GetInputLayout());
#pragma endregion

#pragma region 頂点計算
#if 1


	hr = dev.GetImmediateContext()->Map(
		m_pVertexBuffer.Get(),
		0,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);

	//	頂点宣言
	vector<SkinnedVertex> vs;

#pragma region スキニング計算
#if 1
	vector<XMMATRIX> compMat;
	compMat.resize(GetArraySize(c_VerticesPosition));
	for (size_t i = 0; i < GetArraySize(c_VerticesPosition); i++)
	{
		//ボーン
		for (size_t j = 0; j < GetArraySize(c_Joints[i]); j++)
		{
			uint32_t jointIndex = c_Joints[i][j].index;
			auto skMat = XMLoadFloat4x4(&c_SkinMats[0][jointIndex]);
			float w = c_Joints[i][j].weight;
			compMat[i] += skMat * w;
		}
	}

	for (size_t i = 0; i < GetArraySize(c_VerticesPosition); i++)
	{
		XMVECTOR vec = {
		c_VerticesPosition[i].x,
		c_VerticesPosition[i].y,
		c_VerticesPosition[i].z,
		1
		};
		vec= XMVector4Transform(vec, compMat[i]);
		SkinnedVertex v =
		{
			{vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2]}
		};
		vs.push_back(v);
	}

#endif // 1

#pragma endregion


#pragma region 合成行列
#if 0
	
//	頂点編集
	for (size_t i = 0; i < GetArraySize(c_CompositeMatrix); i++)
	{
		XMVECTOR vec = {
			c_VerticesPosition[i].x,
			c_VerticesPosition[i].y,
			c_VerticesPosition[i].z,
			1
		};
#ifdef CONST_READ
		XMMATRIX m = XMLoadFloat4x4(&c_CompositeMatrix[i]);
#else
		XMMATRIX m = XMLoadFloat4x4(&m_CompositeMatrix[frameIndex][i]);
#endif // DEBUG
		
		vec = XMVector4Transform(vec, m);
		SkinnedVertex v = 
		{
			{vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2]}
		};
		vs.push_back(v);
	}
#endif // 0
#pragma endregion

	//	メモリコピー
	memcpy_s(ms.pData, ms.RowPitch, (void*)(vs.data()), sizeof(SkinnedVertex) * GetArraySize(c_VerticesPosition));

	dev.GetImmediateContext()->Unmap(m_pVertexBuffer.Get(), 0);
#endif // 1
#pragma endregion

#pragma region 頂点バッファ
	uint32_t stride = sizeof(SkinnedVertex);
	static constexpr uint32_t vertexBufferOffset = 0;
	dev.GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&vertexBufferOffset
	);
#pragma endregion

#pragma region 頂点インデックス
	static constexpr uint32_t indexBufferOffset = 0;
	dev.GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);
#pragma endregion

	dev.GetImmediateContext()->DrawIndexed(GetArraySize(c_Indices), 0, 0);
}

