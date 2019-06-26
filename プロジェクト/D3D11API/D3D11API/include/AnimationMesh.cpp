#include "stdafx.h"
#include "..\AnimationMesh.h"
#include "Direct3D11.h"
#include "Camera.h"
#include "MyGame.h"
#include "MeshReadHelper.h"


#pragma region シェーダー
#include <d3dcompiler.h>
#include "MeshConstantBuffer.h"

using namespace D3D11::Graphic;
using namespace D3D11;
class AnimMeshShader
	:public AbstractShader
{
public:
	AnimMeshShader();
	~AnimMeshShader();

	HRESULT Setup()override final { return E_FAIL; }
	HRESULT DynamicSetup()override final;

private:

};

AnimMeshShader::AnimMeshShader()
	:AbstractShader()
{
}

AnimMeshShader::~AnimMeshShader()
{
}

HRESULT AnimMeshShader::DynamicSetup()
{
	auto&dev = Direct3D11::GetInstance();
	HRESULT hr;

	//	パス
	auto path = L"shader.hlsl";
	
	//	シェーダー生成
	ID3DBlob *pCompileVS = NULL;
	ID3DBlob *pCompilePS = NULL;
	hr = D3DCompileFromFile(path, 0, 0, "VS", "vs_5_0", 0, 0, &pCompileVS, 0);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = dev.GetDevice()->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), 0, m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = D3DCompileFromFile(path, 0, 0, "PS", "ps_5_0", 0, 0, &pCompilePS, 0);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	hr = dev.GetDevice()->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), 0, m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	//	頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = dev.GetDevice()->CreateInputLayout(layout, GetArraySize(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), m_pVertexLayout.GetAddressOf());
	if (FAILED(hr)) {
		pCompileVS->Release();
		pCompilePS->Release();
		return E_FAIL;
	}
	pCompileVS->Release();
	pCompilePS->Release();

	//	コンスタントバッファ
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(D3D11::Graphic::MeshConstantBuffer);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	hr = dev.GetDevice()->CreateBuffer(&cb, 0, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
	return S_OK;
}


#pragma endregion


HRESULT API::AnimationMesh::Init(std::string path)
{
	m_pShader = std::make_shared<AnimMeshShader>();
	if (FAILED(m_pShader->DynamicSetup())) {
		return E_FAIL;
	}

	HRESULT hr;

	auto device = Direct3D11::GetInstance().GetDevice();
	auto context = Direct3D11::GetInstance().GetImmediateContext();

	auto mesh = Helper::MeshReadHelper::ReadAnim(path);
	m_IndexCount = mesh.indices.size();
	m_VertexCount = mesh.vertices[0][0].size();

	for (int i = 0; i < mesh.animCount; ++i) {
		for (int j = 0; j < mesh.frameCount[i]; ++j) {
			m_VertexList[i][j] = mesh.vertices[i][j];
		}
	}
	
#pragma region 頂点生成
	D3D11_BUFFER_DESC vBD;
	vBD.ByteWidth = sizeof(VERTEX) * m_VertexCount;
	vBD.Usage = D3D11_USAGE_DYNAMIC;
	vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBD.MiscFlags = 0;
	vBD.StructureByteStride = 0;
	//	サブリソースの仕様
	D3D11_SUBRESOURCE_DATA sd;
	//sd.pSysMem = mesh.vertices[0].v[0].data();

	sd.pSysMem = m_VertexList[0][0].data();
	//hr = device->CreateBuffer(&vBD, &sd, m_pVertexBuffer.GetAddressOf());
	hr = device->CreateBuffer(&vBD, 0, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
#pragma endregion

#pragma region 頂点インデックス
	D3D11_BUFFER_DESC iBD;
	iBD.ByteWidth = sizeof(m_IndexCount) * m_IndexCount;
	iBD.Usage = D3D11_USAGE_DEFAULT;
	iBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBD.CPUAccessFlags = 0;
	iBD.MiscFlags = 0;
	iBD.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = mesh.indices.data();//頂点インデックス配列
	hr = device->CreateBuffer(&iBD, &iData, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		return E_FAIL;
	}
#pragma endregion

#pragma region 設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(*m_pShader->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	context->PSSetConstantBuffers(0, 1, m_pShader->GetConstantBuffer());
	context->VSSetShader(*m_pShader->GetVertexShader(), NULL, 0);
	context->PSSetShader(*m_pShader->GetPixelShader(), NULL, 0);

#pragma endregion
	return S_OK;
}

void API::AnimationMesh::Render()
{
	auto&camera = API::Camera::GetInstance();
	auto context = Direct3D11::GetInstance().GetImmediateContext();

	D3D11_MAPPED_SUBRESOURCE pdata;
	MeshConstantBuffer cb;
	//	コンスタントバッファの受け渡し
	cb.m.world = transform.GetTranslationMatrix();
	cb.m.view = camera.GetViewMatrix();
	cb.m.proj = camera.GetProjMatrix();
	cb.color = { 0,0,0,0 };
	context->Map(*m_pShader->GetConstantBuffer(),0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	context->Unmap(*m_pShader->GetConstantBuffer(), 0);

	//	頂点の受け渡し(斜めに線)
	//頂点バッファの書き換えに成功していれば斜めに赤い線が描画されているはず
	//DirectX::XMFLOAT3 vs[] = {
	//{  0.1, 0.1,0 }  ,	//右上
	//{ -0.1, 0.1,0 } ,	//左上
	//{  0.1,-0.1,0 } ,	//右下
	//{ -0.1,-0.1,0 },	//左下
	//};
	//std::vector<Graphic::MeshVertex>hoge;
	//for (auto it : vs) {
	//	hoge.push_back(it);
	//}
	//context->Map(m_pVertexBuffer.Get(),0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(hoge.data()), sizeof(VERTEX) * m_VertexCount);
	//context->Unmap(m_pVertexBuffer.Get(), 0);

	//	描画
	context->DrawIndexed(m_IndexCount, 0, 0);
}

