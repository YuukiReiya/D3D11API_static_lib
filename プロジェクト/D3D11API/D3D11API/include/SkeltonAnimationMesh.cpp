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

#pragma region �萔�o�b�t�@
struct alignas(16) AnimConstantBuffer
{
	MatrixConstantBuffer m;
	//DirectX::XMMATRIX bornMat[12];//packoffset���Ō�Ɏ����Ă���̈��肩�c


	//	18 = 1���_��"�e����^����{�[����" * �|���S�����\�����钸�_��(�O�p�|���S���Ȃ̂� = 3)
	//�� 6(7) * 3 = 18(21)
	//DirectX::XMMATRIX bornMat[18];//packoffset���Ō�Ɏ����Ă���̈��肩�c
	DirectX::XMMATRIX bornMat[60];//packoffset���Ō�Ɏ����Ă���̈��肩�c
};
#pragma endregion

#pragma region �{�[��
struct Bone
{
#if 0
	//	API
	unsigned int id;		//	�{�[���̔ԍ�(�C���f�b�N�X�Ƃ��ċ@�\)
	XMFLOAT4X4 offsetMat;	//	�I�t�Z�b�g�s��
	XMMATRIX* combMatPtr;	//	�e���x�𒼐ڊǗ�����z��̃|�C���^(XMMATRIX*[]�^)

	//	Converter
	Bone* child;			//	���g�̎q�{�[��
	Bone* sibling;			//	���g�Ɠ����e�����Z��{�[��
	XMFLOAT4X4 initMat;		//	�����p��
	std::vector<XMFLOAT4X4> frameMat;	//	�t���[���s��


	Bone() :id(), child(), sibling(), combMatPtr() {
		XMStoreFloat4x4(&initMat, XMMatrixIdentity());
		XMStoreFloat4x4(&offsetMat, XMMatrixIdentity());
	}
#else
	//	�I�t�Z�b�g�s��
	XMMATRIX offsetMat;

	//	�����p��
	XMMATRIX initMat;

	//	�t���[�����p��(0�t���[��)
	XMMATRIX frameMat;
#endif

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
//Bone*g_Bones = new Bone[7];
std::vector<Bone>g_Bones;

#endif // BONE

HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	//auto data = Helper::MeshReadHelper::Read("twin.yfm");
	auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");


	//	���_
	vector<AnimVertex>vv;
#pragma region ���_
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

		//	���W
		av.position = it.position;

		//	�d��
		for (size_t i = 0; i < c_AffectedBoneCount; i++)
		{
			//	�ǂݍ��݃f�[�^�����݂��Ȃ�
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

	//	���_�C���f�b�N�X
	vector<uint32_t>vi;
#pragma region ���_�C���f�b�N�X
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

#pragma region �{�[����񏉊���
#ifdef BONE
	const unsigned int c_BoneCount = data.initialMatrix.size();
	const unsigned int c_AnimFrame = data.frame;
	for (size_t i = 0; i < c_BoneCount; i++)
	{
		Bone bone;


		//	�����p��
		//bone.initMat = data.initialMatrix[i];
		bone.initMat = XMLoadFloat4x4(&data.initialMatrix[i]);
		//	�t���[�����p��
		/*for (size_t f = 0; f < c_AnimFrame; f++)
		{
			bone.frameMat.push_back(data.frameMatrix[f][i]);

		}*/
		//bone.frameMat = data.frameMatrix[0][i];
		bone.frameMat = XMLoadFloat4x4(&data.frameMatrix[0][i]);

		//	�I�t�Z�b�g�s�� = �����p���̋t�s��
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
	//cb.frameMat

#pragma region �{�[���̌v�Z�H

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

