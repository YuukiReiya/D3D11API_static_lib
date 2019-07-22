#include "stdafx.h"
#include "AnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"
#include "MeshShader.h"

#pragma region シェーダー
#include <d3dcompiler.h>
#include "MeshConstantBuffer.h"

using namespace D3D11::Graphic;
using namespace D3D11;
class AnimMeshShader
	:public AbstractShader
{
public:
	AnimMeshShader();
	~AnimMeshShader();

	HRESULT Setup()override final { return E_FAIL; }
	HRESULT DynamicSetup()override final;

private:

};

AnimMeshShader::AnimMeshShader()
	:AbstractShader()
{
}

AnimMeshShader::~AnimMeshShader()
{
}

HRESULT AnimMeshShader::DynamicSetup()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr;

	//	パス
	auto path = L"shader.hlsl";
	
	//	シェーダー生成
	ID3DBlob *pCompileVS = NULL;
	ID3DBlob *pCompilePS = NULL;
	hr = D3DCompileFromFile(path, 0, 0, "VS", "vs_5_0", 0, 0, &pCompileVS, 0);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = dev.GetDevice()->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), 0, m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = D3DCompileFromFile(path, 0, 0, "PS", "ps_5_0", 0, 0, &pCompilePS, 0);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = dev.GetDevice()->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), 0, m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	//	頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = dev.GetDevice()->CreateInputLayout(layout, GetArraySize(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), m_pVertexLayout.GetAddressOf());
	if (FAILED(hr)) {
		pCompileVS->Release();
		pCompilePS->Release();
		return E_FAIL;
	}
	pCompileVS->Release();
	pCompilePS->Release();

	//	コンスタントバッファ
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	hr = dev.GetDevice()->CreateBuffer(&cb, 0, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	return S_OK;
}


#pragma endregion


HRESULT API::AnimationMesh::Init(std::string path)
{
	m_pShader = std::make_shared<MeshShader>();
	if (FAILED(m_pShader->Setup())) {
		return E_FAIL;
	}

	HRESULT hr;
	
	auto device = Direct3D11::GetInstance().GetDevice();
	auto context = Direct3D11::GetInstance().GetImmediateContext();

	//auto mesh = Helper::MeshReadHelper::ReadAnim(path);
	auto mesh = Helper::MeshReadHelper::ReadFrameAnim(path);
	m_IndexCount = mesh.indices.size();
	m_VertexCount = mesh.vertices[0][0].size();

	//for (int i = 0; i < mesh.animCount; ++i) {
	//	for (int j = 0; j < mesh.frameCount[i]; ++j) {
	//		m_VertexList[i][j] = mesh.vertices[i][j];
	//	}
	//}
	
	for (int i = 0; i < mesh.vertices.size(); ++i) {
		for (int j = 0; j < mesh.vertices[i].size(); ++j) {
			m_VertexList[i][j] = mesh.vertices[i][j];
		}
	}
	
#pragma region 頂点生成
	//D3D11_BUFFER_DESC vBD;
	//vBD.ByteWidth = sizeof(VERTEX) * m_VertexCount;
	//vBD.Usage = D3D11_USAGE_DYNAMIC;
	//vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//vBD.MiscFlags = 0;
	//vBD.StructureByteStride = 0;
	////	サブリソースの仕様
	//D3D11_SUBRESOURCE_DATA sd;
	////sd.pSysMem = mesh.vertices[0].v[0].data();

	//sd.pSysMem = m_VertexList[0][0].data();
	//hr = device->CreateBuffer(&vBD, &sd, m_pVertexBuffer.GetAddressOf());
	////hr = device->CreateBuffer(&vBD, 0, m_pVertexBuffer.GetAddressOf());
	//if (FAILED(hr)) {
	//	return E_FAIL;
	//}

	//if (FAILED(CreateVertexBuffer(m_VertexList[0][0]))) {
	//	ErrorLog("anim vertex");
	//}
	std::vector<MeshVertex> vertices;
	for (size_t i = 0; i < mesh.vertices[0][0].size(); i++)
	{
		auto& pos = mesh.vertices[0][0][i];
		//auto&uv = mesh.uv[i];
		DirectX::XMFLOAT2 uv = {-1,-1};
		MeshVertex v;
		v.position = pos;
		v.uv = uv;
		m_UV.push_back(uv);
		vertices.push_back(v);
	}


	if (FAILED(CreateVertexBuffer(vertices))) {
		ErrorLog("anim vertex");
	}
#pragma endregion

#pragma region 頂点インデックス
	//D3D11_BUFFER_DESC iBD;
	//iBD.ByteWidth = sizeof(m_IndexCount) * m_IndexCount;
	//iBD.Usage = D3D11_USAGE_DEFAULT;
	//iBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//iBD.CPUAccessFlags = 0;
	//iBD.MiscFlags = 0;
	//iBD.StructureByteStride = 0;
	//D3D11_SUBRESOURCE_DATA iData;
	//iData.pSysMem = mesh.indices.data();//頂点インデックス配列
	//hr = device->CreateBuffer(&iBD, &iData, m_pIndexBuffer.GetAddressOf());
	//if (FAILED(hr)) {
	//	return E_FAIL;
	//}

	if (FAILED(CreateIndexBuffer(mesh.indices))) {
		ErrorLog("anim index");
	}
#pragma endregion

#pragma region 設定
	//UINT stride = sizeof(VERTEX);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	//context->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//context->IASetInputLayout(*m_pShader->GetInputLayout());
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	//context->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	//context->VSSetShader(*m_pShader->GetVertexShader(), NULL, 0);
	//context->PSSetShader(*m_pShader->GetPixelShader(), NULL, 0);
#pragma endregion
	return S_OK;
}

