/*
	@file	Mesh.h
	@date	2019/05/17
	@author	�ԏ� �G�P
	@brief	���b�V��
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
#include "MeshReadHelper.h"
#include "Camera.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	�R���X�g���N�^
*/
Mesh::Mesh() :m_IndexCount(0) {
	transform = std::make_shared<Transform>();
}

/*!
	@brief	���[�u�R���X�g���N�^
*/
API::Mesh::Mesh(const Mesh & inst)
{
	*this = inst;
}

/*!
	@brief	�f�X�g���N�^
	@detail	�J�������̌Ăяo��
*/
Mesh::~Mesh() {
	Finalize();
}

/*!
	@fn			Initialize
	@brief		������
	@detail		���f���݂̂̓ǂݍ��݂ŕʓr�e�N�X�`�����쐬����K�v������
	@param[in]	�O���t�@�C���̃p�X
	@return	S_OK:���� E_FAIL:���s
*/
HRESULT Mesh::Initialize(std::string path)
{
	try
	{
		//	�O���t�@�C���̓ǂݎ��
		auto rd = Helper::MeshReadHelper::Read(path);

		//	�T���v���[
		/*if (FAILED(CreateSamplerState(this))) {
			string error = "\"" + path + "\" (mesh sampler) is not create!";
			throw error;
		}*/

		//	���_�o�b�t�@�쐬
		if (FAILED(CreateVertexBuffer(this, rd.vertices))) {
			string error = "\"" + path + "\" (mesh vertex) is not create!";
			throw error;
		}

		//	�C���f�b�N�X�o�b�t�@�쐬
		if (FAILED(CreateIndexBuffer(this, rd.indices))) {
			string error = "\"" + path + "\" (mesh index) is not create!";
			throw error;
		}
	}
	catch (const string error)
	{
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn			Initialize
	@brief		������
	@detail		���f���ƃe�N�X�`���𓯎��ɐ�������
	@param[in]	�O���t�@�C���̃p�X
	@param[in]	���蓖�Ă�e�N�X�`���̃p�X
	@return	S_OK:���� E_FAIL:���s
*/
HRESULT API::Mesh::Initialize(std::string meshPath, std::string texPath)
{
	HRESULT hr;
	hr = Initialize(meshPath);
	
	//	SRV
	//hr = CreateTexture(texPath);

	//if (FAILED(hr)) {
	//	string error = "\"" + texPath + "\" (mesh shader resource view) is not create!";
	//	ErrorLog(error);
	//	return E_FAIL;
	//}

	//if (FAILED(hr)) {
	//	//	���b�V���̏������Ɏ��s
	//	return E_FAIL;
	//}

	return S_OK;
}

/*!
	@fn		Finalize
	@brief	�j������
	@detail	�����o�̖����I�ȊJ��
*/
void API::Mesh::Finalize()
{
	//	�V�F�[�_�[�̎�Q�ƊJ��
	m_pShader.reset();

	//	�C���f�b�N�X�o�b�t�@�J��
	m_pIndexBuffer.Reset();

	//	���_��������
	m_IndexCount = 0;

	//	���_�o�b�t�@�J��
	m_pVertexBuffer.Reset();

	//	�T���v���[�X�e�[�g�J��
	//m_pSamplerState.Reset();

	//	SRV�J��
	//m_pSRV.Reset();
	m_pMaterial.reset();
}

/*!
	@fn		Render
	@brief	�`�揈��
	@detail	�������z�֐����I�[�o�[���C�h
*/
void Mesh::Render()
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

	//	�e�N�X�`��
	SetupTexture();

	//	�`�施��
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
}

/*!
	@fn		SetupTopology
	@brief	�g�|���W�[�̃Z�b�g�A�b�v
*/
void API::Mesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*!
	@fn		SetupInputLayout
	@brief	���_���C�A�E�g�̃Z�b�g�A�b�v
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
	//	���_���ێ�
	mesh->m_IndexCount = indices.size();

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
		mesh->m_pIndexBuffer.GetAddressOf()
	);
}

/*!
	@fn			SetupIndexBuffer
	@brief		���������C���f�b�N�X�o�b�t�@�̃Z�b�g
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
	@brief	���_�o�b�t�@�̃Z�b�g�A�b�v
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
	@brief	�R���X�^���g�o�b�t�@�̃Z�b�g�A�b�v
	@return	S_OK:���� E_FAIL:���s
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
		w = XMMatrixTranspose(transform->GetMatrix());

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

		//	�f�[�^�̊i�[
		value.m.world = w;
		value.m.view = v;
		value.m.proj = p;
		value.color = color.GetRGBA();

		//	�������R�s�[
		memcpy_s(mapped.pData, mapped.RowPitch, (void*)(&value), sizeof(value));

		//	�A�N�Z�X���I��
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(
			*shader->GetConstantBuffer(),
			NULL
		);

		//	�R���X�^���g�o�b�t�@���V�F�[�_�[�ɓo�^

		//	���_�V�F�[�_�[
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
			0,
			1,
			shader->GetConstantBuffer()
		);

		//	�s�N�Z���V�F�[�_�[
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
	@brief	ImmediateContext�ɃV�F�[�_�[���o�C���h
	@detail	�Z�b�g����V�F�[�_�[�͒��_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[
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

		//	���_�V�F�[�_�[
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
			*shader->GetVertexShader(),
			NULL,
			0
		);

		//	�s�N�Z���V�F�[�_�[
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
	@brief	�e�N�X�`���̃Z�b�g�A�b�v
	@detail	SRV�ƃT���v���[�̃Z�b�g�A�b�v���s��
	@note	�e�N�X�`���̖������f�����z�肵�AError�ɂ�郁�b�Z�[�W�{�b�N�X�͕\�����Ȃ�
*/
void API::Mesh::SetupTexture()
{
	auto material = m_pMaterial.lock();


	//	�T���v���[
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

/*!
	@template	Vertex
	@brief		CreateVertexBuffer�֐��Ŏg�����_�\���̂̂��߂̃e���v���[�g
*/
template <class Vertex>

/*!
	@fn			CreateVertexBuffer
	@brief		���_�o�b�t�@����
	@param[in]	������̃��b�V��
	@param[in]	���_�o�b�t�@���\�����钸�_���
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT API::Mesh::CreateVertexBuffer(Mesh*mesh, std::vector<Vertex>verttices)
{
	//	�o�b�t�@�̎d�l
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * verttices.size();
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
		mesh->m_pVertexBuffer.GetAddressOf()
	);
}