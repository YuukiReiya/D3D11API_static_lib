#include "stdafx.h"
#include "Mesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	コンストラクタ
*/
Mesh::Mesh() 
{
	transform = std::make_shared<Transform>();
}

/*!
	@brief	デストラクタ
*/
Mesh::~Mesh()
{
}

/*!
	@fn			Initialize
	@brief		初期化
	@param[in]	読み込むファイルのパス
*/
HRESULT Mesh::Initialize(std::string path)
{
	auto data = Helper::MeshReadHelper::Read(path);

	//	頂点生成
	if (FAILED(CreateVertexBuffer(this, data.vertices))) {
		return E_FAIL;
	}
	//	頂点バッファ設定
	SetupVertexBuffer();

	//	インデックスバッファ作成
	if (FAILED(CreateIndexBuffer(this, data.indices))) {
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn		Render
	@brief	描画
*/
void Mesh::Render()
{
	//	トポロジーのセット
	SetupTopology();

	//	シェーダーのセット
	SetupBindShader();

	//	コンスタントバッファのセット
	SetupConstantBuffer();

	SetupIndexBuffer();

	//	描画
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(
		m_VertexIndex.size(),
		0,
		0
	);
	//Direct3D11::GetInstance().GetImmediateContext()->DrawIndexedInstanced(
	//	m_VertexIndex.size(),
	//	1,
	//	0, 0, 0
	//);
}

/*!
	@fn			SetupShader
	@brief		シェーダーの設定
	@detail		弱参照でバインドする
	@param[in]	登録するシェーダーのポインタ
*/
void API::Mesh::SetupShader(D3D11::Graphic::AbstractShader * shader)
{
	m_pShader = shader->GetSharedPtr();

}

/*!
	@fn		SetupTopology
	@brief	トポロジーの設定
*/
void API::Mesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

/*!
	@fn		SetupConstantBuffer
	@brief	コンスタントバッファの設定
*/
void API::Mesh::SetupConstantBuffer()
{
	auto shader = *m_pShader.lock();
	auto& device = Direct3D11::GetInstance();

	//	ワールド行列
	DirectX::XMMATRIX world = transform->GetMatrix();

	//	ビュー行列
	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();

	//	プロジェクション行列
	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();

	//	頂点シェーダー用のCバッファ登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBuffer()
	);

	//	ピクセルシェーダー用のCバッファ登録
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBuffer()
	);

	//	マッピング用変数の宣言
	D3D11_MAPPED_SUBRESOURCE pMapData;

	//	バッファへのアクセス(書き換え)許可
	HRESULT hr;
	hr = device.GetImmediateContext()->Map(
		*shader->GetConstantBuffer(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pMapData
	);
	if (FAILED(hr)) {
		std::string error = "Texture mapping is failed!";
		ErrorLog(error);
		//	アクセス権を閉じて抜ける
		device.GetImmediateContext()->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	MeshShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	バッファに代入
	cb.m_WorldMatrix = world;
	cb.m_ViewMatrix = view;
	cb.m_ProjectionMatrix = proj;
	cb.m_Color = color.GetRGBA();

	//	メモリコピー
	memcpy_s(pMapData.pData, pMapData.RowPitch, (void*)(&cb), sizeof(cb));

	//	アクセス許可終了
	device.GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);
}

/*!
	@fn		SetupBindShader
	@brief	シェーダーの設定
*/
void API::Mesh::SetupBindShader()
{
	auto shader = *m_pShader.lock();

	//	頂点シェーダー
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*shader->GetVertexShader(),
		NULL,
		NULL
	);

	//	ピクセルシェーダー
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*shader->GetPixelShader(),
		NULL,
		NULL
	);

}

/*!
	@fn			CreateVertexBuffer
	@brief		メッシュの頂点を生成
	@detail		静的関数
	@param[in]	構成する頂点情報
	@param[in]	構成する頂点インデックス
	@param[in]	メッシュのポインタ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Mesh::CreateVertexBuffer(Mesh * mesh, std::vector<MeshVertex> vertex)
{
	//	頂点定義
	std::vector<MeshVertex>v;
	for (auto it : vertex)
	{
		MeshVertex tmp;
		tmp.m_Pos.x = it.m_Pos.x;
		tmp.m_Pos.y = it.m_Pos.y;
		tmp.m_Pos.z = it.m_Pos.z;
		v.push_back(tmp);
	}

	//	バッファ定義
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// GPUから読み込みと書き込みを許可
	bd.ByteWidth = sizeof(MeshVertex)*vertex.size();			// バッファのサイズ
	//bd.ByteWidth = sizeof(v);									// バッファのサイズ
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド

	// サブリソースのデータを定義
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = v.data();					// 初期化データへのポインタ

	// 頂点バッファの開放
	mesh->m_pVertexBuffer.Reset();

	HRESULT hr;
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		mesh->m_pVertexBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "Mesh vertex buffer is not Create!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn		SetupVertexBuffer
	@brief	頂点バッファ設定
*/
void API::Mesh::SetupVertexBuffer()
{
	uint32_t stride = sizeof(MeshVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

}

/*!
	@fn			CreateIndexBuffer
	@brief		インデックスバッファ作成
	@detail		静的関数
	@param[in]	設定するメッシュのポインタ
	@param[in]	頂点のインデックス
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Mesh::CreateIndexBuffer(Mesh * mesh, std::vector<uint32_t> index)
{
	mesh->m_pIndexBuffer.Reset();
	mesh->m_VertexIndex = index;

	//	バッファ定義
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t)*index.size();
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	
	// サブリソースのデータを定義
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = mesh->m_VertexIndex.data();		// 初期化データへのポインタ

	HRESULT hr;
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		mesh->m_pIndexBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "Mesh index buffer is not Create!";
		ErrorLog(error);
		return E_FAIL;
	}


	return S_OK;
}

/*!
	@fn		SetupVertexBuffer
	@brief	インデックスバッファ設定
*/
void API::Mesh::SetupIndexBuffer()
{
	const uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		offset
	);

}