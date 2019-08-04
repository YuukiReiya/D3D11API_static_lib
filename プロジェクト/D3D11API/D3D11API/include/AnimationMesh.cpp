/*!
	@file	AnimationMesh.cpp
	@date	2019/08/04
	@author	�ԏ� �G�P
	@brief	�t���[���A�j���[�V�������b�V��
*/
#include "stdafx.h"
#include "AnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"
#include "MeshShader.h"
#include "MeshConstantBuffer.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Graphic
*/
using namespace Graphic;

/*!
	@fn			Initialize
	@brief		������
	@param[in]	�ǂݍ��݃p�X
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::AnimationMesh::Initialize(std::string path)
{
	m_pShader = std::make_shared<MeshShader>();
	if (FAILED(m_pShader->Setup())) {
		return E_FAIL;
	}

	HRESULT hr;
	auto device = Direct3D11::GetInstance().GetDevice();
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	auto mesh = Helper::MeshReadHelper::ReadFrameAnim(path);
	m_IndexCount = mesh.indices.size();
	m_VertexCount = mesh.vertices[0][0].size();

	for (int i = 0; i < mesh.vertices.size(); ++i) {
		for (int j = 0; j < mesh.vertices[i].size(); ++j) {
			m_VertexList[i][j] = mesh.vertices[i][j];
		}
	}
	
	std::vector<MeshVertex> vertices;
	for (size_t i = 0; i < mesh.vertices[0][0].size(); i++)
	{
		auto& pos = mesh.vertices[0][0][i];
		//auto&uv = mesh.uv[i];
		DirectX::XMFLOAT2 uv = {-1,-1};
		MeshVertex v;
		v.position = pos;
		v.uv = uv;
		m_UV.push_back(uv);
		vertices.push_back(v);
	}

	if (FAILED(CreateVertexBuffer(vertices))) { return E_FAIL; }
	if (FAILED(CreateIndexBuffer(mesh.indices))) { return E_FAIL; }
	return S_OK;
}

/*!
	@fn		Render
	@brief	�`��
*/
void API::AnimationMesh::Render()
{
	//	�g�|���W�[
	SetupTopology();

	//	�R���X�^���g�o�b�t�@
	SetupConstantBuffer();

	//	�V�F�[�_�[�̃o�C���h
	SetupBindShader();

	//	���_���C�A�E�g�̃Z�b�g�A�b�v
	SetupInputLayout();

	//	���_�o�b�t�@�̃Z�b�g�A�b�v
	SetupVertexBuffer();

	//	�C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
	SetupIndexBuffer();

	//	�`�施��
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
}

/*!
	@fn			CreateVertexBuffer
	@brief		���_�o�b�t�@�̍쐬
	@param[in]	�ϒ��z��Ɋi�[�������_���
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::AnimationMesh::CreateVertexBuffer(std::vector<MeshVertex> verttices)
{
	//	�o�b�t�@�̎d�l
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MeshVertex) * verttices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
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

/*!
	@fn			CreateIndexBuffer
	@brief		�C���f�b�N�X�o�b�t�@�̍쐬
	@param[in]	�ϒ��z��Ɋi�[�����C���f�b�N�X���
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::AnimationMesh::CreateIndexBuffer(std::vector<uint32_t> indices)
{
	//	���_���ێ�
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

/*!
	@fn		SetupTopology
	@brief	�g�|���W�[�̐ݒ�
*/
void API::AnimationMesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*!
	@fn		SetupInputLayout
	@brief	���_���C�A�E�g�̐ݒ�
*/
void API::AnimationMesh::SetupInputLayout()
{
	try
	{
		//	shader
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetInputLayout()) {
			throw "this shader is inputlayout has an invalid value";
		}
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
			*m_pShader->GetInputLayout()
		);
	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return;
	}
}

/*!
	@fn		SetupIndexBuffer
	@brief	�C���f�b�N�X�o�b�t�@�̐ݒ�
*/
void API::AnimationMesh::SetupIndexBuffer()
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
	@brief	���_�o�b�t�@�̐ݒ�
*/
void API::AnimationMesh::SetupVertexBuffer()
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
	@brief	�R���X�^���g�o�b�t�@�̐ݒ�
	@return	����:S_OK ���s:E_FAIL
*/
HRESULT API::AnimationMesh::SetupConstantBuffer()
{
	try
	{
		//	shader
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetConstantBuffer()) {
			throw "this shader is constant buffer has an invalid value";
		}

		HRESULT hr;
		DirectX::XMMATRIX w, v, p;

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
			*m_pShader->GetConstantBuffer(),
			NULL,
			D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
			NULL,
			&mapped
		);
		if (FAILED(hr))
		{
			Direct3D11::GetInstance().GetImmediateContext()->Unmap(
				*m_pShader->GetConstantBuffer(),
				NULL
			);
			throw "Failed to map constant buffer for this shader";
		}

		//	�f�[�^�̊i�[
		value.matrix.world = w;
		value.matrix.view = v;
		value.matrix.proj = p;
		value.color = {1,1,1,1};

		//	�������R�s�[
		memcpy_s(mapped.pData, mapped.RowPitch, (void*)(&value), sizeof(value));

		//	�A�N�Z�X���I��
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(
			*m_pShader->GetConstantBuffer(),
			NULL
		);

		//==================================================
		//	���_�o�b�t�@�̏�������
		auto context = Direct3D11::GetInstance().GetImmediateContext();
		hr = context->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		if (FAILED(hr)) {
			ErrorLog("anim map failed");
		}

		std::vector<MeshVertex>vVertList;
		for (size_t i = 0; i < m_UV.size(); i++)
		{
			MeshVertex v;
			v.position = m_VertexList[m_AnimIndex.x][m_AnimIndex.y][i];
			v.uv = m_UV[i];
			vVertList.push_back(v);
		}
		memcpy_s(
			mapped.pData,
			mapped.RowPitch,
			(void*)(vVertList.data()),
			sizeof(Graphic::MeshVertex) * m_VertexCount
		);
		context->Unmap(m_pVertexBuffer.Get(), 0);
		//==================================================


		//	�R���X�^���g�o�b�t�@���V�F�[�_�[�ɓo�^

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

	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return E_FAIL;
	}


	return S_OK;
}

/*!
	@fn		SetupBindShader
	@brief	�V�F�[�_�[�̐ݒ�
*/
void API::AnimationMesh::SetupBindShader()
{
	try
	{
		if (!m_pShader) {
			throw "this setup bind shader is out of reference";
		}
		if (!*m_pShader->GetVertexShader()) {
			throw "this shader is vertex shader has an invalid value";
		}
		if (!*m_pShader->GetPixelShader()) {
			throw "this shader is pixel shader has an invalid value";
		}

		//	���_�V�F�[�_�[
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
			*m_pShader->GetVertexShader(),
			NULL,
			0
		);

		//	�s�N�Z���V�F�[�_�[
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
			*m_pShader->GetPixelShader(),
			NULL,
			0
		);
	}
	catch (const std::string error)
	{
		ErrorLog(error);
		return;
	}

}