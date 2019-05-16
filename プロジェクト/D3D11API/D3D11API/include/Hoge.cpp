#include "stdafx.h"
#include "Hoge.h"
#include "MyGame.h"
#include "Direct3D11.h"
#include <d3dcompiler.h>
#include "AbstractShader.h"
#include "MeshCompVS.h"
#include "MeshCompPS.h"

using namespace D3D11;
using namespace DirectX;

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

	//	頂点
	{
		//右上
		mV.push_back({ { 0.5f, 0.5f,0},{1,0,0,1} });
		//右下
		mV.push_back({ { 0.5f, -0.5f,0},{1,0,0,1} });
		//左下
		mV.push_back({ {-0.5f, -0.5f,0},{1,0,0,1} });
		//左上
		mV.push_back({ {-0.5f, 0.5f,0},{1,0,0,1} });

		mV.push_back({ {-0.0f, 1.0f,0},{1,0,0,1} });
	}

	//	コンスタントバッファ
	{
		HRESULT hr;
		D3D11_BUFFER_DESC cdesc;
		cdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cdesc.ByteWidth = sizeof(CBuffer);
		cdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cdesc.MiscFlags = 0;
		cdesc.StructureByteStride = 0;
		cdesc.Usage = D3D11_USAGE_DYNAMIC;
		hr =Direct3D11::GetInstance().GetDevice()->CreateBuffer(
			&cdesc,
			0,
			&cb
		);
		if (FAILED(hr)) {
			ErrorLog("CBUFFER");
			return;
		}
	}

	//	頂点バッファ
	{
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(Vertex) * mV.size();// * 要素数
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = mV.data();
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		if (FAILED(dev.Device->CreateBuffer(&bd, &sd, &vb))) {
			ErrorLog("vバッファ作成失敗");
		}
	}

	//	インデックスバッファ
	{
		mI.push_back(0);
		mI.push_back(1);
		mI.push_back(2);
		mI.push_back(3);
		mI.push_back(0);

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int)*mI.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = mI.data();

		if (FAILED(dev.Device->CreateBuffer(
			&bd, &sd, &ib
		))) {
			ErrorLog("iバッファ作成失敗");
		}
		dev.ImmediateContext->IASetIndexBuffer(
			ib, DXGI_FORMAT_R32_UINT,0
		);
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
		Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(vs, 0, 0);
		Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(ps, 0, 0);
	}

	//	コンスタントバッファ送信
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		CBuffer cb_desc;
		hr = Direct3D11::GetInstance().GetImmediateContext()->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (FAILED(hr)) {
			ErrorLog("map");
			exit(0);
		}
		cb_desc.world = w;
		cb_desc.view = v;
		cb_desc.proj = p;
		cb_desc.color = { 1,1,1,1 };

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb_desc), sizeof(cb_desc));
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(cb, 0);
	}

	//	CBufferを使うシェーダーのバインド
	{
		Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(0, 1, &cb);
		Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(0, 1, &cb);
	}

	//	頂点バッファセット
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	}

	//	頂点レイアウトセット
	{
		Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(il);
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
		0, 1, &samp
	);
	//}

	//	SRV
	//if (pSRV != nullptr) {
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0, 1, &srv
	);
	//}

	//Direct3D11::GetInstance().GetImmediateContext()->Draw(3, 0);
	Direct3D11::GetInstance().GetImmediateContext()->DrawIndexed(mI.size(), 0, 0);
}

