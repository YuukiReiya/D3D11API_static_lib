#include "stdafx.h"
#include "Mesh.h"
#include "MeshVertex.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "MemoryLeaks.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include "WICTextureLoader.h"
#include "MeshConstantBuffer.h"
#include "MeshReadHelper.h"
#include "Camera.h"

/*!
	@brief	usingディレクティブ
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	usingディレクティブ
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	コンストラクタ
*/
Mesh::Mesh() :m_IndexCount(0) {}

/*!
	@brief	デストラクタ
	@detail	開放処理の呼び出し
*/
Mesh::~Mesh() {
	Finalize();
}

HRESULT Mesh::Initialize()
{
	m_IndexCount = 0;

	//if (FAILED(CreateInputLayout(this))) {
	//	ErrorLog("inputlayout");
	//	exit(0);
	//}

	//	外部ファイルの読み取り
	auto rd = Helper::MeshReadHelper::Read("test.yfm");

	//	頂点バッファ作成
	if (FAILED(CreateVertexBuffer(this,rd.vertices))) {
		ErrorLog("vertexbuffer");
		exit(0);
	}

	//	インデックスバッファ作成
	if (FAILED(CreateIndexBuffer(this,rd.indices))) {
		ErrorLog("indexbuffer");
		exit(0);
	}
	//	インデックスの設定
	SetupIndexBuffer(this);


	//if (FAILED(CreateVertexShader())) {
	//	ErrorLog("vshader");
	//	exit(0);
	//}

	//if (FAILED(CreatePixelShader())) {
	//	ErrorLog("pshader");
	//	exit(0);
	//}

	//if (FAILED(CreateConstantBuffer(this))) {
	//	ErrorLog("constant buffer");
	//	exit(0);
	//}

	//===================================
	//	デバイス
#pragma region 直

	//auto& dev = D3D11::Direct3D11::GetInstance();

	////	頂点レイアウト
	//{
	//	D3D11_INPUT_ELEMENT_DESC vd[]
	//	{
	//		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
	//		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	//	};

	//	HRESULT hr = D3D11::Direct3D11::GetInstance().Device->CreateInputLayout(
	//		vd,
	//		GetArraySize(vd),
	//		g_vs_main,
	//		sizeof(g_vs_main),
	//		m_pInputLayout.GetAddressOf()
	//	);
	//	if (FAILED(
	//		hr
	//	)) {
	//		ErrorLog("layout 作成失敗");
	//	}
	//}

	////	頂点シェーダー
	//{
	//	if (FAILED(
	//		D3D11::Direct3D11::GetInstance().Device->CreateVertexShader(
	//			&g_vs_main,
	//			sizeof(g_vs_main),
	//			NULL,
	//			m_pVertexShader.GetAddressOf()
	//		)
	//	)) {
	//		ErrorLog("vs作成失敗");
	//	}
	//}

	////	ピクセルシェーダー
	//{
	//	if (FAILED(
	//		D3D11::Direct3D11::GetInstance().Device->CreatePixelShader(
	//			&g_ps_main,
	//			sizeof(g_ps_main),
	//			NULL,
	//			m_pPixelShader.GetAddressOf()
	//		)
	//	)) {
	//		ErrorLog("ps作成失敗");
	//	}
	//}

	//auto rd = Helper::MeshReadHelper::Read("test.yfm");

	////	コンスタントバッファ
	//{
	//	HRESULT hr;
	//	D3D11_BUFFER_DESC cb;
	//	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	cb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	//	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	cb.MiscFlags = 0;
	//	cb.StructureByteStride = 0;
	//	cb.Usage = D3D11_USAGE_DYNAMIC;
	//	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
	//		&cb,
	//		0,
	//		m_pConstantBuffer.GetAddressOf()
	//	);
	//	if (FAILED(hr)) {
	//		ErrorLog("CBUFFER");
	//		return E_FAIL;
	//	}
	//}

	//auto&dev = Direct3D11::GetInstance();
	//vector<MeshVertex>vertex;
	////	頂点バッファ
	//{
	//	vertex.clear();
	//	for (auto it : rd.vertices) {
	//		vertex.push_back({ it.position });
	//	}

	//	D3D11_BUFFER_DESC bd;
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(MeshVertex) * vertex.size();// * 要素数
	//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	bd.CPUAccessFlags = 0;
	//	bd.MiscFlags = 0;

	//	D3D11_SUBRESOURCE_DATA sd;
	//	sd.pSysMem = vertex.data();

	//	if (FAILED(dev.Device->CreateBuffer(&bd, &sd, m_pVertexBuffer.GetAddressOf()))) {
	//		ErrorLog("vバッファ作成失敗");
	//	}
	//}

	//std::vector<uint32_t>index;
	////	インデックスバッファ
	//{
	//	index = rd.indices;

	//	D3D11_BUFFER_DESC bd;
	//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(uint32_t)*index.size();
	//	bd.CPUAccessFlags = 0;
	//	bd.MiscFlags = 0;

	//	D3D11_SUBRESOURCE_DATA sd;
	//	SecureZeroMemory(&sd, sizeof(sd));
	//	sd.pSysMem = index.data();

	//	if (FAILED(dev.Device->CreateBuffer(
	//		&bd, &sd, m_pIndexBuffer.GetAddressOf()
	//	))) {
	//		ErrorLog("iバッファ作成失敗");
	//	}
	//	dev.ImmediateContext->IASetIndexBuffer(
	//		m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
	//	);
	//}
	//m_IndexCount = index.size();

	//	SRV
	{
		//	テクスチャ読み込み
		std::wstring path;
		path = L"hoge.png";

		Microsoft::WRL::ComPtr<ID3D11Resource> pResource = nullptr;
		HRESULT hr;
		hr = CreateWICTextureFromFile(
			Direct3D11::GetInstance().GetDevice(),
			path.c_str(),
			pResource.GetAddressOf(),
			m_pSRV.GetAddressOf()
		);

		// ローカル変数のメモリ開放
		if (pResource.Get() != nullptr) {
			pResource.Reset();
		}
		if (FAILED(hr)) {
			ErrorLog("srv");
		};
	}

	//	サンプラー
	{
		D3D11_SAMPLER_DESC sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		HRESULT hr;
		hr = Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
			&sd,
			m_pSamplerState.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("sampler");
		}
	}

