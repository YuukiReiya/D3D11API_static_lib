#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
//struct alignas(16) Vertex
//{
//	DirectX::XMFLOAT3 pos;
//	//float col[4];
//};
//
//struct CBuffer
//{
//	DirectX::XMMATRIX world;
//	DirectX::XMMATRIX view;
//	DirectX::XMMATRIX proj;
//	DirectX::XMFLOAT4 color;
//};

class Hoge
{
public:
	Hoge() {};
	~Hoge() {};

	void Init();
	void Update();
	void Draw();
private:
	/*std::vector<Vertex>mV;
	std::vector<UINT>mI;*/
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	ID3D11InputLayout* il;
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11Buffer* pcb;
	ID3D11SamplerState* samp;
	ID3D11ShaderResourceView*srv;
	//std::vector<UINT>m_Index;
	//std::vector<Vertex>m_Vertex;
	uint32_t indexCount;
};


