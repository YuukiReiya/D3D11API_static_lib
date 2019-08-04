/*!
	@file	AnimationMesh.cpp
	@date	2019/08/04
	@author	番場 宥輝
	@brief	フレームアニメーションメッシュ
*/
#include "stdafx.h"
#include "AnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"
#include "MeshShader.h"
#include "MeshConstantBuffer.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	D3D11::Graphic
*/
using namespace Graphic;

/*!
	@fn			Initialize
	@brief		初期化
	@param[in]	読み込みパス
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::AnimationMesh::Initialize(std::string path)
{
	m_pShader = std::make_shared<MeshShader>();
	if (FAILED(m_pShader->Setup())) {
		return E_FAIL;
	}

	HRESULT hr;
	auto device = Direct3D11::GetInstance().GetDevice();
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	auto mesh = Helper::MeshReadHelper::ReadFrameAnim(path);
	m_IndexCount = mesh.indices.size();
	m_VertexCount = mesh.vertices[0][0].size();

	for (int i = 0; i < mesh.vertices.size(); ++i) {
		for (int j = 0; j < mesh.vertices[i].size(); ++j) {
			m_VertexList[i][j] = mesh.vertices[i][j];
		}
	}
	
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

	if (FAILED(CreateVertexBuffer(vertices))) { return E_FAIL; }
	if (FAILED(CreateIndexBuffer(mesh.indices))) { return E_FAIL; }
	return S_OK;
}

/*!
	@fn		Render
	@brief	描画
*/
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

	//	描画命令
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
}

/*!
	@fn			CreateVertexBuffer
	@brief		頂点バッファの作成
	@param[in]	可変長配列に格納した頂点情報
	@return		成功:S_OK 失敗:E_FAIL
*/
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

/*!
	@fn			CreateIndexBuffer
	@brief		インデックスバッファの作成
	@param[in]	可変長配列に格納したインデックス情報
	@return		成功:S_OK 失敗:E_FAIL
*/
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

/*!
	@fn		SetupTopology
	@brief	トポロジーの設定
*/
void API::AnimationMesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*!
	@fn		SetupInputLayout
	@brief	頂点レイアウトの設定
*/
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

/*!
	@fn		SetupIndexBuffer
	@brief	インデックスバッファの設定
*/
void API::AnimationMesh::SetupIndexBuffer()
{
	static constexpr uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);

}

/*!
	@fn		SetupVertexBuffer
	@brief	頂点バッファの設定
*/
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

/*!
	@fn		SetupConstantBuffer
	@brief	コンスタントバッファの設定
	@return	成功:S_OK 失敗:E_FAIL
*/
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
		value.matrix.world = w;
		value.matrix.view = v;
		value.matrix.proj = p;
		value.color = {1,1,1,1};

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
			v.position = m_VertexList[m_AnimIndex.x][m_AnimIndex.y][i];
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

/*!
	@fn		SetupBindShader
	@brief	シェーダーの設定
*/
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