#pragma endregion


	return S_OK;
}

/*!
	@fn		Finalize
	@brief	破棄処理
	@detail	メンバの明示的な開放
*/
void API::Mesh::Finalize()
{
	//	シェーダーの弱参照開放
	m_pShader.reset();

	//	インデックスバッファ開放
	m_pIndexBuffer.Reset();

	//	頂点数初期化
	m_IndexCount = 0;

	//	頂点バッファ開放
	m_pVertexBuffer.Reset();

	//	サンプラーステート開放
	m_pSamplerState.Reset();

	//	SRV開放
	m_pSRV.Reset();
}

/*!
	@fn		Render
	@brief	描画処理
	@detail	純粋仮想関数をオーバーライド
*/
void Mesh::Render()
{
	HRESULT hr;
	XMMATRIX w, v, p;
	//	world
	{
		//w = XMMatrixTranslation(0, 0, 0);
		w = transform.GetMatrix();
		w = XMMatrixTranspose(w);
	}

	//	view
	{
		XMVECTOR eye, look, upvec;
		//eye = { gEyePt.x,gEyePt.y,gEyePt.z };
		eye = { 0,0,-10 };
		look = { 0,0,0 };
		//look = { gLookPt.x,gLookPt.y,gLookPt.z };
		upvec = { 0,1,0 };
		//v = XMMatrixLookAtLH(eye, look, upvec);
		v = Camera::GetInstance().GetViewMatrix();
		v = XMMatrixTranspose(v);
	}

	//	proj
	{
		float pi = 3.14159265358979323846f;
		p = XMMatrixPerspectiveFovLH(pi / 4, 1920 / 1080, 0.1f, 100.0f);
		p = Camera::GetInstance().GetProjMatrix();
		p = XMMatrixTranspose(p);
	}

	//	シェーダーセット
	{
		//Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(m_pVertexShader.Get(), 0, 0);
		//Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(m_pPixelShader.Get(), 0, 0);
		auto shader = *m_pShader.lock();
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(*shader->GetVertexShader(), 0, 0);
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(*shader->GetPixelShader(), 0, 0);
	}

	//	コンスタントバッファ送信
	{
		auto shader = *m_pShader.lock();
		D3D11_MAPPED_SUBRESOURCE pData;
		MeshConstantBuffer cb;
		hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
			*shader->GetConstantBuffer(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0, 
			&pData
		);
		if (FAILED(hr)) {
			ErrorLog("map");
			exit(0);
		}
		//pcb.m_WorldMatrix = w;
		//pcb.m_ViewMatrix = v;
		//pcb.m_ProjectionMatrix = p;
		cb.m.world = w;
		cb.m.view = v;
		cb.m.proj = p;
		//pcb.m.world = w;
		//pcb.m.view = v;
		//pcb.m.proj = p;
		cb.color = { 1,1,1,1 };

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(*shader->GetConstantBuffer(), 0);
	}

	//	CBufferを使うシェーダーのバインド
	{
		auto shader = *m_pShader.lock();
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
	}

	//	頂点バッファセット
	{
		UINT stride = sizeof(MeshVertex);
		UINT offset = 0;
		Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
			0,
			1,
			m_pVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);
	}

	//	頂点レイアウトセット
	{
		//Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(m_pInputLayout.Get());
		auto shader = *m_pShader.lock();
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(*shader->GetInputLayout());
	}

	//	トポロジー
	{
		//	頂点壊れる？(ポリゴン変)
		//Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//	ソリッド(見かけは大丈夫っぽい)
		Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
		//	綺麗にポリゴン表示出来ているっぽい
		//Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		m_pSamplerState.GetAddressOf()
	);

	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		m_pSRV.GetAddressOf()
	);

	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);

}


HRESULT API::Mesh::CreateIndexBuffer(Mesh * mesh, std::vector<uint32_t> indices)
{
	//	頂点数保持
	mesh->m_IndexCount = indices.size();

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
		mesh->m_pIndexBuffer.GetAddressOf()
	);
}

void API::Mesh::SetupIndexBuffer(Mesh * mesh)
{
	static const uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		mesh->m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);

}

template <class T>
HRESULT API::Mesh::CreateVertexBuffer(Mesh*mesh, std::vector<T>verttices)
{
	//	バッファの仕様
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(T) * verttices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = NULL;

	//	サブリソースの仕様
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = verttices.data();

	//	頂点バッファ作成
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		mesh->m_pVertexBuffer.GetAddressOf()
	);

}