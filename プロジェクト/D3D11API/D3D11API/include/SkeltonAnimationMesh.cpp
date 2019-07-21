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
	//auto data = Helper::MeshReadHelper::ReadAnim("Anim/anim.yfm");
	auto data = Helper::MeshReadHelper::ReadAnim("Anim/humanoid.yfm");

#pragma region �ǂݍ��݃f�[�^�̃m�[�}���C�Y

	//	���_(�����p��)
	for (auto it : data.vertices) {
		MeshVertex v;
		v.position = it.position;
		m_Vertex.push_back(v);
	}
	//m_Vertex = data.vertices;

	//	�I�t�Z�b�g�s��
	for (auto it : data.initialMatrix)
	{
		XMMATRIX ofMat = XMMatrixInverse(0, XMLoadFloat4x4(&it));
		//XMMATRIX ofMat = XMLoadFloat4x4(&it);
		m_OffsetMatrix.push_back(ofMat);
	}

	//	�t���[���s��̃X�^�b�N�m��
	m_FrameMatrix.resize(data.initialMatrix.size());
	for (auto& it : m_FrameMatrix)
	{
		it.resize(30);
	}

	//	�t���[���s��
	for (int frame = 0; frame < data.frame; ++frame)
	{
		for (int bone = 0; bone < data.initialMatrix.size(); ++bone) {
			XMMATRIX mat = XMLoadFloat4x4(&data.frameMatrix[frame][bone]);
			m_FrameMatrix[bone][frame] = mat;
		}
	}
	
	//	�d�� & �֘A�{�[���ԍ�
	for (auto v : data.vertices) {

		RelationInfo ri;
		ri.boneNo = v.indexOfBonesAffested;
		for (auto it : v.indexOfBonesAffested) {
			ri.weight.push_back(v.weight[it]);
		}
		m_RI.push_back(ri);
	}

	for (int c = 0; c < m_RI.size(); ++c) {

		float tWeight = 0;
		int count = m_RI[c].boneNo.size();
		for (int i = 0; i < count; ++i) {
			tWeight += m_RI[c].weight[i];
		}

		if (tWeight != 1.0f) {
			//ErrorLog("this");
		}
	}

	//	�������_�ۑ�
	m_InitVertex = m_Vertex;

#pragma endregion


	//	���_�o�b�t�@
	if (FAILED(CreateVertexBuffer(m_Vertex))) {
		ErrorLog("create vertex");
	}

	//	�C���f�b�N�X
	if (FAILED(CreateIndexBuffer(data.indices))) {
		ErrorLog("create index");
	}

	//	�V�F�[�_�[
	m_pShader = make_shared<MeshShader>();
	m_pShader->Setup();

	//	�g�����X�t�H�[��
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

#pragma region �g�|���W�[
	//
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region GPU���M
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

#pragma region �萔�o�b�t�@���M
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

	//	�������R�s�[
	memcpy_s(mp.pData, mp.RowPitch, (void*)(&cb), sizeof(cb));

	//	�A�N�Z�X���I��
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*m_pShader->GetConstantBuffer(),
		NULL
	);
#pragma endregion

#pragma region ���_�̏�������

	auto context = Direct3D11::GetInstance().GetImmediateContext();
	hr = context->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mp);
	if (FAILED(hr)) {
		ErrorLog("vertex map failed");
	}

	//
	//	���_�̏�������
	//
