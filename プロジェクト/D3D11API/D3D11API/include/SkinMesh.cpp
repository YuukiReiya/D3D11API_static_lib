#include "stdafx.h"
#include "SkinMesh.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "SkinnedVertex.h"
#include "MeshShader.h"
#include "MeshConstantBuffer.h"
#include "Camera.h"
#include "SkinnedVertex.h"

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

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
	@brief	usingディレクティブ
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	コンストラクタ
*/
API::SkinMesh::SkinMesh()
	: m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
	, frameIndex(0)
{
	transform = make_shared<Transform>();
}

/*!
	@brief	デストラクタ
*/
API::SkinMesh::~SkinMesh()
{
}

/*!
	@fn			Initialize
	@brief		初期化
	@param[in]	読み込み用のファイルパス
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT API::SkinMesh::Initialize(std::string path)
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr = E_FAIL;

	try
	{
		m_pShader = make_shared<MeshShader>();
		if (FAILED(m_pShader->Setup())) { throw runtime_error("shader setup"); }

		//	ファイル読み込み
		if (!Load(this, path)) { throw runtime_error("load to file"); }

		//	スキニングの指定(ソフトウェアスキニング or シェーダー)
		static constexpr uint32_t c_ShaderJointCount = 4;
		m_eSkinningMode = m_Vertices.end() != std::find_if(
			m_Vertices.begin(), m_Vertices.end(),
			[](SkinnedVertex v)-> bool { return v.joints.size() > c_ShaderJointCount; }
		) ? SkinningMode::SOFTWARE : SkinningMode::SHADER;

		//	インデックスバッファ作成
		if (FAILED(CreateIndexBuffer())) { throw runtime_error("create index buffer"); }

		//	頂点バッファ作成
		if (FAILED(CreateVertexBuffer())) { throw runtime_error("create vertex buffer"); }
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
	@fn		Render
	@brief	描画
*/
void API::SkinMesh::Render()
{
	//	トポロジー
	SetupTopology();

	//	ソフトウェアスキニング
	SoftwareSkinning();

	//	頂点レイアウト
	SetupInputLayout();

	//	頂点バッファ
	SetupVertexBuffer();

	//	インデックスバッファ
	SetupIndexBuffer();

	//	テクスチャ
	SetupTexture();

	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_Indices.size(), 0, 0);
}

/*!
	@fn		SetupTopology
	@brief	トポロジーのセットアップ
*/
void API::SkinMesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

/*!
	@fn		SetupInputLayout
	@brief	頂点レイアウトのセットアップ
*/
void API::SkinMesh::SetupInputLayout()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(*m_pShader->GetInputLayout());
}

/*!
	@fn		SetupVertexBuffer
	@brief	頂点バッファのセットアップ
*/
void API::SkinMesh::SetupVertexBuffer()
{
	uint32_t stride = sizeof(SkinnedVertex);
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
	@fn		SetupIndexBuffer
	@brief	インデックスバッファのセットアップ
*/
void API::SkinMesh::SetupIndexBuffer()
{
	static constexpr uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);
}

/*!
	@fn			Load
	@brief		ファイルの読み込み
	@param[in]	バインド先メッシュ
	@param[in]	ファイルのパス
	@return		true:成功 false:失敗
*/
bool API::SkinMesh::Load(SkinMesh * mesh, std::string filePath)
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
	static constexpr string_view c_Colon = ":";

	//	頂点インデックス
	getline(ifs, buf);
	while (true)
	{
		auto commaOffset = buf.find(c_Comma);
		if (commaOffset == string::npos) { break; }
		uint32_t index = stoi(buf.substr(0, commaOffset));
		mesh->m_Indices.push_back(index);
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

		SkinnedVertex vertex;
		vertex.uv = { u,v };
		mesh->m_Vertices.push_back(vertex);
	}

	const bool uvEmpty = mesh->m_Vertices.empty();
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
			mesh->m_Vertices.push_back({ {x,y,z} ,{ -1,-1 } });
			continue;
		}

		//	UV有り
		++index;
		mesh->m_Vertices[index].position = { x,y,z };
	}

	//	重み
	for (size_t i = 0; i < mesh->m_Vertices.size(); i++)
	{
		getline(ifs, buf);
		while (true)
		{
			auto colonOffset = buf.find(c_Colon);
			if (colonOffset == string::npos) { break; }
			uint32_t jointIndex = stoi(buf.substr(0, colonOffset));
			buf = buf.substr(colonOffset + 1);
			auto weightOffset = buf.find(c_Comma);
			if (weightOffset == string::npos) { break; }
			float weight = stof(buf.substr(0, weightOffset));
			buf = buf.substr(weightOffset + 1);
			mesh->m_Vertices[i].joints.push_back({ jointIndex,weight });
		}
	}

	return true;
}

