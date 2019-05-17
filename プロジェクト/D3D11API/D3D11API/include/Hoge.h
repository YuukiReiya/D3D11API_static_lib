#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
struct Vertex
{
	float pos[3];
	float col[4];
};

struct CBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4 color;
};

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
	UINT indexCount;
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	ID3D11InputLayout* il;
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11Buffer* cb;
	ID3D11SamplerState* samp;
	ID3D11ShaderResourceView*srv;
};


