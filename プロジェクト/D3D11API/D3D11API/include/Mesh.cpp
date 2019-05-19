#include "stdafx.h"
#include "Mesh.h"
#include "MeshVertex.h"
#include "Direct3D11.h"
#include "MyGame.h"
#include "MemoryLeaks.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include "WICTextureLoader.h"
#include "MeshConstantBuffer.h"
#include "MeshReadHelper.h"
#include <vector>
#include "Camera.h"

using namespace DirectX;
using namespace D3D11;
using namespace API;
using namespace D3D11::Graphic;
using namespace std;

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

HRESULT Mesh::Initialize()
{
	//if (FAILED(CreateInputLayout(this))) {
	//	ErrorLog("inputlayout");
	//	exit(0);
	//}

	//if (FAILED(CreateVertexBuffer(this))) {
	//	ErrorLog("vertexbuffer");
	//	exit(0);
	//}

	//if (FAILED(CreateIndexBuffer(this))) {
	//	ErrorLog("indexbuffer");
	//	exit(0);
	//}
	//Direct3D11::GetInstance().GetImmediateContext()->IASetIndexBuffer(
	//	m_pIndexBuffer.Get(),
	//	DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
	//	0
	//);

	//if (FAILED(CreateVertexShader())) {
	//	ErrorLog("vshader");
	//	exit(0);
	//}

	//if (FAILED(CreatePixelShader())) {
	//	ErrorLog("pshader");
	//	exit(0);
	//}

	//if (FAILED(CreateConstantBuffer(this))) {
	//	ErrorLog("constant buffer");
	//	exit(0);
	//}

	//===================================
	//	�f�o�C�X
	auto& dev = D3D11::Direct3D11::GetInstance();

	//	���_���C�A�E�g
	{
		D3D11_INPUT_ELEMENT_DESC vd[]
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		HRESULT hr = D3D11::Direct3D11::GetInstance().Device->CreateInputLayout(
			vd,
			GetArraySize(vd),
			g_vs_main,
			sizeof(g_vs_main),
			m_pInputLayout.GetAddressOf()
		);
		if (FAILED(
			hr
		)) {
			ErrorLog("layout �쐬���s");
		}
	}

	//	���_�V�F�[�_�[
	{
		if (FAILED(
			D3D11::Direct3D11::GetInstance().Device->CreateVertexShader(
				&g_vs_main,
				sizeof(g_vs_main),
				NULL,
				m_pVertexShader.GetAddressOf()
			)
		)) {
			ErrorLog("vs�쐬���s");
		}
	}

	//	�s�N�Z���V�F�[�_�[
	{
		if (FAILED(
			D3D11::Direct3D11::GetInstance().Device->CreatePixelShader(
				&g_ps_main,
				sizeof(g_ps_main),
				NULL,
				m_pPixelShader.GetAddressOf()
			)
		)) {
			ErrorLog("ps�쐬���s");
		}
	}

	auto rd = Helper::MeshReadHelper::Read("test.yfm");


	//	�R���X�^���g�o�b�t�@
	{
		HRESULT hr;
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
			&cb,
			0,
			m_pConstantBuffer.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("CBUFFER");
			return E_FAIL;
		}
	}

	vector<MeshVertex>vertex;
	//	���_�o�b�t�@
	{
		vertex.clear();
		for (auto it : rd.vertices) {
			vertex.push_back({ it.position });
		}

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MeshVertex) * vertex.size();// * �v�f��
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertex.data();

		if (FAILED(dev.Device->CreateBuffer(&bd, &sd, m_pVertexBuffer.GetAddressOf()))) {
			ErrorLog("v�o�b�t�@�쐬���s");
		}
	}

	std::vector<uint32_t>index;
	//	�C���f�b�N�X�o�b�t�@
	{
		index = rd.indices;

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*index.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index.data();

		if (FAILED(dev.Device->CreateBuffer(
			&bd, &sd, m_pIndexBuffer.GetAddressOf()
		))) {
			ErrorLog("i�o�b�t�@�쐬���s");
		}
		dev.ImmediateContext->IASetIndexBuffer(
			m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
		);
	}
	indexCount = index.size();

	//	SRV
	{
		//	�e�N�X�`���ǂݍ���
		std::wstring path;
		path = L"hoge.png";

		Microsoft::WRL::ComPtr<ID3D11Resource> pResource = nullptr;
		HRESULT hr;
		hr = CreateWICTextureFromFile(
			Direct3D11::GetInstance().GetDevice(),
			path.c_str(),
			pResource.GetAddressOf(),
			m_pSRV.GetAddressOf()
		);

		// ���[�J���ϐ��̃������J��
		if (pResource.Get() != nullptr) {
			pResource.Reset();
		}
		if (FAILED(hr)) {
			ErrorLog("srv");
		};
	}

	//	�T���v���[
	{
		D3D11_SAMPLER_DESC sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		HRESULT hr;
		hr = Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
			&sd,
			m_pSamplerState.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("sampler");
		}
	}



	return S_OK;
}


