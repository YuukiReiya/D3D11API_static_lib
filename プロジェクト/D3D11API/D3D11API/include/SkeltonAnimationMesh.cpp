#include "stdafx.h"
#include "SkeltonAnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MatrixConstantBuffer.h"
#include "MyGame.h"
//#include "../../cso/ps_AnimMesh60.h"
//#include "../../cso/vs_AnimMesh60.h"
#include "../../cso/vs_SkinAnim.h"
#include "../../cso/ps_SkinAnim.h"
#include <DirectXMath.h>
#include "MeshReadHelper.h"

using namespace D3D11;
using namespace D3D11::Graphic;
using namespace std;
using namespace DirectX;

#pragma region 定数バッファ
struct alignas(16) AnimConstantBuffer
{
	MatrixConstantBuffer m;
	//DirectX::XMMATRIX bornMat[12];//packoffsetか最後に持ってくるの安定か…


	//	18 = 1頂点に"影響を与えるボーン数" * ポリゴンを構成する頂点数(三角ポリゴンなので = 3)
	//∴ 6(7) * 3 = 18(21)
	//DirectX::XMMATRIX bornMat[18];//packoffsetか最後に持ってくるの安定か…
	DirectX::XMMATRIX bornMat[60];//packoffsetか最後に持ってくるの安定か…
};
#pragma endregion

#pragma region ボーン
struct Bone
{
#if 0
	//	API
	unsigned int id;		//	ボーンの番号(インデックスとして機能)
	XMFLOAT4X4 offsetMat;	//	オフセット行列
	XMMATRIX* combMatPtr;	//	影響度を直接管理する配列のポインタ(XMMATRIX*[]型)

	//	Converter
	Bone* child;			//	自身の子ボーン
	Bone* sibling;			//	自身と同じ親を持つ兄弟ボーン
	XMFLOAT4X4 initMat;		//	初期姿勢
	std::vector<XMFLOAT4X4> frameMat;	//	フレーム行列


	Bone() :id(), child(), sibling(), combMatPtr() {
		XMStoreFloat4x4(&initMat, XMMatrixIdentity());
		XMStoreFloat4x4(&offsetMat, XMMatrixIdentity());
	}
#else
	//	オフセット行列
	XMMATRIX offsetMat;

	//	初期姿勢
	XMMATRIX initMat;

	//	フレーム時姿勢(0フレーム)
	XMMATRIX frameMat;
#endif

};
#pragma endregion


#pragma region シェーダーオブジェクト
class AnimShader
	: public AbstractShader
{
public:
	AnimShader():AbstractShader() {}
	~AnimShader() {}


	HRESULT Setup()override final;
	HRESULT DynamicSetup()override final { return E_FAIL; }

private:

};
HRESULT AnimShader::Setup()
{
	HRESULT hr;
	auto device = D3D11::Direct3D11::GetInstance().GetDevice();
#pragma region 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,	0,	0							,D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "BLENDWEIGHT",	0,DXGI_FORMAT_R32G32B32_FLOAT,  0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		//	{ "BLENDINDICES",	0,DXGI_FORMAT_R8_UINT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	
	hr = device->CreateInputLayout(
		desc,
		GetArraySize(desc),
		g_vs_main,
		sizeof(g_vs_main),
		m_pVertexLayout.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("create inputlayout");
		return E_FAIL; 
	}
#pragma endregion

#pragma region 頂点シェーダー
	hr = device->CreateVertexShader(
		&g_vs_main,
		sizeof(g_vs_main),
		NULL,
		m_pVertexShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("create vs");
		return E_FAIL; 
	}
#pragma endregion

#pragma region ピクセルシェーダー
	hr = device->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("create ps");
		return E_FAIL;
	}
#pragma endregion

#pragma region 定数バッファ
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(AnimConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(
		&cb,
		NULL,
		m_pConstantBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		ErrorLog("create cb");
		return E_FAIL;
	}

#pragma endregion


	return S_OK;
}


#pragma endregion

#define BONE
#ifdef BONE
//Bone*g_Bones = new Bone[7];
std::vector<Bone>g_Bones;

#endif // BONE

HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	//auto data = Helper::MeshReadHelper::Read("twin.yfm");
	auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");


	//	頂点
	vector<AnimVertex>vv;
