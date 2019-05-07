#include "stdafx.h"
#include "Mesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Graphic
*/
using namespace D3D11::Graphic;

/*!
	@brief	�R���X�g���N�^
*/
Mesh::Mesh() 
{
	transform = std::make_shared<Transform>();
}

/*!
	@brief	�f�X�g���N�^
*/
Mesh::~Mesh()
{
}

/*!
	@fn			Initialize
	@brief		������
	@param[in]	�ǂݍ��ރt�@�C���̃p�X
*/
HRESULT Mesh::Initialize(std::string path)
{
	auto data = Helper::MeshReadHelper::Read(path);

	//	���_����
	if (FAILED(CreateVertexBuffer(this, data.vertices))) {
		return E_FAIL;
	}
	//	���_�o�b�t�@�ݒ�
	SetupVertexBuffer();

	//	�C���f�b�N�X�o�b�t�@�쐬
	if (FAILED(CreateIndexBuffer(this, data.indices))) {
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@fn		Render
	@brief	�`��
*/
void Mesh::Render()
{
	//	�g�|���W�[�̃Z�b�g
	SetupTopology();

	//	�V�F�[�_�[�̃Z�b�g
	SetupBindShader();

	//	�R���X�^���g�o�b�t�@�̃Z�b�g
	SetupConstantBuffer();

	SetupIndexBuffer();

	//	�`��
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
	@brief		�V�F�[�_�[�̐ݒ�
	@detail		��Q�ƂŃo�C���h����
	@param[in]	�o�^����V�F�[�_�[�̃|�C���^
*/
void API::Mesh::SetupShader(D3D11::Graphic::AbstractShader * shader)
{
	m_pShader = shader->GetSharedPtr();

}

/*!
	@fn		SetupTopology
	@brief	�g�|���W�[�̐ݒ�
*/
void API::Mesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

/*!
	@fn		SetupConstantBuffer
	@brief	�R���X�^���g�o�b�t�@�̐ݒ�
*/
void API::Mesh::SetupConstantBuffer()
{
	auto shader = *m_pShader.lock();
	auto& device = Direct3D11::GetInstance();

	//	���[���h�s��
	DirectX::XMMATRIX world = transform->GetMatrix();

	//	�r���[�s��
	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();

	//	�v���W�F�N�V�����s��
	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();

	//	���_�V�F�[�_�[�p��C�o�b�t�@�o�^
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBuffer()
	);

	//	�s�N�Z���V�F�[�_�[�p��C�o�b�t�@�o�^
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBuffer()
	);

	//	�}�b�s���O�p�ϐ��̐錾
	D3D11_MAPPED_SUBRESOURCE pMapData;

	//	�o�b�t�@�ւ̃A�N�Z�X(��������)����
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
		//	�A�N�Z�X������Ĕ�����
		device.GetImmediateContext()->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	MeshShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	�o�b�t�@�ɑ��
	cb.m_WorldMatrix = world;
	cb.m_ViewMatrix = view;
	cb.m_ProjectionMatrix = proj;
	cb.m_Color = color.GetRGBA();

	//	�������R�s�[
	memcpy_s(pMapData.pData, pMapData.RowPitch, (void*)(&cb), sizeof(cb));

	//	�A�N�Z�X���I��
	device.GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);
}

/*!
	@fn		SetupBindShader
	@brief	�V�F�[�_�[�̐ݒ�
*/
void API::Mesh::SetupBindShader()
{
	auto shader = *m_pShader.lock();

	//	���_�V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*shader->GetVertexShader(),
		NULL,
		NULL
	);

	//	�s�N�Z���V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*shader->GetPixelShader(),
		NULL,
		NULL
	);

}

/*!
	@fn			CreateVertexBuffer
	@brief		���b�V���̒��_�𐶐�
	@detail		�ÓI�֐�
	@param[in]	�\�����钸�_���
	@param[in]	�\�����钸�_�C���f�b�N�X
	@param[in]	���b�V���̃|�C���^
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::Mesh::CreateVertexBuffer(Mesh * mesh, std::vector<MeshVertex> vertex)
{
	//	���_��`
	std::vector<MeshVertex>v;
	for (auto it : vertex)
	{
		MeshVertex tmp;
		tmp.m_Pos.x = it.m_Pos.x;
		tmp.m_Pos.y = it.m_Pos.y;
		tmp.m_Pos.z = it.m_Pos.z;
		v.push_back(tmp);
	}

	//	�o�b�t�@��`
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;								// GPU����ǂݍ��݂Ə������݂�����
	bd.ByteWidth = sizeof(MeshVertex)*vertex.size();			// �o�b�t�@�̃T�C�Y
	//bd.ByteWidth = sizeof(v);									// �o�b�t�@�̃T�C�Y
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h

	// �T�u���\�[�X�̃f�[�^���`
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = v.data();					// �������f�[�^�ւ̃|�C���^

	// ���_�o�b�t�@�̊J��
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
	@brief	���_�o�b�t�@�ݒ�
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
	@brief		�C���f�b�N�X�o�b�t�@�쐬
	@detail		�ÓI�֐�
	@param[in]	�ݒ肷�郁�b�V���̃|�C���^
	@param[in]	���_�̃C���f�b�N�X
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::Mesh::CreateIndexBuffer(Mesh * mesh, std::vector<uint32_t> index)
{
	mesh->m_pIndexBuffer.Reset();
	mesh->m_VertexIndex = index;

	//	�o�b�t�@��`
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t)*index.size();
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	
	// �T�u���\�[�X�̃f�[�^���`
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = mesh->m_VertexIndex.data();		// �������f�[�^�ւ̃|�C���^

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
	@brief	�C���f�b�N�X�o�b�t�@�ݒ�
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