void Mesh::Render()
{
	HRESULT hr;
	XMMATRIX w, v, p;
	//	world
	{
		//w = XMMatrixTranslation(0, 0, 0);
		w = transform.GetMatrix();
		w = XMMatrixTranspose(w);
	}

	//	view
	{
		XMVECTOR eye, look, upvec;
		//eye = { gEyePt.x,gEyePt.y,gEyePt.z };
		eye = { 0,0,-10 };
		look = { 0,0,0 };
		//look = { gLookPt.x,gLookPt.y,gLookPt.z };
		upvec = { 0,1,0 };
		//v = XMMatrixLookAtLH(eye, look, upvec);
		v = Camera::GetInstance().GetViewMatrix();
		v = XMMatrixTranspose(v);
	}

	//	proj
	{
		float pi = 3.14159265358979323846f;
		p = XMMatrixPerspectiveFovLH(pi / 4, 1920 / 1080, 0.1f, 100.0f);
		p = Camera::GetInstance().GetProjMatrix();
		p = XMMatrixTranspose(p);
	}

	//	�V�F�[�_�[�Z�b�g
	{
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(m_pVertexShader.Get(), 0, 0);
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(m_pPixelShader.Get(), 0, 0);
	}

	//	�R���X�^���g�o�b�t�@���M
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		MeshConstantBuffer cb;
		hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
			m_pConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0, 
			&pData
		);
		if (FAILED(hr)) {
			ErrorLog("map");
			exit(0);
		}
		//pcb.m_WorldMatrix = w;
		//pcb.m_ViewMatrix = v;
		//pcb.m_ProjectionMatrix = p;
		cb.m.world = w;
		cb.m.view = v;
		cb.m.proj = p;
		//pcb.m.world = w;
		//pcb.m.view = v;
		//pcb.m.proj = p;
		cb.color = { 1,1,1,1 };

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(m_pConstantBuffer.Get(), 0);
	}

	//	CBuffer���g���V�F�[�_�[�̃o�C���h
	{
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	}

	//	���_�o�b�t�@�Z�b�g
	{
		UINT stride = sizeof(MeshVertex);
		UINT offset = 0;
		Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
			0,
			1,
			m_pVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);
	}

	//	���_���C�A�E�g�Z�b�g
	{
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(m_pInputLayout.Get());
	}

	//	�g�|���W�[
	{
		//	���_����H(�|���S����)
		//Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//	�\���b�h(�������͑��v���ۂ�)
		//Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
		//	�Y��Ƀ|���S���\���o���Ă�����ۂ�
		Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		m_pSamplerState.GetAddressOf()
	);

	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		m_pSRV.GetAddressOf()
	);

	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(indexCount, 0, 0);

}

HRESULT API::Mesh::CreateInputLayout(Mesh * mesh)
{
	D3D11_INPUT_ELEMENT_DESC vd[]
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	return Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
		vd,
		GetArraySize(vd),
		g_vs_main,
		sizeof(g_vs_main),
		mesh->m_pInputLayout.GetAddressOf()
	);
}

HRESULT API::Mesh::CreateVertexBuffer(Mesh * mesh)
{
	return E_FAIL;
}

HRESULT API::Mesh::CreateIndexBuffer(Mesh * mesh)
{
	return E_FAIL;
}

HRESULT API::Mesh::CreateConstantBuffer(Mesh * mesh)
{
	return E_FAIL;
}

HRESULT API::Mesh::CreateVertexShader()
{
	return E_FAIL;
}

HRESULT API::Mesh::CreatePixelShader()
{
	return E_FAIL;
}
