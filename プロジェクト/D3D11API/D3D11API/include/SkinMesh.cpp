#include "stdafx.h"
#include "SkinMesh.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "SkinnedVertex.h"
#include "SimpleCompVertex.h"
#include "SimpleCompPixel.h"
#include "MeshConstantBuffer.h"
#include "Camera.h"
#include "SkinnedVertex.h"
#include "MeshReadHelper.h"
#include <algorithm>

using namespace API;
using namespace std;
using namespace D3D11;
using namespace D3D11::Graphic;
using namespace DirectX;

#pragma region �V�F�[�_�[
class Shader
	:public Graphic::AbstractShader
{
public:
	explicit Shader() :AbstractShader() {}
	~Shader() {}
	HRESULT Setup()override final;
	HRESULT DynamicSetup()override { return E_FAIL; }
private:

};
HRESULT Shader::Setup()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT	hr = E_FAIL;
#pragma region ���_���C�A�E�g

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	hr = dev.GetDevice()->CreateInputLayout(
		desc,
		GetArraySize(desc),
		g_vs_main,
		sizeof(g_vs_main),
		m_pVertexLayout.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("input layout"); }
#pragma endregion

#pragma region ���_�V�F�[�_�[
	hr = dev.GetDevice()->CreateVertexShader(
		&g_vs_main,
		sizeof(g_vs_main),
		NULL,
		m_pVertexShader.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("vertex shader"); }
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�[
	hr = dev.GetDevice()->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("pixel shader"); }
#pragma endregion

#pragma region �萔�o�b�t�@
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	hr = dev.GetDevice()->CreateBuffer(
		&bd,
		NULL,
		m_pConstantBuffer.GetAddressOf()
	);
	if (FAILED(hr)) { ErrorLog("constant buffer"); }
#pragma endregion

	return S_OK;
}


#pragma endregion

API::SkinMesh::SkinMesh()
	: m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
{
	transform = make_shared<Transform>();
}

API::SkinMesh::~SkinMesh()
{
}

void API::SkinMesh::Init()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr = E_FAIL;

	m_pShader = make_shared<Shader>();
	m_pShader->Setup();

#pragma region �萔�ǂݍ���
#if 0
#define CONST_READ
#pragma region �C���f�b�N�X�o�b�t�@
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*GetArraySize(c_Indices);
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = NULL;

		//	�T�u���\�[�X�̎d�l	
		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = c_Indices;
		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pIndexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("index buffer"); }
	}
#pragma endregion

#pragma region ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SkinnedVertex) * GetArraySize(c_VerticesPosition);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = NULL;

		//	�T�u���\�[�X�̎d�l
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = c_VerticesPosition;
 
		hr =dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pVertexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("vertex"); }
	}
#pragma endregion
#endif // 0
#pragma endregion

#pragma region �O���t�@�C���ǂݍ���
#if 0
	auto data = Helper::MeshReadHelper::ReadSkin("newSample.yfm");
#pragma region �C���f�b�N�X�o�b�t�@
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*data.indices.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = NULL;

		//	�T�u���\�[�X�̎d�l	
		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = data.indices.data();
		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pIndexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("index buffer"); }
	}
#pragma endregion

#pragma region ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SkinnedVertex) * data.vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = NULL;

		//	�T�u���\�[�X�̎d�l
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data.vertices.data();

		hr = dev.GetDevice()->CreateBuffer(
			&bd,
			&sd,
			m_pVertexBuffer.GetAddressOf()
		);
		if (FAILED(hr)) { ErrorLog("vertex"); }
	}
#pragma endregion

#pragma region �����s��
	m_CompositeMatrix.resize(data.vertices.size());
	//	�t���[����
	for (size_t i = 0; i < data.vertices[0].compMat.size(); i++)
	{
		//	���_��
		for (size_t j = 0; j < data.vertices.size(); j++)
		{
			m_CompositeMatrix[i].push_back(data.vertices[j].compMat[i]);
		}
	}
#pragma endregion
#endif // 0
#pragma endregion

#pragma region �V�t�@�C���ǂݍ���
	if (!Load(this, "newSample.yfm")) {
		ErrorLog("�ǂݍ��ݎ��s");
	}

	//	
	static constexpr uint32_t c_ShaderJointCount = 4;
	m_eSkinningMode = m_Vertices.end() != std::find_if(
		m_Vertices.begin(), m_Vertices.end(),
		[](SkinnedVertex v)-> bool { return v.joints.size() > c_ShaderJointCount; }
	) ? SkinningMode::SOFTWARE : SkinningMode::SHADER;
	if (FAILED(CreateIndexBuffer())) { return; }
	if (FAILED(CreateVertexBuffer())) { 
		ErrorLog("vertex");
	}
#pragma endregion


}

