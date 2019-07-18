#include "stdafx.h"
#include "SkeltonAnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MatrixConstantBuffer.h"
#include "MyGame.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include "MeshReadHelper.h"
#include "MeshConstantBuffer.h"
#include "MeshShader.h"

using namespace D3D11;
using namespace D3D11::Graphic;
using namespace std;
using namespace DirectX;

HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");
	
#pragma region 読み込みデータのノーマライズ

	//	頂点(初期姿勢)
	for (auto it : data.vertices) {
		MeshVertex v;
		v.position = it.position;
		m_Vertex.push_back(v);
	}
	//m_Vertex = data.vertices;

	//	オフセット行列
	for (auto it : data.initialMatrix)
	{
		XMMATRIX ofMat = XMMatrixInverse(0, XMLoadFloat4x4(&it));
		m_OffsetMatrix.push_back(ofMat);
	}

	//	フレーム行列のスタック確保
	m_FrameMatrix.resize(data.initialMatrix.size());
	for (auto& it : m_FrameMatrix)
	{
		it.resize(30);
	}

	//	フレーム行列
	for (int frame = 0; frame < data.frame; ++frame)
	{
		for (int bone = 0; bone < data.initialMatrix.size(); ++bone) {
			XMMATRIX mat = XMLoadFloat4x4(&data.frameMatrix[frame][bone]);
			m_FrameMatrix[bone][frame] = mat;
		}
	}
	
	//	重み & 関連ボーン番号
	for (auto v : data.vertices) {

		RelationInfo ri;
		ri.boneNo = v.indexOfBonesAffested;
		for (auto it : v.indexOfBonesAffested) {
			ri.weight.push_back(v.weight[it]);
		}
		m_RI.push_back(ri);
	}

#pragma endregion


	//	頂点バッファ
	if (FAILED(CreateVertexBuffer(m_Vertex))) {
		ErrorLog("create vertex");
	}

	//	インデックス
	if (FAILED(CreateIndexBuffer(data.indices))) {
		ErrorLog("create index");
	}

	//	シェーダー
	m_pShader = make_shared<MeshShader>();
	m_pShader->Setup();

	//	トランスフォーム
	transform = make_shared<Transform>();

	return E_NOTIMPL;
}

void API::Anim::SkeltonAnimationMesh::Destroy()
{
}

#include "Keyboard.h"
void API::Anim::SkeltonAnimationMesh::Render()
{
	HRESULT hr;

#pragma region トポロジー
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region GPU送信
	D3D11_MAPPED_SUBRESOURCE mp;

#pragma region WVP
	DirectX::XMMATRIX w, v, p;
	//	world
	w = XMMatrixTranspose(transform->GetWorldMatrix());

	//	view
	v = DirectX::XMMatrixTranspose(Camera::GetInstance().GetViewMatrix());

	//	proj
	p = DirectX::XMMatrixTranspose(Camera::GetInstance().GetProjMatrix());
#pragma endregion

#pragma region 定数バッファ送信
	MeshConstantBuffer cb;

	hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		*m_pShader->GetConstantBuffer(),
		NULL,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		0,
		&mp
	);
	if (FAILED(hr))
	{
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(
			*m_pShader->GetConstantBuffer(),
			NULL
		);
		ErrorLog("failed to map");
	}

	cb.m.world = w;
	cb.m.view = v;
	cb.m.proj = p;

	//	メモリコピー
	memcpy_s(mp.pData, mp.RowPitch, (void*)(&cb), sizeof(cb));

	//	アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*m_pShader->GetConstantBuffer(),
		NULL
	);
#pragma endregion

#pragma region 頂点の書き換え

	auto context = Direct3D11::GetInstance().GetImmediateContext();
	hr = context->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mp);
	if (FAILED(hr)) {
		ErrorLog("vertex map failed");
	}

	//
	//	頂点の書き換え
	//
	{
		//	確認用コード
		//if (Keyboard::GetButtonDown('M')) {
		//	m_Vertex[0].position = { 0,0,0 };
		//	m_Vertex[1604].position = { 0,0,0 };
		//}


		//	ボーン行列を求める
		//	BM = offsetMat * FrameMatrix
		vector<XMMATRIX>boneMat;
		for (int i = 0; i < m_FrameMatrix.size(); ++i)
		{
			XMMATRIX mat = m_OffsetMatrix[i] * m_FrameMatrix[i][0];
			boneMat.push_back(mat);
		}

		//	頂点
		for (int vCount = 0; vCount < m_Vertex.size(); ++vCount)
		{
			XMMATRIX compMat = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
			float lastWeight = 0;//	最後の重み
			int size = m_RI[vCount].boneNo.size() - 1;
			for (int i = 0; i < size; ++i)
			{
				float weight = m_RI[vCount].weight[i];
				int index = m_RI[vCount].boneNo[i];
				lastWeight += weight;
				compMat += weight * boneMat[index];
			}
			compMat += boneMat[size] * (1.0f - lastWeight);

			//compMat = XMMatrixTranspose(compMat);
			m_Vertex[vCount].position = 
			{
				compMat.r[3].m128_f32[0],
				compMat.r[3].m128_f32[1],
				compMat.r[3].m128_f32[2],
			};
		}
	}

	memcpy_s(
		mp.pData,
		mp.RowPitch,
		(void*)(m_Vertex.data()),
		sizeof(Graphic::MeshVertex) * m_Vertex.size()
	);
	context->Unmap(m_pVertexBuffer.Get(), 0);

#pragma endregion


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


#pragma endregion

#pragma region シェーダーのバインド
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*m_pShader->GetVertexShader(),
		0, 0
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*m_pShader->GetPixelShader(),
		0, 0
	);
#pragma endregion

#pragma region 頂点レイアウト
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		*m_pShader->GetInputLayout()
	);
#pragma endregion

#pragma region 頂点バッファ
	uint32_t stride = sizeof(MeshVertex);
	static constexpr uint32_t vertexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&vertexBufferOffset
	);

#pragma endregion

#pragma region インデックスバッファ
	static constexpr uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);

#pragma endregion

	//描画
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
}


template<class Vertex>
HRESULT API::Anim::SkeltonAnimationMesh::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	//	バッファの仕様
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = NULL;

	//	サブリソースの仕様
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = vertices.data();

	//	頂点バッファ作成
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pVertexBuffer.GetAddressOf()
	);
}

HRESULT API::Anim::SkeltonAnimationMesh::CreateIndexBuffer(std::vector<uint32_t> indices)
{
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