#pragma region 頂点
#ifdef BONE
#if 0


	AnimVertex vtx[] = {
		{ DirectX::XMFLOAT3(-0.5000f, -2.2887f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{2, 0, 0, 0} },
		{ DirectX::XMFLOAT3(-0.5000f, -1.2887f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{1, 2, 0, 0} },
		{ DirectX::XMFLOAT3(-0.5000f, -0.2887f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{0, 0, 0, 0} },
		{ DirectX::XMFLOAT3(-1.3660f,  0.2113f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{3, 4, 0, 0} },
		{ DirectX::XMFLOAT3(-2.2321f,  0.7113f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{4, 0, 0, 0} },
		{ DirectX::XMFLOAT3(-1.7321f,  1.5774f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{4, 0, 0, 0} },
		{ DirectX::XMFLOAT3(-0.8660f,  1.0774f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{3, 4, 0, 0} },
		{ DirectX::XMFLOAT3(0.0000f,  0.5774f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{0, 0, 0, 0} },
		{ DirectX::XMFLOAT3(0.8660f,  1.0774f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{5, 6, 0, 0} },
		{ DirectX::XMFLOAT3(1.7321f,  1.5774f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{6, 0, 0, 0} },
		{ DirectX::XMFLOAT3(2.2321f,  0.7113f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{6, 0, 0, 0} },
		{ DirectX::XMFLOAT3(1.3660f,  0.2113f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{5, 6, 0, 0} },
		{ DirectX::XMFLOAT3(0.5000f, -0.2887f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{0, 0, 0, 0} },
		{ DirectX::XMFLOAT3(0.5000f, -1.2887f, 0.0f), DirectX::XMFLOAT3(0.50f, 0.50f, 0.00f), new int[4]{1, 2, 0, 0} },
		{ DirectX::XMFLOAT3(0.5000f, -2.2887f, 0.0f), DirectX::XMFLOAT3(1.00f, 0.00f, 0.00f), new int[4]{2, 0, 0, 0} },
	};

	data.vertices.clear();
	for (auto it:vtx)
	{
		data.vertices.push_back(it.position);
	}
#endif // 0
#endif // BONE
#pragma endregion
	for (auto it : data.vertices) {

		AnimVertex av;

		//	座標
		av.position = it.position;

		//	重み
		for (size_t i = 0; i < c_AffectedBoneCount; i++)
		{
			//	読み込みデータが存在しない
			if (it.weight.size() <= i) {
				av.weights[i] = 0.0f;
				av.bornIndex[i] = 0;
				continue;
			}
			auto index = it.indexOfBonesAffested[i];
			av.bornIndex[i] = index;
			av.weights[i] = it.weight[index];
		}
		
		vv.push_back(av);
	}

	//	頂点インデックス
	vector<uint32_t>vi;
#pragma region 頂点インデックス
#ifdef BONE
#if 0
	uint32_t idx[39] = {
		0, 1, 14,
		1, 13, 14,
		1, 2, 13,
		2, 12, 13,
		2, 7, 12,
		2, 6, 7,
		2, 3, 6,
		3, 5, 6,
		3, 4, 5,
		7, 8, 12,
		8, 11, 12,
		8, 9, 11,
		9, 10, 11,
	};
	data.indices.clear();
	for (auto it : idx) {
		data.indices.push_back(it);
	}
#endif // 0
#endif // BONE
#pragma endregion
	vi = data.indices;

#pragma region ボーン情報初期化
#ifdef BONE
	const unsigned int c_BoneCount = data.initialMatrix.size();
	const unsigned int c_AnimFrame = data.frame;
	for (size_t i = 0; i < c_BoneCount; i++)
	{
		Bone bone;


		//	初期姿勢
		//bone.initMat = data.initialMatrix[i];
		bone.initMat = XMLoadFloat4x4(&data.initialMatrix[i]);
		//	フレーム時姿勢
		/*for (size_t f = 0; f < c_AnimFrame; f++)
		{
			bone.frameMat.push_back(data.frameMatrix[f][i]);

		}*/
		//bone.frameMat = data.frameMatrix[0][i];
		bone.frameMat = XMLoadFloat4x4(&data.frameMatrix[0][i]);

		//	オフセット行列 = 初期姿勢の逆行列
		//XMMATRIX m = DirectX::XMLoadFloat4x4(&bone.initMat);
		//m = XMMatrixInverse(NULL, m);
		//XMStoreFloat4x4(&bone.offsetMat, m);
		bone.offsetMat = XMMatrixInverse(NULL, bone.initMat);

		//	
		g_Bones.push_back(bone);
	}
#endif
#pragma endregion



	if (FAILED(CreateVertexBuffer(vv))) {
		ErrorLog("falied to create vertex");
	}

	if (FAILED(CreateIndexBuffer(vi))) {
		ErrorLog("falied to create index");
	}

	//	シェーダー
	m_pShader = make_shared<AnimShader>();
	if (FAILED(m_pShader->Setup())) {
		ErrorLog("falied to setup shader");
	}

	//	トランスフォーム
	transform = make_shared<Transform>();

	return E_NOTIMPL;
}

void API::Anim::SkeltonAnimationMesh::Destroy()
{
#ifdef BONE

#endif // BONE

}

void API::Anim::SkeltonAnimationMesh::Render()
{
#pragma region トポロジー
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region ボーン
#ifdef BONE


#endif // BONE



#pragma endregion

#pragma region 定数バッファ

	DirectX::XMMATRIX w, v, p;
#pragma region WVP
	//	world
	w = XMMatrixTranspose(transform->GetWorldMatrix());

	//	view
	v = DirectX::XMMatrixTranspose(Camera::GetInstance().GetViewMatrix());

	//	proj
	p = DirectX::XMMatrixTranspose(Camera::GetInstance().GetProjMatrix());
#pragma endregion

#pragma region CBuffer
	D3D11_MAPPED_SUBRESOURCE mp;
	AnimConstantBuffer cb;
	HRESULT hr;

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
	//cb.frameMat

#pragma region ボーンの計算？

	const unsigned int c_BoneCount = g_Bones.size();
	//for (size_t i = 0; i < c_BoneCount; i++)
	//{
	//	DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4(&g_Bones[i].offsetMat) * DirectX::XMLoadFloat4x4(&g_Bones[i].frameMat[animIndex]);
	//	cb.bornMat[i] = m;
	//	
	//	cb.bornMat[i] = DirectX::XMMatrixIdentity();
	//}

	for (size_t i = 1; i < c_BoneCount; i++)
	{
		cb.bornMat[i] = g_Bones[i].offsetMat*g_Bones[i].frameMat;

		cb.bornMat[i] = XMMatrixTranspose(cb.bornMat[i]);

		cb.bornMat[i] = XMMatrixIdentity();
	}

#pragma endregion

	//	メモリコピー
	memcpy_s(mp.pData, mp.RowPitch, (void*)(&cb), sizeof(cb));
	//	アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*m_pShader->GetConstantBuffer(),
		NULL
	);

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
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AnimVertex) * verttices.size();
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