//void API::SkinMesh::Render()
//{
//	HRESULT hr = E_FAIL;
//	auto& dev = Direct3D11::GetInstance();
//
//#pragma region �g�|���W�[
//	dev.GetImmediateContext()->IASetPrimitiveTopology(
//		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
//	);
//#pragma endregion
//
//#pragma region �萔�o�b�t�@
//	auto&camera = Camera::GetInstance();
//	XMMATRIX w, v, p;
//	w = XMMatrixTranspose(transform->GetWorldMatrix());
//	v = XMMatrixTranspose(camera.GetViewMatrix());
//	p = XMMatrixTranspose(camera.GetProjMatrix());
//
//	D3D11_MAPPED_SUBRESOURCE ms;
//	D3D11::Graphic::MeshConstantBuffer cb;
//
//	hr = dev.GetImmediateContext()->Map(
//		*m_pShader->GetConstantBuffer(),
//		NULL,
//		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
//		NULL,
//		&ms
//	);
//	if (FAILED(hr)) { ErrorLog("map"); }
//	cb.m.world = w;
//	cb.m.view = v;
//	cb.m.proj = p;
//	cb.color = { 1,1,1,1 };
//
//	memcpy_s(ms.pData, ms.RowPitch, (void*)(&cb), sizeof(cb));
//	dev.GetImmediateContext()->Unmap(*m_pShader->GetConstantBuffer(), NULL);
//#pragma endregion
//
//#pragma region �V�F�[�_�[
//	dev.GetImmediateContext()->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
//	dev.GetImmediateContext()->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
//	dev.GetImmediateContext()->VSSetShader(*m_pShader->GetVertexShader(), 0, 0);
//	dev.GetImmediateContext()->PSSetShader(*m_pShader->GetPixelShader(), 0, 0);
//#pragma endregion
//
//#pragma region ���_���C�A�E�g
//	dev.GetImmediateContext()->IASetInputLayout(*m_pShader->GetInputLayout());
//#pragma endregion
//
//#pragma region ���_�v�Z
//#if 1
//
//
//	hr = dev.GetImmediateContext()->Map(
//		m_pVertexBuffer.Get(),
//		0,
//		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
//		0,
//		&ms
//	);
//
//	//	���_�錾
//	vector<SkinnedVertex> vs;
//
//#pragma region �X�L�j���O�v�Z
//#if 1
//	vector<XMMATRIX> compMat;
//	compMat.resize(GetArraySize(c_VerticesPosition));
//	for (size_t i = 0; i < GetArraySize(c_VerticesPosition); i++)
//	{
//		//�{�[��
//		for (size_t j = 0; j < GetArraySize(c_Joints[i]); j++)
//		{
//			uint32_t jointIndex = c_Joints[i][j].index;
//			auto skMat = XMLoadFloat4x4(&c_SkinMats[frameIndex][jointIndex]);
//			float w = c_Joints[i][j].weight;
//			compMat[i] += skMat * w;
//		}
//	}
//
//	for (size_t i = 0; i < GetArraySize(c_VerticesPosition); i++)
//	{
//		XMVECTOR vec = {
//		c_VerticesPosition[i].x,
//		c_VerticesPosition[i].y,
//		c_VerticesPosition[i].z,
//		1
//		};
//		vec= XMVector4Transform(vec, compMat[i]);
//		SkinnedVertex v =
//		{
//			{vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2]}
//		};
//		vs.push_back(v);
//	}
//
//#endif // 1
//
//#pragma endregion
//
//
//#pragma region �����s��
//#if 0
//	
////	���_�ҏW
//	for (size_t i = 0; i < GetArraySize(c_CompositeMatrix); i++)
//	{
//		XMVECTOR vec = {
//			c_VerticesPosition[i].x,
//			c_VerticesPosition[i].y,
//			c_VerticesPosition[i].z,
//			1
//		};
//#ifdef CONST_READ
//		XMMATRIX m = XMLoadFloat4x4(&c_CompositeMatrix[i]);
//#else
//		XMMATRIX m = XMLoadFloat4x4(&m_CompositeMatrix[frameIndex][i]);
//#endif // DEBUG
//		
//		vec = XMVector4Transform(vec, m);
//		SkinnedVertex v = 
//		{
//			{vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2]}
//		};
//		vs.push_back(v);
//	}
//#endif // 0
//#pragma endregion
//
//	//	�������R�s�[
//	memcpy_s(ms.pData, ms.RowPitch, (void*)(vs.data()), sizeof(SkinnedVertex) * GetArraySize(c_VerticesPosition));
//
//	dev.GetImmediateContext()->Unmap(m_pVertexBuffer.Get(), 0);
//#endif // 1
//#pragma endregion
//
//#pragma region ���_�o�b�t�@
//	uint32_t stride = sizeof(SkinnedVertex);
//	static constexpr uint32_t vertexBufferOffset = 0;
//	dev.GetImmediateContext()->IASetVertexBuffers(
//		0,
//		1,
//		m_pVertexBuffer.GetAddressOf(),
//		&stride,
//		&vertexBufferOffset
//	);
//#pragma endregion
//
//#pragma region ���_�C���f�b�N�X
//	static constexpr uint32_t indexBufferOffset = 0;
//	dev.GetImmediateContext()->IASetIndexBuffer(
//		m_pIndexBuffer.Get(),
//		DXGI_FORMAT_R32_UINT,
//		indexBufferOffset
//	);
//#pragma endregion
//
//	dev.GetImmediateContext()->DrawIndexed(GetArraySize(c_Indices), 0, 0);
//}

void API::SkinMesh::Render()
{
	SetupTopology();

	switch (m_eSkinningMode)
	{
	case API::SkinMesh::SOFTWARE:SoftwareSkinning(); break;
	case API::SkinMesh::SHADER:
		break;
	default:
		break;
	}

	SetupInputLayout();
	SetupVertexBuffer();
	SetupIndexBuffer();

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
	if (!m_AnimClip.expired()) {
		auto animClip = m_AnimClip.lock();

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
				{ vec.m128_f32[0],vec.m128_f32[1],vec.m128_f32[2] }
			};
			vertices.push_back(v);
		}

		//	�������R�s�[
		memcpy_s(ms.pData, ms.RowPitch, (void*)(vertices.data()), sizeof(SkinnedVertex) * GetArraySize(c_VerticesPosition));
		dev.GetImmediateContext()->Unmap(m_pVertexBuffer.Get(), 0);
	}
#pragma endregion

#pragma region 


#pragma endregion
}

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