#define OVERWRITE_VERTEX
#ifdef OVERWRITE_VERTEX
	{
		//	�m�F�p�R�[�h
		//if (Keyboard::GetButtonDown('M')) {
		//	m_Vertex[0].position = { 0,0,0 };
		//	m_Vertex[1604].position = { 0,0,0 };
		//}

#pragma region �A�j���[�V�����p�R�[�h
#if 0
		static int frame = 0;
		static int animIndex = 0;
		const int updateframe = 30;
		frame++;
		if (frame > updateframe) {
			animIndex = animIndex < 30 - 1 ? ++animIndex : 0;
			frame = 0;
		}
#else
		static int animIndex = 0;
#endif // 0
#pragma endregion

		//	�{�[���s������߂�
		//	BM = offsetMat * FrameMatrix
		vector<XMMATRIX>boneMat;
		for (int i = 0; i < m_FrameMatrix.size(); ++i)
		{
			XMMATRIX mat = m_OffsetMatrix[i] * m_FrameMatrix[i][animIndex];
			boneMat.push_back(mat);
		}

#pragma region �����p�����W
#if 1
		m_Vertex = m_InitVertex;
#endif // 1
#pragma endregion

#pragma region v = ��(V(n-1) * W(n-1) * FM(n-1)) + (V(n) * (1.0f - W(n)) * FB(n)) 
#if 0
		for (int vCount = 0; vCount < m_Vertex.size(); ++vCount)
		{
			auto& v = m_Vertex[vCount];
			auto loopCount = m_RI[vCount].boneNo.size() - 1;
			XMVECTOR comb = {0,0,0,0};
			float lastWeight = 0;
			for (int i = 0; i < loopCount; ++i)
			{
				auto boneIndex = m_RI[vCount].boneNo[i];
				auto weight = m_RI[vCount].weight[i];
				lastWeight += weight;
				XMVECTOR pos = {
					v.position.x,
					v.position.y,
					v.position.z,
					1
				};
				XMMATRIX m = boneMat[boneIndex] * weight;
				m = XMMatrixTranspose(m);
				comb += XMVector4Transform(pos, m);
			}
			XMVECTOR lastPos = {
				v.position.x,
				v.position.y,
				v.position.z,
				1
			};
			XMMATRIX lastM = boneMat[m_RI[vCount].boneNo[loopCount]] * (1.0f - lastWeight);
			lastM = XMMatrixTranspose(lastM);
			comb += XMVector4Transform(lastPos, lastM);


			//	���_
			v.position = {
				comb.m128_f32[0],
				comb.m128_f32[1],
				comb.m128_f32[2],
			};
		}
#elif 0
#pragma endregion
#pragma region v = ��(V(n) * W(n) * FB(n))
		for (int vCount = 0; vCount < m_Vertex.size(); ++vCount)
		{
			auto& v = m_Vertex[vCount];
			auto loopCount = m_RI[vCount].boneNo.size();
			XMVECTOR comb = { 0,0,0,0 };
			for (int i = 0; i < loopCount; ++i)
			{
				auto boneIndex = m_RI[vCount].boneNo[i];
				auto weight = m_RI[vCount].weight[i];
				XMVECTOR pos = {
					v.position.x,
					v.position.y,
					v.position.z,
					1
				};
				XMMATRIX m = boneMat[boneIndex] * weight;
				m = XMMatrixTranspose(m);
				comb += XMVector4Transform(pos, m);
			}

			//	���_
			v.position = {
				comb.m128_f32[0],
				comb.m128_f32[1],
				comb.m128_f32[2],
			};
		}
#pragma endregion
#else
#pragma region comb:XMMATRIX
		for (int vCount = 0; vCount < m_Vertex.size(); ++vCount)
		{
			auto& v = m_Vertex[vCount];
			auto loopCount = m_RI[vCount].boneNo.size()-1;
			XMMATRIX comb = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
			//XMMATRIX comb = XMMatrixIdentity();
			float lastWeight = 0;
			for (int i = 0; i < loopCount; ++i)
			{
				auto boneIndex = m_RI[vCount].boneNo[i];
				auto weight = m_RI[vCount].weight[i];
				lastWeight += weight;
				comb += boneMat[boneIndex] * weight;
			}

			comb += boneMat[loopCount] * (1.0f - lastWeight);


			XMVECTOR pos = {
				v.position.x,
				v.position.y,
				v.position.z,
				1
			};
			pos = XMVector4Transform(pos, comb);

			//	���_
			v.position = {
				pos.m128_f32[0],
				pos.m128_f32[1],
				pos.m128_f32[2],
			};
		}
#pragma endregion

#endif 
	}
#endif

	memcpy_s(
		mp.pData,
		mp.RowPitch,
		(void*)(m_Vertex.data()),
		sizeof(Graphic::MeshVertex) * m_Vertex.size()
	);
	context->Unmap(m_pVertexBuffer.Get(), 0);

#pragma endregion


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


template<class Vertex>
HRESULT API::Anim::SkeltonAnimationMesh::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	//	�o�b�t�@�̎d�l
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = NULL;

	//	�T�u���\�[�X�̎d�l
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = vertices.data();

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
}