void API::AnimationMesh::Render()
{
	//	トポロジー
	SetupTopology();

	//	コンスタントバッファ
	SetupConstantBuffer();

	//	シェーダーのバインド
	SetupBindShader();

	//	頂点レイアウトのセットアップ
	SetupInputLayout();

	//	頂点バッファのセットアップ
	SetupVertexBuffer();

	//	インデックスバッファのセットアップ
	SetupIndexBuffer();

	//	テクスチャ
	SetupTexture();

	//	描画命令
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);


}

HRESULT API::AnimationMesh::CreateVertexBuffer(std::vector<MeshVertex> verttices)
{
	//	バッファの仕様
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MeshVertex) * verttices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = NULL;

	//	サブリソースの仕様
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = verttices.data();

	//	頂点バッファ作成
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pVertexBuffer.GetAddressOf()
	);
}

HRESULT API::AnimationMesh::CreateIndexBuffer(std::vector<uint32_t> indices)
{
	//	頂点数保持
	m_IndexCount = indices.size();

	//	バッファの仕様
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t)*indices.size();
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = NULL;

	//	サブリソースの仕様	
	D3D11_SUBRESOURCE_DATA sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = indices.data();

	//	インデックスバッファ作成
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pIndexBuffer.GetAddressOf()
	);
}

void API::AnimationMesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void API::AnimationMesh::SetupInputLayout()
{
	try
	{
		//	shader
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetInputLayout()) {
			throw "this shader is inputlayout has an invalid value";
		}
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
			*m_pShader->GetInputLayout()
		);
	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return;
	}
}

void API::AnimationMesh::SetupIndexBuffer()
{
	static constexpr uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);

}

void API::AnimationMesh::SetupVertexBuffer()
{
	uint32_t stride = sizeof(MeshVertex);
	static constexpr uint32_t vertexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&vertexBufferOffset
	);

}

HRESULT API::AnimationMesh::SetupConstantBuffer()
{
	try
	{
		//	shader
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetConstantBuffer()) {
			throw "this shader is constant buffer has an invalid value";
		}

		HRESULT hr;
		DirectX::XMMATRIX w, v, p;

		//	world
		w = XMMatrixTranspose(transform->GetWorldMatrix());

		//	view
		v = XMMatrixTranspose(Camera::GetInstance().GetViewMatrix());

		//	proj
		p = XMMatrixTranspose(Camera::GetInstance().GetProjMatrix());

		//	cbuffer
		D3D11_MAPPED_SUBRESOURCE mapped;
		MeshConstantBuffer value;

		hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
			*m_pShader->GetConstantBuffer(),
			NULL,
			D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
			NULL,
			&mapped
		);
		if (FAILED(hr))
		{
			Direct3D11::GetInstance().GetImmediateContext()->Unmap(
				*m_pShader->GetConstantBuffer(),
				NULL
			);
			throw "Failed to map constant buffer for this shader";
		}

		//	データの格納
		value.m.world = w;
		value.m.view = v;
		value.m.proj = p;
		value.color = {1,0,0,1};

		//	メモリコピー
		memcpy_s(mapped.pData, mapped.RowPitch, (void*)(&value), sizeof(value));

		//	アクセス許可終了
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(
			*m_pShader->GetConstantBuffer(),
			NULL
		);

		//==================================================
		//	頂点バッファの書き換え
		auto context = Direct3D11::GetInstance().GetImmediateContext();
		hr = context->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		if (FAILED(hr)) {
			ErrorLog("anim map failed");
		}

		std::vector<MeshVertex>vVertList;
		for (size_t i = 0; i < m_UV.size(); i++)
		{
			MeshVertex v;
			v.position = m_VertexList[animIndex.x][animIndex.y][i];
			v.uv = m_UV[i];
			vVertList.push_back(v);
		}
		memcpy_s(
			mapped.pData,
			mapped.RowPitch,
			(void*)(vVertList.data()),
			sizeof(Graphic::MeshVertex) * m_VertexCount
		);
		context->Unmap(m_pVertexBuffer.Get(), 0);
		//==================================================


		//	コンスタントバッファをシェーダーに登録

		//	頂点シェーダー
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
			0,
			1,
			m_pShader->GetConstantBuffer()
		);

		//	ピクセルシェーダー
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
			0,
			1,
			m_pShader->GetConstantBuffer()
		);

	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return E_FAIL;
	}


	return S_OK;
}

void API::AnimationMesh::SetupBindShader()
{
	try
	{
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetVertexShader()) {
			throw "this shader is vertex shader has an invalid value";
		}
		if (!*m_pShader->GetPixelShader()) {
			throw "this shader is pixel shader has an invalid value";
		}

		//	頂点シェーダー
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
			*m_pShader->GetVertexShader(),
			NULL,
			0
		);

		//	ピクセルシェーダー
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
			*m_pShader->GetPixelShader(),
			NULL,
			0
		);
	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return;
	}

}

void API::AnimationMesh::SetupTexture()
{
}