/*!
	@fn		SoftwareSkinning
	@brief	ソフトウェアスキニング
*/
void API::SkinMesh::SoftwareSkinning()
{
	auto&dev = Direct3D11::GetInstance();
	auto&camera = Camera::GetInstance();

	HRESULT hr;

#pragma region 定数バッファ
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
	if (FAILED(hr)) { 
		ErrorLog("Map failed"); 
		dev.GetImmediateContext()->Unmap(*m_pShader->GetConstantBuffer(), NULL);
	}
	cb.m.world = w;
	cb.m.view = v;
	cb.m.proj = p;
	cb.color = { 1,1,1,1 };

	memcpy_s(ms.pData, ms.RowPitch, (void*)(&cb), sizeof(cb));
	dev.GetImmediateContext()->Unmap(*m_pShader->GetConstantBuffer(), NULL);
#pragma endregion

#pragma region シェーダーのバインド
	dev.GetImmediateContext()->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->VSSetShader(*m_pShader->GetVertexShader(), 0, 0);
	dev.GetImmediateContext()->PSSetShader(*m_pShader->GetPixelShader(), 0, 0);
#pragma endregion
	
#pragma region 頂点の書き換え
	if (!m_pAnimClip.expired()) {
		auto animClip = m_pAnimClip.lock();

		hr = dev.GetImmediateContext()->Map(
			m_pVertexBuffer.Get(),
			0,
			D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
			0,
			&ms
		);

		//	合成行列
		vector<XMMATRIX>compMats;
		compMats.resize(m_Vertices.size());
		//	頂点ループ
		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			//	ジョイントループ
			for (size_t j = 0; j < m_Vertices[i].joints.size(); j++)
			{
				uint32_t jointIndex = m_Vertices[i].joints[j].index;
				float weight = m_Vertices[i].joints[j].weight;
				XMMATRIX skinningMat = animClip.get()->matrixPalette[jointIndex][frameIndex];
				compMats[i] += (skinningMat * weight);
			}
		}
	
		//	変換後頂点
		vector<SkinnedVertex>vertices;
		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			XMVECTOR vec =
			{
				m_Vertices[i].position.x,
				m_Vertices[i].position.y,
				m_Vertices[i].position.z,
				1
			};
			vec = XMVector4Transform(vec, compMats[i]);
			SkinnedVertex v=
			{
				{ vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2] },
				{ m_Vertices[i].uv }
			};
			vertices.push_back(v);
		}

		//	メモリコピー
		memcpy_s(ms.pData, ms.RowPitch, (void*)(vertices.data()), sizeof(SkinnedVertex) * m_Vertices.size());
		dev.GetImmediateContext()->Unmap(m_pVertexBuffer.Get(), 0);
	}
#pragma endregion

}

/*!
	@fn		SetupTexture
	@brief	テクスチャ情報のセットアップ
	@detail	SRVとサンプラーをピクセルシェーダーにセットする
*/
void API::SkinMesh::SetupTexture()
{
	auto material = m_pMaterial.lock();
	if (m_pMaterial.expired()) { return; }

	//	サンプラー
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		material->GetSamplerState()
	);

	//	SRV
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		material->GetShaderResourceView()
	);
}

/*!
	@fn		CreateIndexBuffer
	@brief	インデックスバッファの作成
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT API::SkinMesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * m_Indices.size();
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_Indices.data();
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pIndexBuffer.GetAddressOf()
	);
}

/*!
	@fn		CreateVertexBuffer
	@brief	頂点バッファの作成
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT API::SkinMesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SkinnedVertex)*m_Vertices.size();
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = m_Vertices.data();
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pVertexBuffer.GetAddressOf()
	);
}
