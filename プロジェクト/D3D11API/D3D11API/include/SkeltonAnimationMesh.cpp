#include "stdafx.h"
#include "SkeltonAnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MatrixConstantBuffer.h"
#include "MyGame.h"
#include "vs_AnimMesh.h"
#include "ps_AnimMesh.h"
#include <DirectXMath.h>
#include "MeshReadHelper.h"

using namespace D3D11;
using namespace D3D11::Graphic;
using namespace std;
using namespace DirectX;

#pragma region �萔�o�b�t�@
struct alignas(16) AnimConstantBuffer
{
	MatrixConstantBuffer m;
	DirectX::XMMATRIX bornMat[12];
};
#pragma endregion

#pragma region �{�[��
struct Bone
{
	unsigned int id;
	Bone* child;
	Bone* sibling;
	XMFLOAT4X4 offsetMat;
	XMFLOAT4X4 initMat;
	XMFLOAT4X4 bornMat;
	XMMATRIX* combMatPtr;
	Bone() :id(), child(), sibling(), combMatPtr() {
		XMStoreFloat4x4(&initMat, XMMatrixIdentity());
		XMStoreFloat4x4(&offsetMat, XMMatrixIdentity());
		XMStoreFloat4x4(&bornMat, XMMatrixIdentity());
	}
};
#pragma endregion


#pragma region �V�F�[�_�[�I�u�W�F�N�g
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
#pragma region ���_���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,	0,	0							,D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "BLENDWEIGHT",	0,DXGI_FORMAT_R32G32B32_FLOAT,  0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "BLENDINDICES",	0,DXGI_FORMAT_R8_UINT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
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

#pragma region ���_�V�F�[�_�[
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

#pragma region �s�N�Z���V�F�[�_�[
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

#pragma region �萔�o�b�t�@
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
Bone*g_Bones = new Bone[7];

static void CalcRelativeMat(Bone*bone, XMFLOAT4X4*offsetMat) {
	if (bone->child) {
		CalcRelativeMat(bone->child, &bone->offsetMat);
	}
	if (bone->sibling) {
		CalcRelativeMat(bone->sibling, offsetMat);
	}
	if (offsetMat) {
		XMMATRIX m = XMLoadFloat4x4(&bone->initMat);
		m *= XMLoadFloat4x4(offsetMat);
		XMStoreFloat4x4(&bone->initMat, m);
	}
}

static void UpdateBone(Bone*bone, XMFLOAT4X4*worldMat) {
	
	XMMATRIX m = XMLoadFloat4x4(&bone->bornMat)*XMLoadFloat4x4(worldMat);
	XMStoreFloat4x4(&bone->bornMat, m);

	bone->combMatPtr[bone->id] = XMLoadFloat4x4(&bone->offsetMat)*XMLoadFloat4x4(&bone->bornMat);

	if (bone->child) {
		UpdateBone(bone->child, &bone->bornMat);
	}

	if (bone->sibling) {
		UpdateBone(bone->sibling, worldMat);
	}
}
#endif // BONE

HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	auto data = Helper::MeshReadHelper::Read("twin.yfm");

	//	���_
	vector<AnimVertex>vv;
#pragma region ���_
#ifdef BONE
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
#pragma endregion
	for (auto it : data.vertices) {
		vv.push_back({it.position});
	}

	//	���_�C���f�b�N�X
	vector<uint32_t>vi;
#pragma region ���_�C���f�b�N�X
#ifdef BONE
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
#pragma endregion
	vi = data.indices;

#pragma region �{�[�����
#ifdef BONE
	//	�e�q�֌W
	g_Bones[0].child = &g_Bones[1];
	g_Bones[1].child = &g_Bones[2];
	g_Bones[3].child = &g_Bones[4];
	g_Bones[5].child = &g_Bones[6];
	g_Bones[1].sibling = &g_Bones[3];
	g_Bones[3].sibling = &g_Bones[5];

	//	�����p��
	XMStoreFloat4x4(&g_Bones[0].initMat, XMMatrixRotationZ(XMConvertToRadians(-90.0f)));
	XMStoreFloat4x4(&g_Bones[1].initMat, XMMatrixRotationZ(XMConvertToRadians(-90.0f)));
	XMStoreFloat4x4(&g_Bones[2].initMat, XMMatrixRotationZ(XMConvertToRadians(-90.0f)));
	XMStoreFloat4x4(&g_Bones[3].initMat, XMMatrixRotationZ(XMConvertToRadians(150.0f)));
	XMStoreFloat4x4(&g_Bones[4].initMat, XMMatrixRotationZ(XMConvertToRadians(150.0f)));
	XMStoreFloat4x4(&g_Bones[5].initMat, XMMatrixRotationZ(XMConvertToRadians(30.0f)));
	XMStoreFloat4x4(&g_Bones[6].initMat, XMMatrixRotationZ(XMConvertToRadians(30.0f)));
	g_Bones[0].initMat._41 = 0, g_Bones[0].initMat._42 = 0;
	g_Bones[1].initMat._41 = 0, g_Bones[1].initMat._42 = -1;
	g_Bones[2].initMat._41 = 0, g_Bones[2].initMat._42 = -2;
	g_Bones[3].initMat._41 = -0.683f, g_Bones[3].initMat._42 = 0.3943f;
	g_Bones[4].initMat._41 = -1.549f, g_Bones[4].initMat._42 = 0.8943f;
	g_Bones[5].initMat._41 = 0.683f, g_Bones[5].initMat._42 = 0.3943f;
	g_Bones[6].initMat._41 = 1.549f, g_Bones[6].initMat._42 = 0.8943f;

	//	�I�t�Z�b�g�s��s��
	XMMATRIX* combMat = new XMMATRIX[7];
	for (int i = 0; i < 7; ++i) {
		g_Bones[i].id = i;
		g_Bones[i].combMatPtr = combMat;
		XMMATRIX m = DirectX::XMLoadFloat4x4(&g_Bones[i].initMat);
		m = XMMatrixInverse(NULL, m);
		XMStoreFloat4x4(&g_Bones[i].offsetMat, m);
	}

	//	�e���猩�����Ύp���ɕϊ�
	CalcRelativeMat(g_Bones, 0);
