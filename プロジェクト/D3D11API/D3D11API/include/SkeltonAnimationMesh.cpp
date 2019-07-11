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
	DirectX::XMMATRIX boneMat[60];
};
#pragma endregion

#pragma region �{�[��
struct Bone
{
	unsigned int id;
	Bone* child;
	Bone* sibling;
	XMMATRIX offsetMat;
	XMMATRIX initMat;
	XMMATRIX boneMat;
	XMMATRIX* combMatPtr;
	Bone() :
		id(),
		child(),
		sibling(),
		combMatPtr(),
		initMat(XMMatrixIdentity()),
		offsetMat(XMMatrixIdentity()),
		boneMat(XMMatrixIdentity())
	{}
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

//====================================================-
static constexpr int c_BoneCount = 60;
Bone g_Bones[c_BoneCount];

//====================================================-
HRESULT API::Anim::SkeltonAnimationMesh::Init()
{
	auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");

	//	���_
	vector<AnimVertex>vv;
	for (auto it : data.vertices) {
		vv.push_back({it.position});
	}

	//	�d��
	for (int i = 0; i < vv.size(); ++i)
	{
		for (int j = 0; j < 4; ++j) {

			//�_�~�[
			if (data.vertices[i].indexOfBonesAffested.size() <= j) {
				vv[i].bornIndex[j] = 0;
				switch (i)
				{
				case 0:
					vv[i].weight.x = 0;
					break;
				case 1:
					vv[i].weight.y = 0;
					break;
				case 2:
					vv[i].weight.z = 0;
					break;
				default:
					break;
				}

				continue;
			}
			
			// ����

			//	�֘A�{�[���C���f�b�N�X
			vv[i].bornIndex[j] = data.vertices[i].indexOfBonesAffested[j];

			int index = vv[i].bornIndex[j];

			//	�d��
			switch (i)
			{
			case 0:
				vv[i].weight.x = data.vertices[i].weight[index];
					break;
			case 1:
				vv[i].weight.y = data.vertices[i].weight[index];
				break;
			case 2:
				vv[i].weight.z = data.vertices[i].weight[index];
				break;
			default:
				break;
			}
			
		}
	}

	//	���_�C���f�b�N�X
	vector<uint32_t>vi;
#pragma region ���_�C���f�b�N�X
#pragma endregion
	vi = data.indices;

#pragma region �{�[�����
	//	�����p��
	for (int i = 0; i < c_BoneCount; ++i)
	{
		g_Bones[i].initMat = XMLoadFloat4x4(&data.initialMatrix[i]);
	}

	//	�t���[���s��
	for (int i = 0; i < c_BoneCount; ++i)
	{
		g_Bones[i].boneMat = XMLoadFloat4x4(&data.frameMatrix[0][i]);
	}

	//	�I�t�Z�b�g�s��
	for (int i = 0; i < c_BoneCount; ++i)
	{
		g_Bones[i].offsetMat = XMMatrixInverse(NULL, g_Bones[i].initMat);
	}

	//	�����s��
	XMMATRIX* compMatrix = new XMMATRIX[c_BoneCount];
	for (int i = 0; i < c_BoneCount; ++i)
	{
		g_Bones[i].combMatPtr = compMatrix;
	}
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
}

void API::Anim::SkeltonAnimationMesh::Render()
{
#pragma region �g�|���W�[
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

#pragma region �{�[���̌v�Z�H

	//for (int i = 0; i < 7; ++i) {
	//	//	�ϊ��s��
	//	cb.boneMat[i] = g_Bones[i].combMatPtr[i];
	//
	//	//	�����p��
	//	//cb.boneMat[i] = XMMatrixIdentity();
	//
	//	//	�[���s��
	//	//cb.boneMat[i] = XMMATRIX(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	//	
	//	//���󂯓n�����s��Ȃ��ƕ`�悪���������Ȃ邵�A
	//	//�@�[���s��𓊂���ƕ`�悳��Ȃ�
	//}

	//	�����s��
	for (int i = 0; i < c_BoneCount; ++i)
	{
		g_Bones[i].combMatPtr[i] = g_Bones[i].initMat*g_Bones[i].boneMat;
	}

	for (int i = 0; i < c_BoneCount; ++i)
	{
		//	�ϊ��s��
		cb.boneMat[i] = XMMatrixTranspose(g_Bones[i].combMatPtr[i]);
		//cb.boneMat[i] = g_Bones[i].combMatPtr[i];

		//cb.boneMat[i] = XMMatrixIdentity();
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

