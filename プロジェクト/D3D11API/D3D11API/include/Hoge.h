#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <wrl/client.h>
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
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pcb;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samp;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>srv;
	//std::vector<UINT>m_Index;
	//std::vector<Vertex>m_Vertex;
	uint32_t indexCount;
};


