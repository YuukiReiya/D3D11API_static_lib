/*
	@file	Mesh.h
	@date	2019/05/17
	@author	番場 宥輝
	@brief	メッシュ
*/
#include "stdafx.h"
#include "Mesh.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "MemoryLeaks.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include "WICTextureLoader.h"
#include "MeshConstantBuffer.h"
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
Mesh::Mesh() :m_IndexCount(0) {
	transform = std::make_shared<Transform>();
}

/*!
	@brief	ムーブコンストラクタ
*/
API::Mesh::Mesh(const Mesh & inst)
{
	*this = inst;
}

/*!
	@brief	デストラクタ
	@detail	開放処理の呼び出し
*/
Mesh::~Mesh() {
	Finalize();
}

/*!
	@fn			Initialize
	@brief		初期化
	@detail		モデルのみの読み込みで別途テクスチャを作成する必要がある
	@param[in]	外部ファイルのパス
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT Mesh::Initialize(std::string path)
{
	try
	{
		vector<uint32_t>indices;
		vector<MeshVertex>vertices;
		//	外部ファイルの読み取り
		if (!Load(path, indices, vertices)) { throw runtime_error("load to file"); }

		//	頂点バッファ作成
		if (FAILED(CreateVertexBuffer(this, vertices))) { throw runtime_error("create vertex buffer"); }

		//	インデックスバッファ作成
		if (FAILED(CreateIndexBuffer(this,indices))) { throw runtime_error("create index buffer"); }

		//	インデックス数保持
		m_IndexCount = indices.size();
	}
	catch (const std::exception&e)
	{
		string error = "\"" + path + "\" " + "Failed to " + e.what() + ".";
		ErrorLog(error);
		return E_FAIL;
	}
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
	//m_pSamplerState.Reset();

	//	SRV開放
	//m_pSRV.Reset();
	m_pMaterial.reset();
}

/*!
	@fn		Render
	@brief	描画処理
	@detail	純粋仮想関数をオーバーライド
*/
void Mesh::Render()
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

/*!
	@fn		SetupTopology
	@brief	トポロジーのセットアップ
*/
void API::Mesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*!
	@fn		SetupInputLayout
	@brief	頂点レイアウトのセットアップ
*/
void API::Mesh::SetupInputLayout()
{
	try
	{
		//	shader
		auto shader = *m_pShader.lock();
		if (!shader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*shader->GetInputLayout()) {
			throw "this shader is inputlayout has an invalid value";
		}
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
			*shader->GetInputLayout()
		);
	}
	catch (const string error)
	{
		ErrorLog(error);
		return;
	}
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

/*!
	@fn			SetupIndexBuffer
	@brief		生成したインデックスバッファのセット
*/
void API::Mesh::SetupIndexBuffer()
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
	@brief	頂点バッファのセットアップ
*/
void API::Mesh::SetupVertexBuffer()
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
	@brief	コンスタントバッファのセットアップ
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT API::Mesh::SetupConstantBuffer()
{
	try
	{
		//	shader
		auto shader = *m_pShader.lock();
		if (!shader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*shader->GetConstantBuffer()) {
			throw "this shader is constant buffer has an invalid value";
		}

		HRESULT hr;
		XMMATRIX w, v, p;

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
			*shader->GetConstantBuffer(),
			NULL,
			D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
			NULL,
			&mapped
		);
		if (FAILED(hr))
		{
			Direct3D11::GetInstance().GetImmediateContext()->Unmap(
				*shader->GetConstantBuffer(),
				NULL
			);
			throw "Failed to map constant buffer for this shader";
		}

		//	データの格納
		value.m.world = w;
		value.m.view = v;
		value.m.proj = p;
		value.color = color.GetRGBA();

		//	メモリコピー
		memcpy_s(mapped.pData, mapped.RowPitch, (void*)(&value), sizeof(value));

		//	アクセス許可終了
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(
			*shader->GetConstantBuffer(),
			NULL
		);

		//	コンスタントバッファをシェーダーに登録

		//	頂点シェーダー
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
			0,
			1,
			shader->GetConstantBuffer()
		);

		//	ピクセルシェーダー
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
			0,
			1,
			shader->GetConstantBuffer()
		);
			
	}
	catch (const string error)
	{
		ErrorLog(error);
		return E_FAIL;
	}
	

	return S_OK;
}

