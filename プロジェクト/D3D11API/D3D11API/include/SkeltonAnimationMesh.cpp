#include "stdafx.h"
#include "SkeltonAnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MatrixConstantBuffer.h"
#include "MyGame.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include <DirectXMath.h>
#include "MeshReadHelper.h"
#include "MeshConstantBuffer.h"
#include "MeshShader.h"

using namespace D3D11;
using namespace D3D11::Graphic;
using namespace std;
using namespace DirectX;

#pragma region 定数バッファ
//struct alignas(16) AnimConstantBuffer
//{
//	MatrixConstantBuffer m;
//	DirectX::XMMATRIX boneMat[60];
//};
#pragma endregion

#pragma region ボーン
//struct Bone
//{
//	unsigned int id;
//	Bone* child;
//	Bone* sibling;
//	XMMATRIX offsetMat;
//	XMMATRIX initMat;
//	XMMATRIX boneMat;
//	XMMATRIX* combMatPtr;
//	Bone() :
//		id(),
//		child(),
//		sibling(),
//		combMatPtr(),
//		initMat(XMMatrixIdentity()),
//		offsetMat(XMMatrixIdentity()),
//		boneMat(XMMatrixIdentity())
//	{}
//};
#pragma endregion


#pragma region シェーダーオブジェクト
//class AnimShader
//	: public AbstractShader
//{
//public:
//	AnimShader():AbstractShader() {}
//	~AnimShader() {}
//
//
//	HRESULT Setup()override final;
//	HRESULT DynamicSetup()override final { return E_FAIL; }
//
//private:
//
//};
//HRESULT AnimShader::Setup()
//{
//	HRESULT hr;
//	auto device = D3D11::Direct3D11::GetInstance().GetDevice();
//#pragma region 頂点レイアウト
//	D3D11_INPUT_ELEMENT_DESC desc[] = {
//			{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,	0,	0							,D3D11_INPUT_PER_VERTEX_DATA,0},
//			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//			{ "COLOR",	 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//			//{ "BLENDWEIGHT",	0,DXGI_FORMAT_R32G32B32_FLOAT,  0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//			//{ "BLENDINDICES",	0,DXGI_FORMAT_R8_UINT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//	};
//	
//	hr = device->CreateInputLayout(
//		desc,
//		GetArraySize(desc),
//		g_vs_main,
//		sizeof(g_vs_main),
//		m_pVertexLayout.GetAddressOf()
//	);
//	if (FAILED(hr)) {
//		ErrorLog("create inputlayout");
//		return E_FAIL; 
//	}
//#pragma endregion

#pragma region 頂点シェーダー
	//hr = device->CreateVertexShader(
	//	&g_vs_main,
	//	sizeof(g_vs_main),
	//	NULL,
	//	m_pVertexShader.GetAddressOf()
	//);
	//if (FAILED(hr)) {
	//	ErrorLog("create vs");
	//	return E_FAIL; 
	//}
#pragma endregion

#pragma region ピクセルシェーダー
	//hr = device->CreatePixelShader(
	//	&g_ps_main,
	//	sizeof(g_ps_main),
	//	NULL,
	//	m_pPixelShader.GetAddressOf()
	//);
	//if (FAILED(hr)) {
	//	ErrorLog("create ps");
	//	return E_FAIL;
	//}
#pragma endregion

#pragma region 定数バッファ
//	D3D11_BUFFER_DESC cb;
//	cb.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
//	//cb.ByteWidth = sizeof(AnimConstantBuffer);
//	cb.ByteWidth = sizeof(MeshConstantBuffer);
//	cb.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
//	cb.MiscFlags = 0;
//	cb.StructureByteStride = 0;
//	cb.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
//	hr = device->CreateBuffer(
//		&cb,
//		NULL,
//		m_pConstantBuffer.GetAddressOf()
//	);
//	if (FAILED(hr)) {
//		ErrorLog("create cb");
//		return E_FAIL;
//	}
//
#pragma endregion
//
//
//	return S_OK;
//}
//

#pragma endregion

//====================================================-
static constexpr int c_BoneCount = 60;
//Bone g_Bones[c_BoneCount];
vector<D3D11::Graphic::MeshVertex>g_pVertex;
//====================================================-
HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	//auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");
	auto data = Helper::MeshReadHelper::Read("twin.yfm");

	//	頂点
	g_pVertex = data.vertices;

	//	重み
	//g_pVertex = vv;

	//	頂点インデックス
	vector<uint32_t>vi;
	vi = data.indices;

	if (FAILED(CreateVertexBuffer(g_pVertex))) {
		ErrorLog("falied to create vertex");
	}

	if (FAILED(CreateIndexBuffer(vi))) {
		ErrorLog("falied to create index");
	}

	//	シェーダー
	m_pShader = make_shared<MeshShader>();
	if (FAILED(m_pShader->Setup())) {
		ErrorLog("falied to setup shader");
	}

	//	トランスフォーム
	transform = make_shared<Transform>();

	return E_NOTIMPL;
}

void API::Anim::SkeltonAnimationMesh::Destroy()
{
}

void API::Anim::SkeltonAnimationMesh::Render()
{
#pragma region トポロジー
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region 定数バッファ
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mp;
	DirectX::XMMATRIX w, v, p;
#pragma region WVP
	//	world
	w = XMMatrixTranspose(transform->GetWorldMatrix());

	//	view
	v = DirectX::XMMatrixTranspose(Camera::GetInstance().GetViewMatrix());

	//	proj
	p = DirectX::XMMatrixTranspose(Camera::GetInstance().GetProjMatrix());
#pragma endregion

#pragma region WVP_cpy
	//AnimConstantBuffer cb;
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
#pragma endregion

	//	メモリコピー
	memcpy_s(mp.pData, mp.RowPitch, (void*)(&cb), sizeof(cb));

	//	アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*m_pShader->GetConstantBuffer(),
		NULL
	);

#pragma region 頂点の書き換え

	auto context = Direct3D11::GetInstance().GetImmediateContext();
	hr = context->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mp);
	if (FAILED(hr)) {
		ErrorLog("anim map failed");
	}

	//
	//	頂点の書き換え
	//
	{



	}


	memcpy_s(
		mp.pData,
		mp.RowPitch,
		(void*)(g_pVertex.data()),
		sizeof(Graphic::MeshVertex) * g_pVertex.size()
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
	uint32_t stride = sizeof(AnimVertex);
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

HRESULT API::Anim::SkeltonAnimationMesh::CreateVertexBuffer(std::vector<AnimVertex> verttices)
{
	//	バッファの仕様
	D3D11_BUFFER_DESC bd;

	/*
		頂点を動的に書き換える場合は "DYNAMIC"にすること！
	*/
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AnimVertex) * verttices.size();
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

	return E_NOTIMPL;
}

HRESULT API::Anim::SkeltonAnimationMesh::CreateVertexBuffer(std::vector<MeshVertex>verttices)
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