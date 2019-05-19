#include "stdafx.h"
#include "Hoge.h"
#include "MyGame.h"
#include "Direct3D11.h"
#include <d3dcompiler.h>
#include "MeshCompVS.h"
#include "MeshCompPS.h"
#include "WICTextureLoader.h"
#include "Keyboard.h"
#include "MeshReadHelper.h"

//
#include "MeshConstantBuffer.h"
#include "MeshVertex.h"
//

using namespace D3D11;
using namespace DirectX;
using namespace std;

void Hoge::Init()
{
	//	デバイス
	auto& dev = D3D11::Direct3D11::GetInstance();

	//	頂点レイアウト
	{
		D3D11_INPUT_ELEMENT_DESC vd[]
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		HRESULT hr = D3D11::Direct3D11::GetInstance().Device->CreateInputLayout(
			vd,
			ARRAYSIZE(vd),
			//GetArraySize(vd),
			g_vs_main,
			sizeof(g_vs_main),
			&il
		);
		if (FAILED(
			hr
		)) {
			ErrorLog("layout 作成失敗");
		}
	}

	//	頂点シェーダー
	{
		if (FAILED(
			D3D11::Direct3D11::GetInstance().Device->CreateVertexShader(
				&g_vs_main,
				sizeof(g_vs_main),
				NULL,
				&vs
			)
		)) {
			ErrorLog("vs作成失敗");
		}
	}

	//	ピクセルシェーダー
	{
		if (FAILED(
			D3D11::Direct3D11::GetInstance().Device->CreatePixelShader(
				&g_ps_main,
				sizeof(g_ps_main),
				NULL,
				&ps
			)
		)) {
			ErrorLog("ps作成失敗");
		}
	}


	//	コンスタントバッファ
	{
		HRESULT hr;
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//cb.ByteWidth = sizeof(CBuffer);
		cb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		hr =Direct3D11::GetInstance().GetDevice()->CreateBuffer(
			&cb,
			0,
			pcb.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("CBUFFER");
			return;
		}
	}

	auto rd = Helper::MeshReadHelper::Read("test.yfm");

	//std::vector<Vertex>m_Vertex;
	std::vector<D3D11::Graphic::MeshVertex>m_Vertex;
	//	頂点
	{
		//reset
		m_Vertex.clear();
		int i = 0;
		while (i < rd.vertices.size())
		{
			//mV.push_back({ { rd.vertices[i].position } });
			m_Vertex.push_back({ rd.vertices[i].position });
			//vertex.push_back({ { read.vertices[i].pos }, { 1,0,0,1 } });
			i++;
		}
	}

	//	頂点バッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(Vertex) * m_Vertex.size();// * 要素数
		bd.ByteWidth = sizeof(D3D11::Graphic::MeshVertex) * m_Vertex.size();// * 要素数
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = m_Vertex.data();

		if (FAILED(dev.Device->CreateBuffer(&bd, &sd, vb.GetAddressOf()))) {
			ErrorLog("vバッファ作成失敗");
		}
	}

	std::vector<uint32_t>m_Index;
	//	インデックスバッファ
	{
		m_Index = rd.indices;

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t)*m_Index.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index.data();

		if (FAILED(dev.Device->CreateBuffer(
			&bd, &sd, &ib
		))) {
			ErrorLog("iバッファ作成失敗");
		}
		dev.ImmediateContext->IASetIndexBuffer(
			ib.Get(), DXGI_FORMAT_R32_UINT, 0
		);
	}
	indexCount = m_Index.size();
	//	SRV
	{
		//	テクスチャ読み込み
		std::wstring path;
		path = L"hoge.png";

		Microsoft::WRL::ComPtr<ID3D11Resource> pResource = nullptr;
		HRESULT hr;
		hr = CreateWICTextureFromFile(
			Direct3D11::GetInstance().GetDevice(),
			path.c_str(),
			pResource.GetAddressOf(),
			&srv
		);

		// ローカル変数のメモリ開放
		if (pResource.Get() != nullptr) {
			pResource.Reset();
		}
		if (FAILED(hr)) {
			ErrorLog("srv"); 
		};
	}
	
	//	サンプラー
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
			&samp
		);
		if (FAILED(hr)) {
			ErrorLog("sampler");
		}
	}

}

void Hoge::Update()
{
}

XMFLOAT3 gEyePt{ 0,0,-10 }, gLookPt{ 0,0,0 };

void Hoge::Draw()
{
	HRESULT hr;
	XMMATRIX w, v, p;
	//	world
	{
		w = XMMatrixTranslation(0, 0, 0);
		w = XMMatrixTranspose(w);
	}

	//	view
	{
		XMVECTOR eye, look, upvec;
		eye = { gEyePt.x,gEyePt.y,gEyePt.z };
		//look = { 0,0,0 };
		look = { gLookPt.x,gLookPt.y,gLookPt.z };
		upvec = { 0,1,0 };
		v = XMMatrixLookAtLH(eye, look, upvec);
		v = XMMatrixTranspose(v);
	}

	//	proj
	{
		float pi = 3.14159265358979323846f;
		p = XMMatrixPerspectiveFovLH(pi / 4, 1920 / 1080, 0.1f, 100.0f);
		p = XMMatrixTranspose(p);
	}

	//	シェーダーセット
	{
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(vs.Get(), 0, 0);
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(ps.Get(), 0, 0);
	}

	//	コンスタントバッファ送信
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		//CBuffer cb_desc;
		D3D11::Graphic::MeshConstantBuffer cb_desc;
		hr = Direct3D11::GetInstance().GetImmediateContext()->Map(pcb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (FAILED(hr)) {
			ErrorLog("map");
			exit(0);
		}
		//cb_desc.world = w;
		//cb_desc.view = v;
		//cb_desc.proj = p;
		cb_desc.m.world = w;
		cb_desc.m.view= v;
		cb_desc.m.proj = p;
		cb_desc.color = { 1,1,1,1 };

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb_desc), sizeof(cb_desc));
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(pcb.Get(), 0);
	}

	//	CBufferを使うシェーダーのバインド
	{
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(0, 1, pcb.GetAddressOf());
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(0, 1, pcb.GetAddressOf());
	}

	//	頂点バッファセット
	{
		//UINT stride = sizeof(Vertex);
		UINT stride = sizeof(D3D11::Graphic::MeshVertex);
		UINT offset = 0;
		Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
	}

	//	頂点レイアウトセット
	{
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(il.Get());
	}

	//	トポロジー
	{
			//	頂点壊れる？(ポリゴン変)
			Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//	ソリッド(見かけは大丈夫っぽい)
			Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
			//	綺麗にポリゴン表示出来ているっぽい
			Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

#ifdef NEW_S
#endif // NEW_S
	//	サンプラー
	//if (pSamp != nullptr) {
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0, 1, samp.GetAddressOf()
	);
	//}

	//	SRV
	//if (pSRV != nullptr) {
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0, 1, srv.GetAddressOf()
	);
	//}

	//Direct3D11::GetInstance().GetImmediateContext()->Draw(3, 0);
	//Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(mI.size(), 0, 0);
	//Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(m_Index.size(), 0, 0);
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(indexCount, 0, 0);
}