/*!
	@fn		SetupBindShader
	@brief	ImmediateContextにシェーダーをバインド
	@detail	セットするシェーダーは頂点シェーダーとピクセルシェーダー
*/
void API::Mesh::SetupBindShader()
{
	try
	{
		auto shader = *m_pShader.lock();
		if (!shader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*shader->GetVertexShader()) {
			throw "this shader is vertex shader has an invalid value";
		}
		if (!*shader->GetPixelShader()) {
			throw "this shader is pixel shader has an invalid value";
		}

		//	頂点シェーダー
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
			*shader->GetVertexShader(),
			NULL,
			0
		);

		//	ピクセルシェーダー
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
			*shader->GetPixelShader(),
			NULL,
			0
		);
	}
	catch (const string error)
	{
		ErrorLog(error);
		return;
	}
}

/*!
	@fn		SetupTexture
	@brief	テクスチャのセットアップ
	@detail	SRVとサンプラーのセットアップを行う
	@note	テクスチャの無いモデルも想定し、Errorによるメッセージボックスは表示しない
*/
void API::Mesh::SetupTexture()
{
	auto material = m_pMaterial.lock();
	if (!material) {
		std::string error = "failed weak reference";
		ErrorLog(error);
		return;
	}

	//	サンプラー
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		(*material)->GetSamplerState()
	);

	//	SRV
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		(*material)->GetShaderResourceView()
	);
}

bool API::Mesh::Load(std::string filePath, std::vector<uint32_t>& indices, std::vector<D3D11::Graphic::MeshVertex>& vertices)
{
	ifstream ifs(filePath);
	if (ifs.fail()) {
		string error = "Failed to read \"" + filePath + "\" file.";
		ErrorLog(error);
		return false;
	}
	string buf;

	//	定数宣言
	static constexpr string_view c_Comma = ",";
	static constexpr string_view c_Space = " ";

	//	頂点インデックス
	getline(ifs, buf);
	while (true)
	{
		auto commaOffset = buf.find(c_Comma);
		if (commaOffset == string::npos) { break; }
		uint32_t index = stoi(buf.substr(0, commaOffset));
		indices.push_back(index);
		buf = buf.substr(commaOffset + 1);
	}

	//	UV
	getline(ifs, buf);
	while (true)
	{
		auto commaOffset = buf.find(c_Comma);
		if (commaOffset == string::npos) { break; }
		float u = stof(buf.substr(0, commaOffset));
		buf = buf.substr(commaOffset + 1);
		auto spaceOffset = buf.find(c_Space);
		if (spaceOffset == string::npos) { break; }
		float v = stof(buf.substr(0, spaceOffset));
		buf = buf.substr(spaceOffset + 1);

		MeshVertex vertex;
		vertex.uv = { u,v };
		vertices.push_back(vertex);
	}

	const bool uvEmpty = vertices.empty();
	int index = -1;

	//	頂点
	getline(ifs, buf);
	while (true)
	{
		auto xOffset = buf.find(c_Comma);
		if (xOffset == string::npos) { break; }
		float x = stof(buf.substr(0, xOffset));
		buf = buf.substr(xOffset + 1);

		auto yOffset = buf.find(c_Comma);
		if (yOffset == string::npos) { break; }
		float y = stof(buf.substr(0, yOffset));
		buf = buf.substr(yOffset + 1);

		auto zOffset = buf.find(c_Space);
		if (zOffset == string::npos) { break; }
		float z = stof(buf.substr(0, zOffset));
		buf = buf.substr(zOffset + 1);

		//	UV無し
		if (uvEmpty) {
			vertices.push_back({ {x,y,z} ,{ -1,-1 } });
			continue;
		}

		//	UV有り
		++index;
		vertices[index].position = { x,y,z };
	}
	return true;
}

/*!
	@template	Vertex
	@brief		CreateVertexBuffer関数で使う頂点構造体のためのテンプレート
*/
template <class Vertex>

/*!
	@fn			CreateVertexBuffer
	@brief		頂点バッファ生成
	@param[in]	生成先のメッシュ
	@param[in]	頂点バッファを構成する頂点情報
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT API::Mesh::CreateVertexBuffer(Mesh*mesh, std::vector<Vertex>verttices)
{
	//	バッファの仕様
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * verttices.size();
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