#endif
#pragma endregion



	if (FAILED(CreateVertexBuffer(vv))) {
		ErrorLog("falied to create vertex");
	}

	if (FAILED(CreateIndexBuffer(vi))) {
		ErrorLog("falied to create index");
	}

	//	�V�F�[�_�[
	m_pShader = make_shared<AnimShader>();
	if (FAILED(m_pShader->Setup())) {
		ErrorLog("falied to setup shader");
	}

	//	�g�����X�t�H�[��
	transform = make_shared<Transform>();

	return E_NOTIMPL;
}

void API::Anim::SkeltonAnimationMesh::Destroy()
{
#ifdef BONE
	delete[] g_Bones;

#endif // BONE

}

void API::Anim::SkeltonAnimationMesh::Render()
{
#pragma region �g�|���W�[
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �{�[��
#ifdef BONE
	XMMATRIX defBone[7];
	defBone[0] = XMMatrixIdentity();

	static float ang = 0;
	ang += 0.03f;
	//	�K���ɓ�����
	for (int i = 1; i < 7; ++i) {
		defBone[i] = XMMatrixRotationY(XMConvertToRadians(sinf(ang) * 70.f));
	}

	//	�e���猩���p�����X�V
	for (int i = 0; i < 7; ++i) {
		XMStoreFloat4x4(&g_Bones[i].bornMat, defBone[i] * XMLoadFloat4x4(&g_Bones[i].initMat));
	}

	//	�p�������[�J���ϊ�
	XMFLOAT4X4 global;
	XMStoreFloat4x4(&global, XMMatrixRotationZ(ang * 0.1f));
	UpdateBone(g_Bones, &global);

#endif // BONE



#pragma endregion

#pragma region �萔�o�b�t�@

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
	//cb.bornMat

#pragma region �{�[���̌v�Z�H

	for (int i = 0; i < 7; ++i) {
		//	�ϊ��s��
		cb.bornMat[i] = g_Bones[i].combMatPtr[i];

		//	�����p��
		//cb.bornMat[i] = XMMatrixIdentity();

		//	�[���s��
		//cb.bornMat[i] = XMMATRIX(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		
		//���󂯓n�����s��Ȃ��ƕ`�悪���������Ȃ邵�A
		//�@�[���s��𓊂���ƕ`�悳��Ȃ�
	}

#pragma endregion

	//	�������R�s�[
	memcpy_s(mp.pData, mp.RowPitch, (void*)(&cb), sizeof(cb));

	//	�A�N�Z�X���I��
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*m_pShader->GetConstantBuffer(),
		NULL
	);

	//	���_�V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		m_pShader->GetConstantBuffer()
	);

	//	�s�N�Z���V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		m_pShader->GetConstantBuffer()
	);
#pragma endregion


#pragma endregion

#pragma region �V�F�[�_�[�̃o�C���h
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*m_pShader->GetVertexShader(),
		0, 0
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*m_pShader->GetPixelShader(),
		0, 0
	);
#pragma endregion

#pragma region ���_���C�A�E�g
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		*m_pShader->GetInputLayout()
	);
#pragma endregion

#pragma region ���_�o�b�t�@
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

#pragma region �C���f�b�N�X�o�b�t�@
	static constexpr uint32_t indexBufferOffset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		indexBufferOffset
	);

#pragma endregion

	//�`��
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
}

HRESULT API::Anim::SkeltonAnimationMesh::CreateVertexBuffer(std::vector<AnimVertex> verttices)
{
	//	�o�b�t�@�̎d�l
	D3D11_BUFFER_DESC bd;

	/*
		���_�𓮓I�ɏ���������ꍇ�� "DYNAMIC"�ɂ��邱�ƁI
	*/
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AnimVertex) * verttices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = NULL;

	//	�T�u���\�[�X�̎d�l
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = verttices.data();

	//	���_�o�b�t�@�쐬
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pVertexBuffer.GetAddressOf()
	);
}

HRESULT API::Anim::SkeltonAnimationMesh::CreateIndexBuffer(std::vector<uint32_t> indices)
{
	m_IndexCount = indices.size();

	//	�o�b�t�@�̎d�l
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t)*indices.size();
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = NULL;

	//	�T�u���\�[�X�̎d�l	
	D3D11_SUBRESOURCE_DATA sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = indices.data();

	//	�C���f�b�N�X�o�b�t�@�쐬
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&sd,
		m_pIndexBuffer.GetAddressOf()
	);

	return E_NOTIMPL;
}

