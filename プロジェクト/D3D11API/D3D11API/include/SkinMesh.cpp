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
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

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
	@brief	using�f�B���N�e�B�u
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	�R���X�g���N�^
*/
API::SkinMesh::SkinMesh()
	: m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
	, frameIndex(0)
{
	transform = make_shared<Transform>();
}

/*!
	@brief	�f�X�g���N�^
*/
API::SkinMesh::~SkinMesh()
{
}

/*!
	@fn			Initialize
	@brief		������
	@param[in]	�ǂݍ��ݗp�̃t�@�C���p�X
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT API::SkinMesh::Initialize(std::string path)
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr = E_FAIL;

	try
	{
		m_pShader = make_shared<MeshShader>();
		if (FAILED(m_pShader->Setup())) { throw runtime_error("shader setup"); }

		//	�t�@�C���ǂݍ���
		if (!Load(this, path)) { throw runtime_error("load to file"); }

		//	�X�L�j���O�̎w��(�\�t�g�E�F�A�X�L�j���O or �V�F�[�_�[)
		static constexpr uint32_t c_ShaderJointCount = 4;
		m_eSkinningMode = m_Vertices.end() != std::find_if(
			m_Vertices.begin(), m_Vertices.end(),
			[](SkinnedVertex v)-> bool { return v.joints.size() > c_ShaderJointCount; }
		) ? SkinningMode::SOFTWARE : SkinningMode::SHADER;

		//	�C���f�b�N�X�o�b�t�@�쐬
		if (FAILED(CreateIndexBuffer())) { throw runtime_error("create index buffer"); }

		//	���_�o�b�t�@�쐬
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
	@brief	�`��
*/
void API::SkinMesh::Render()
{
	//	�g�|���W�[
	SetupTopology();

	//	�\�t�g�E�F�A�X�L�j���O
	SoftwareSkinning();

	//	���_���C�A�E�g
	SetupInputLayout();

	//	���_�o�b�t�@
	SetupVertexBuffer();

	//	�C���f�b�N�X�o�b�t�@
	SetupIndexBuffer();

	//	�e�N�X�`��
	SetupTexture();

	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_Indices.size(), 0, 0);
}

/*!
	@fn		SetupTopology
	@brief	�g�|���W�[�̃Z�b�g�A�b�v
*/
void API::SkinMesh::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

/*!
	@fn		SetupInputLayout
	@brief	���_���C�A�E�g�̃Z�b�g�A�b�v
*/
void API::SkinMesh::SetupInputLayout()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(*m_pShader->GetInputLayout());
}

/*!
	@fn		SetupVertexBuffer
	@brief	���_�o�b�t�@�̃Z�b�g�A�b�v
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
	@brief	�C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
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
	@brief		�t�@�C���̓ǂݍ���
	@param[in]	�o�C���h�惁�b�V��
	@param[in]	�t�@�C���̃p�X
	@return		true:���� false:���s
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

	//	�萔�錾
	static constexpr string_view c_Comma = ",";
	static constexpr string_view c_Space = " ";
	static constexpr string_view c_Colon = ":";

	//	���_�C���f�b�N�X
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

	//	���_
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

		//	UV����
		if (uvEmpty) {
			mesh->m_Vertices.push_back({ {x,y,z} ,{ -1,-1 } });
			continue;
		}

		//	UV�L��
		++index;
		mesh->m_Vertices[index].position = { x,y,z };
	}

	//	�d��
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
	@brief	�\�t�g�E�F�A�X�L�j���O
*/
void API::SkinMesh::SoftwareSkinning()
{
	auto&dev = Direct3D11::GetInstance();
	auto&camera = Camera::GetInstance();

	HRESULT hr;

#pragma region �萔�o�b�t�@
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

#pragma region �V�F�[�_�[�̃o�C���h
	dev.GetImmediateContext()->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	dev.GetImmediateContext()->VSSetShader(*m_pShader->GetVertexShader(), 0, 0);
	dev.GetImmediateContext()->PSSetShader(*m_pShader->GetPixelShader(), 0, 0);
#pragma endregion
	
#pragma region ���_�̏�������
	if (!m_pAnimClip.expired()) {
		auto animClip = m_pAnimClip.lock();

		hr = dev.GetImmediateContext()->Map(
			m_pVertexBuffer.Get(),
			0,
			D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
			0,
			&ms
		);

		//	�����s��
		vector<XMMATRIX>compMats;
		compMats.resize(m_Vertices.size());
		//	���_���[�v
		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			//	�W���C���g���[�v
			for (size_t j = 0; j < m_Vertices[i].joints.size(); j++)
			{
				uint32_t jointIndex = m_Vertices[i].joints[j].index;
				float weight = m_Vertices[i].joints[j].weight;
				XMMATRIX skinningMat = animClip.get()->matrixPalette[jointIndex][frameIndex];
				compMats[i] += (skinningMat * weight);
			}
		}
	
		//	�ϊ��㒸�_
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

		//	�������R�s�[
		memcpy_s(ms.pData, ms.RowPitch, (void*)(vertices.data()), sizeof(SkinnedVertex) * m_Vertices.size());
		dev.GetImmediateContext()->Unmap(m_pVertexBuffer.Get(), 0);
	}
#pragma endregion

}

/*!
	@fn		SetupTexture
	@brief	�e�N�X�`�����̃Z�b�g�A�b�v
	@detail	SRV�ƃT���v���[���s�N�Z���V�F�[�_�[�ɃZ�b�g����
*/
void API::SkinMesh::SetupTexture()
{
	auto material = m_pMaterial.lock();
	if (m_pMaterial.expired()) { return; }

	//	�T���v���[
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
	@brief	�C���f�b�N�X�o�b�t�@�̍쐬
	@return	S_OK:���� E_FAIL:���s
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
	@brief	���_�o�b�t�@�̍쐬
	@return	S_OK:���� E_FAIL:���s
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
