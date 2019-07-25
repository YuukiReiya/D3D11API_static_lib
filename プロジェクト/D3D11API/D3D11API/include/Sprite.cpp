/*
	@file	Sprite.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	スプライト
	@detail	参考:http://zerogram.info/?p=2029
*/
#include "stdafx.h"
#include <D3D11.h>
#include "Sprite.h"
#include "Direct3D11.h"
#include "Camera.h"
//#include "ShaderManager.h"
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "Debug.h"
#include "SpriteConstantBuffer.h"
#include "SpriteCompVS.h"
#include "SpriteCompPS.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace D3D11;
//using namespace D3D11::Graphic;
using namespace API;

#if 0
///*!
//	@brief	コンストラクタ
//*/
//Sprite::Sprite()
//{
//	m_pVertexBuffer		= nullptr;
//	m_pBlendState		= nullptr;
//	//m_Size = { -1,-1 };
//	m_StencilMask = 0xffffffff;
//	transform = std::make_shared<Transform>();
//	SetupBlendPreset(BlendPreset::Default);
//}
//
///*!
//	@brief	デストラクタ
//*/
//Sprite::~Sprite()
//{
//	Finalize();
//}
//
///*!
//	@fn		イニシャライズ
//	@brief	初期化
//	@detail	ブレンドステートの作成を行う
//*/
//HRESULT Sprite::Initialize()
//{
//#pragma region OLD
//	//	頂点
//	//if (FAILED(CreateVertexBuffer())) {
//	//	ErrorLog("vertex buffer");
//	//}
//
//	//	頂点インデックス
//	//if (FAILED(CreateIndexBuffer())) {
//	//	ErrorLog("index buffer");
//	//}
//#pragma endregion
//
//	float x, y;
//	x = 0.5f * 10;
//	y = 0.5f * 10;
//
//	//	頂点
//	Vertex vertices[] =
//	{
//		//	右上
//		{DirectX::XMFLOAT3( x,  y,0), DirectX::XMFLOAT2(1,0)},
//		//	右下
//		{DirectX::XMFLOAT3( x, -y,0), DirectX::XMFLOAT2(1,1)},
//		//	左下
//		{DirectX::XMFLOAT3(-x,  y,0), DirectX::XMFLOAT2(0,1)},
//		//	左上
//		{DirectX::XMFLOAT3(-x, -y,0), DirectX::XMFLOAT2(0,0)},
//	};
//	
//	//	バッファ
//	D3D11_BUFFER_DESC desc;
//	SecureZeroMemory(&desc,sizeof(desc));
//	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(vertices);
//	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
//
//	//	サブリソース
//	D3D11_SUBRESOURCE_DATA data;
//	SecureZeroMemory(&data, sizeof(data));
//	data.pSysMem = vertices;
//
//	//頂点バッファのリセット
//	m_pVertexBuffer.Reset();
//
//	HRESULT hr;
//	auto device = Direct3D11::GetInstance().GetDevice();
//	auto context = Direct3D11::GetInstance().GetImmediateContext();
//
//	//	頂点生成
//	hr = device->CreateBuffer(&desc, &data, m_pVertexBuffer.GetAddressOf());
//	if (FAILED(hr)) { ErrorLog("failed to vertex generation"); }
//	uint32_t stride = sizeof(Vertex);
//	uint32_t offset = 0;
//	context->IAGetVertexBuffers(
//		0, 1, m_pVertexBuffer.GetAddressOf(),
//		&stride, &offset
//	);
//
//
//	return S_OK;
//}
//
///*!
//	@fn		ファイナライズ
//	@brief	破棄処理
//	@detail	メンバの明示的な解放とメンバの初期化
//*/
//void Sprite::Finalize()
//{
//	// メンバの初期化
//	//m_Size = { -1,-1 };
//	m_StencilMask = 0xffffffff;
//
//	// 開放
//	m_pVertexBuffer.Reset();
//	m_pBlendState.Reset();
//}
//
///*!
//	@fn		Render
//	@brief	描画
//*/
//void API::Sprite::Render()
//{
//#if defined DEBUG_SPRITE
//
//	//	シェーダーの参照切れ
//	if (m_pShader.expired()) {
//		ErrorLog("The \"m_pShader\" value referenced in this class is invalid!");
//		return;
//	}
//
//	//	テクスチャの参照切れ
//	if (m_pTexture.expired()) {
//		ErrorLog("The \"m_pTexture\" value referenced in this class is invalid!");
//		return;
//	}
//#endif
//
//	//	トポロジーのセット
//	SetupTopology();
//
//	//	頂点レイアウトのセット
//	SetupInputLayout();
//
//	//	シェーダーのセット
//	SetupBindShader();
//
//	//	サンプラーステートのセット
//	SetupSampler();
//
//	//	ShaderResourceViewのセット
//	SetupSRV();
//
//	//	コンスタントバッファのセット
//	SetupConstantBuffer();
//
//	//	ブレンドステートのセット
//	SetupBlendState();
//
//	//	描画
//	Direct3D11::GetInstance().GetImmediateContext()->Draw(
//		4,		// 頂点数(板ポリゴンなので頂点数は4つ)
//		NULL
//	);
//}
//
///*!
//	@fn			SetupBlendPreset
//	@brief		指定したプリセットのブレンドステートをメンバに設定する
//	@param[in]	指定するプリセットの列挙体
//*/
//void API::Sprite::SetupBlendPreset(BlendPreset preset)
//{
//	/*!
//	※
//	SRC:ソース(これから描画するピクセルの色)
//	DEST:ディストネーション(レンダリングターゲットに描画されているピクセルの色)
//
//	最終的な描画色は以下の「混合関数」によって決まる
//
//		SRC × ブレンディング係数 ＋ DEST × ブレンディング係数
//
//		SRCALPHA:	 SRC のα値
//		INVSRCALPHA: 1 - SRC のα値
//		DESTALPHA:	 DESTのα値
//
//
//		Cr = Cd * (1 - As) + Cs * As
//
//		   = Cs * As + Cd * (1 - As)
//	*/
//
//	//	メモリを解放し、再設定する
//	if (m_pBlendState != nullptr) {
//		m_pBlendState.Reset();
//	}
//
//	// αブレンド
//	// αテスト設定
//	D3D11_BLEND_DESC bd;
//	SecureZeroMemory(&bd, sizeof(bd));
//
//	// ブレンドの有効・無効
//	bd.RenderTarget[0].BlendEnable = preset != BlendPreset::Default;
//
//	//	ブレンディング係数
//	D3D11_BLEND src, dest, srcAlpha, destAlpha;
//
//	//	ブレンディングオプション
//	D3D11_BLEND_OP blendOp, blendOpAlpha;
//
//	//	プリセットで分岐
//	switch (preset)
//	{
//		//	デフォルト
//		case API::Sprite::BlendPreset::Default:
//		{
//			//	係数
//			src			= D3D11_BLEND::D3D11_BLEND_ONE;
//			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
//			dest		= D3D11_BLEND::D3D11_BLEND_ZERO;
//			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
//
//			//	オプション
//			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//		}
//		break;
//
//		//	線形
//		case API::Sprite::BlendPreset::Linear:
//		{
//			//	係数
//			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
//			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
//			dest		= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
//			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
//
//			//	オプション
//			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//		}
//			break;
//
//		//	加算
//		case API::Sprite::BlendPreset::Add:
//		{
//			//	係数
//			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
//			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
//			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
//			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
//
//			//	オプション
//			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//		}
//			break;
//
//		//	減算
//		case API::Sprite::BlendPreset::Subtraction:
//		{
//			//	係数
//			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
//			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
//			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
//			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
//
//			//	オプション
//			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
//			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//		}
//			break;
//
//		//	乗算
//		case API::Sprite::BlendPreset::Multiple:
//		{
//			//	係数
//			src			= D3D11_BLEND::D3D11_BLEND_ZERO;
//			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
//			dest		= D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
//			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
//
//			//	オプション
//			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//		}
//			break;
//
//		//	例外処理
//		default:
//			std::string error = "Invalid value for argument of SetupBlendPreset function!";
//			ErrorLog(error);
//			return;
//	}
//	
//	//	係数
//	bd.RenderTarget[0].SrcBlend			= src;
//	bd.RenderTarget[0].DestBlend		= dest;
//	bd.RenderTarget[0].SrcBlendAlpha	= srcAlpha;
//	bd.RenderTarget[0].DestBlendAlpha	= destAlpha;
//
//	// ブレンドオプション
//	bd.RenderTarget[0].BlendOp		= blendOp;
//	bd.RenderTarget[0].BlendOpAlpha = blendOpAlpha;
//
//	//	RGBA 全てをブレンドする
//	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
//
//	// アンチエイリアス処理
//	bd.IndependentBlendEnable = false;
//
//	HRESULT hr;
//
//	// ブレンドステートの作成
//	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
//		&bd,
//		m_pBlendState.GetAddressOf()
//	);
//	if (FAILED(hr)) {
//		std::string error = "BlendState is not create!";
//		ErrorLog(error);
//		return;
//	}
//}
//
///*!
//	@fn			SetupTexture
//	@brief		テクスチャの設定
//	@detail		弱参照でバインドし、この時点で頂点生成を行う
//	@param[in]	登録するテクスチャのポインタ
//*/
//void API::Sprite::SetupTexture(Texture*  texture)
//{
//	m_pTexture = texture->GetSharedPtr();
//}
//
///*!
//	@fn			SetupShader
//	@brief		シェーダーの設定
//	@detail		弱参照でバインドする
//	@param[in]	登録するシェーダーのポインタ
//*/
//void API::Sprite::SetupShader(D3D11::Graphic::AbstractShader * shader)
//{
//	m_pShader = shader->GetSharedPtr();
//}
//
///*!
//	@fn			頂点生成
//	@brief		スプライトの頂点を生成する
//	@detail		頂点構造体を使って、情報を頂点バッファにバインド
//	@param[in]	画像サイズ
//	@return		成功:S_OK 失敗:E_FAIL
//*/
////HRESULT Sprite::CreateVertex(DirectX::XMINT2 size)
////{
////	// 頂点宣言
////	DirectX::XMFLOAT2 leftTop, rightBottom;			// 頂点座標
////	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;		// UV座標
////
////	// 各頂点定義
////	leftTop.x		= -0.5f*size.x / c_DefaultSize;// 左
////	rightBottom.x	=  0.5f*size.x / c_DefaultSize;// 右
////	leftTop.y		=  0.5f*size.y / c_DefaultSize;// 上
////	rightBottom.y	= -0.5f*size.y / c_DefaultSize;// 下
////
////	// UV定義
////	uvLeftTop.x = uvLeftTop.y = 0;
////	uvRightBottom.x = uvRightBottom.y = 1;
////
////	// 頂点構造体定義
////	SpriteVertex vertices[] = {
////		// 右上
////		{
////			// 頂点
////			DirectX::XMFLOAT3(
////				rightBottom.x,
////				leftTop.y,
////				c_VertexZ
////			),
////			// UV座標
////			DirectX::XMFLOAT2(
////				uvRightBottom.x,
////				uvLeftTop.y
////			),
////	},
////		// 右下
////		{
////			// 頂点
////			DirectX::XMFLOAT3(
////				rightBottom.x,
////				rightBottom.y,
////				c_VertexZ
////			),
////			// UV座標
////			DirectX::XMFLOAT2(
////				uvRightBottom.x,
////				uvRightBottom.y
////			),
////	},
////		// 左上
////		{
////			// 頂点
////			DirectX::XMFLOAT3(
////				leftTop.x,
////				leftTop.y,
////				c_VertexZ
////			),
////			// UV座標
////			DirectX::XMFLOAT2(
////				uvLeftTop.x,
////				uvLeftTop.y
////			),
////	},
////		// 左下
////		{
////			// 頂点
////			DirectX::XMFLOAT3(
////				leftTop.x,
////				rightBottom.y,
////				c_VertexZ
////			),
////			// UV座標
////			DirectX::XMFLOAT2(
////				uvLeftTop.x,
////				uvRightBottom.y
////			),
////	}
////	};
////
////	// 板ポリゴン(四角形ポリゴン)のバッファを定義
////	D3D11_BUFFER_DESC bd;
////	SecureZeroMemory(&bd, sizeof(bd));
////	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
////	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
////	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド
////
////	// サブリソースのデータを定義
////	D3D11_SUBRESOURCE_DATA subResourceData;
////	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
////	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ
////
////	// 頂点バッファの開放
////	m_pVertexBuffer.Reset();
////
////	HRESULT hr;
////
////	// 頂点バッファ生成
////	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
////		&bd,
////		&subResourceData,
////		m_pVertexBuffer.GetAddressOf()
////	);
////	if (FAILED(hr)) {
////		std::string error = "SpriteBuffer is not Create!";
////		ErrorLog(error);
////		return E_FAIL;
////	}
////
////	// 頂点バッファセット
////	uint32_t stride = sizeof(SpriteVertex);
////	uint32_t offset = 0;
////	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
////		0,
////		1,
////		m_pVertexBuffer.GetAddressOf(),
////		&stride,
////		&offset
////	);
////
////	return S_OK;
////}
//
///*!
//	@fn			CreateVertexBuffer
//	@brief		頂点の設定
//*/
////void API::Sprite::SetupVertex()
////{
////	// 頂点宣言
////	DirectX::XMFLOAT2 leftTop, rightBottom;				// 頂点座標
////	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;			// UV座標
////
////	// 各頂点定義
////	leftTop.x		= -0.5f*m_Size.x / c_DefaultSize;	// 左
////	rightBottom.x	=  0.5f*m_Size.x / c_DefaultSize;	// 右
////	leftTop.y		=  0.5f*m_Size.y / c_DefaultSize;	// 上
////	rightBottom.y	= -0.5f*m_Size.y / c_DefaultSize;	// 下
////
////	// UV定義
////	uvLeftTop.x = uvLeftTop.y = 0;
////	uvRightBottom.x = uvRightBottom.y = 1;
////
////	// 頂点構造体定義
////	SpriteVertex vertices[] = {
////		// 右上
////		{
////			// 頂点
////			DirectX::XMFLOAT3(
////				rightBottom.x,
////				leftTop.y,
////				c_VertexZ
////			),
////		// UV座標
////		DirectX::XMFLOAT2(
////			uvRightBottom.x,
////			uvLeftTop.y
////		),
////},
////// 右下
////{
////	// 頂点
////	DirectX::XMFLOAT3(
////		rightBottom.x,
////		rightBottom.y,
////		c_VertexZ
////	),
////		// UV座標
////		DirectX::XMFLOAT2(
////			uvRightBottom.x,
////			uvRightBottom.y
////		),
////},
////// 左上
////{
////	// 頂点
////	DirectX::XMFLOAT3(
////		leftTop.x,
////		leftTop.y,
////		c_VertexZ
////	),
////		// UV座標
////		DirectX::XMFLOAT2(
////			uvLeftTop.x,
////			uvLeftTop.y
////		),
////},
////// 左下
////{
////	// 頂点
////	DirectX::XMFLOAT3(
////		leftTop.x,
////		rightBottom.y,
////		c_VertexZ
////	),
////		// UV座標
////		DirectX::XMFLOAT2(
////			uvLeftTop.x,
////			uvRightBottom.y
////		),
////}
////	};
////
////	// 板ポリゴン(四角形ポリゴン)のバッファを定義
////	D3D11_BUFFER_DESC bd;
////	SecureZeroMemory(&bd, sizeof(bd));
////	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
////	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
////	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド
////
////	// サブリソースのデータを定義
////	D3D11_SUBRESOURCE_DATA subResourceData;
////	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
////	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ
////
////	// 頂点バッファの開放
////	m_pVertexBuffer.Reset();
////
////	HRESULT hr;
////
////	// 頂点バッファ生成
////	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
////		&bd,
////		&subResourceData,
////		m_pVertexBuffer.GetAddressOf()
////	);
////	if (FAILED(hr)) {
////		std::string error = "SpriteBuffer is not Create!";
////		ErrorLog(error);
////		return;
////	}
////
////	// 頂点バッファセット
////	uint32_t stride = sizeof(SpriteVertex);
////	uint32_t offset = 0;
////	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
////		0,
////		1,
////		m_pVertexBuffer.GetAddressOf(),
////		&stride,
////		&offset
////	);
////}
//
///*!
//	@fn		SetupTopology
//	@brief	トポロジーの設定
//	@NOTE	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
//*/
//void API::Sprite::SetupTopology()
//{
//	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
//		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
//	);
//}
//
///*!
//	@fn		CreateInputLayout
//	@brief	頂点レイアウトの設定
//*/
//void API::Sprite::SetupInputLayout()
//{	
//	auto shader = *m_pShader.lock();
//	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
//		*shader->GetInputLayout()
//	);
//
//}
//
///*!
//	@fn		SetupBindShader
//	@brief	シェーダーの設定
//*/
//void API::Sprite::SetupBindShader()
//{
//	auto shader = *m_pShader.lock();
//
//	//	頂点シェーダー
//	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
//		*shader->GetVertexShader(),
//		NULL,
//		NULL
//	);
//
//	//	ピクセルシェーダー
//	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
//		*shader->GetPixelShader(),
//		NULL,
//		NULL
//	);
//
//}
//
///*!
//	@fn		SetupSampler
//	@brief	サンプラーステートの設定
//*/
//void API::Sprite::SetupSampler()
//{
//	auto ptex = *m_pTexture.lock();
//	auto psampler = ptex->GetSamplerState();
//
//	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
//		0,
//		1,
//		psampler
//	);
//}
//
///*!
//	@fn		SetupSRV
//	@brief	ShaderResourceViewの設定
//*/
//void API::Sprite::SetupSRV()
//{
//	auto ptex = *m_pTexture.lock();
//	auto psrv = ptex->GetShaderResourceView();
//
//	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
//		0,
//		1,
//		psrv
//	);
//
//}
//
///*!
//	@fn		SetupConstantBuffer
//	@brief	コンスタントバッファの設定
//*/
////void API::Sprite::SetupConstantBuffer()
////{
////	auto shader = *m_pShader.lock();
////	auto& device = Direct3D11::GetInstance();
////
////	//	ワールド行列
////	//DirectX::XMMATRIX world = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
////	//	*
////	//	DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
////	//	*
////	//	DirectX::XMMatrixTranslation(position.x, position.y, position.z);
////
////	DirectX::XMMATRIX world = transform->GetWorldMatrix();
////	world = DirectX::XMMatrixTranspose(world);
////
////	//	ビュー行列
////	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();
////	view = DirectX::XMMatrixTranspose(view);
////
////	//	プロジェクション行列
////	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();
////	proj = DirectX::XMMatrixTranspose(proj);
////
////	//	頂点シェーダー用のCバッファ登録
////	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
////		0,
////		1,
////		shader->GetConstantBuffer()
////	);
////
////	//	ピクセルシェーダー用のCバッファ登録
////	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
////		0,
////		1,
////		shader->GetConstantBuffer()
////	);
////
////	//	マッピング用変数の宣言
////	D3D11_MAPPED_SUBRESOURCE pMapData;
////
////	//	バッファへのアクセス(書き換え)許可
////	HRESULT hr;
////	hr = device.GetImmediateContext()->Map(
////		*shader->GetConstantBuffer(),
////		NULL,
////		D3D11_MAP_WRITE_DISCARD,
////		NULL,
////		&pMapData
////	);
////	if (FAILED(hr)) {
////		std::string error = "Texture mapping is failed!";
////		ErrorLog(error);
////		//	アクセス権を閉じて抜ける
////		device.GetImmediateContext()->Unmap(*shader->GetConstantBuffer(), NULL);
////		return;
////	}
////
////	SpriteShaderBuffer cb;
////
////	SecureZeroMemory(&cb, sizeof(cb));
////
////	auto ptex = *m_pTexture.lock();
////
////	//	バッファに代入
////	cb.m_WorldMatrix		= world;
////	cb.m_ViewMatrix			= view;
////	cb.m_ProjectionMatrix	= proj;
////	//cb.m.world = world;
////	//cb.m.view = view;
////	//cb.m.proj = proj;
////
////	cb.m_DivNum = DirectX::XMFLOAT2(
////		static_cast<float>(ptex->GetDivCount().x),
////		static_cast<float>(ptex->GetDivCount().y));
////	cb.m_Index = DirectX::XMFLOAT2(
////		static_cast<float>(ptex->GetAtlasOffset().x),
////		static_cast<float>(ptex->GetAtlasOffset().y));
////	cb.m_Color				= ptex->color.GetRGBA();
////	//cb.color = ptex->color.GetRGBA();
////
////	//	メモリコピー
////	memcpy_s(pMapData.pData, pMapData.RowPitch, (void*)(&cb), sizeof(cb));
////
////	//	アクセス許可終了
////	device.GetImmediateContext()->Unmap(
////		*shader->GetConstantBuffer(),
////		NULL
////	);
////}
//
///*!
//	@fn		CreateVertexBuffer
//	@brief	頂点バッファ設定
//*/
////void API::Sprite::SetupVertexBuffer()
////{
////	uint32_t stride = sizeof(SpriteVertex);
////	uint32_t offset = 0;
////	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
////		0,
////		1,
////		m_pVertexBuffer.GetAddressOf(),
////		&stride,
////		&offset
////	);
////}
//
///*!
//	@fn		SetupBlendState
//	@brief	ブレンドステートを設定
//*/
//void API::Sprite::SetupBlendState()
//{
//	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
//		m_pBlendState.Get(),
//		NULL,
//		m_StencilMask
//	);
//}
//
//HRESULT API::Sprite::SetupConstantBuffer()
//{
//	auto context = Direct3D11::GetInstance().GetImmediateContext();
//	DirectX::XMMATRIX world = transform->GetWorldMatrix();
//	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();
//	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();
//
//	//world = DirectX::XMMatrixTranspose(world);
//	//view = DirectX::XMMatrixTranspose(view);
//	//proj = DirectX::XMMatrixTranspose(proj);
//
//	auto shader = *m_pShader.lock();
//	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
//	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
//
//	HRESULT hr;
//
//	D3D11_MAPPED_SUBRESOURCE mapData;
//	hr = context->Map(
//		*shader->GetConstantBuffer(),
//		NULL,
//		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
//		NULL,
//		&mapData
//	);
//	if (FAILED(hr)) {
//		ErrorLog("Texture mapping is failed!");
//		context->Unmap(*shader->GetConstantBuffer(), NULL);
//		return E_FAIL;
//	}
//	D3D11::Graphic::Sprite::ConstantBuffer cb;
//	SecureZeroMemory(&cb, sizeof(cb));
//
//	//cb.matrix.world = world;
//	//cb.matrix.view = view;
//	//cb.matrix.proj = proj;
//	
//	cb.world = world;
//	cb.view = view;
//	cb.proj = proj;
//
//	//	メモリコピー
//	memcpy_s(mapData.pData, mapData.RowPitch, (void*)(&cb), sizeof(cb));
//
//	//	アクセス許可終了
//	context->Unmap(*shader->GetConstantBuffer(), NULL);
//	return S_OK;
//}
//
////HRESULT API::Sprite::CreateIndexBuffer()
////{
////	//	バッファの仕様
////	D3D11_BUFFER_DESC bd;
////	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
////	bd.Usage = D3D11_USAGE_DEFAULT;
////	bd.ByteWidth = sizeof(uint32_t)*GetArraySize(c_Indices);
////	bd.CPUAccessFlags = 0;
////	bd.MiscFlags = NULL;
////
////	//	サブリソースの仕様	
////	D3D11_SUBRESOURCE_DATA sd;
////	SecureZeroMemory(&sd, sizeof(sd));
////	sd.pSysMem = c_Indices;
////
////	//	インデックスバッファ作成
////	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
////		&bd,
////		&sd,
////		m_pIndexBuffer.GetAddressOf()
////	);
////}
////
////HRESULT API::Sprite::CreateVertexBuffer()
////{
////	//	バッファの仕様
////	D3D11_BUFFER_DESC bd;
////	bd.Usage = D3D11_USAGE_DEFAULT;
////	bd.ByteWidth = sizeof(D3D11::Graphic::SpriteVertex) * GetArraySize(c_Vertices);
////	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
////	bd.CPUAccessFlags = 0;
////	bd.MiscFlags = NULL;
////
////	//	サブリソースの仕様
////	D3D11_SUBRESOURCE_DATA sd;
////
////	
////	D3D11::Graphic::SpriteVertex verttices[] =
////	{
////		{c_Vertices[0],{0,0}},
////		{c_Vertices[1],{1,0}},
////		{c_Vertices[2],{1,1}},
////		{c_Vertices[3],{0,1}},
////	};
////	sd.pSysMem = verttices;
////
////	//	頂点バッファ作成
////	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
////		&bd,
////		&sd,
////		m_pVertexBuffer.GetAddressOf()
////	);
////}
#endif
/*!
	@brief	コンストラクタ
*/
Sprite::Sprite()
{
	m_pVertexBuffer = nullptr;
	m_pBlendState = nullptr;
	//m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;
	transform = std::make_shared<Transform>();
	SetupBlendPreset(BlendPreset::Default);
}

HRESULT API::Sprite::Init()
{
	HRESULT hr;
	auto&d3d = Direct3D11::GetInstance();
	auto device = d3d.GetDevice();
	auto context = d3d.GetImmediateContext();

	//頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	hr = device->CreateInputLayout(
		layout,
		GetArraySize(layout),
		g_vs_main,
		sizeof(g_vs_main),
		m_pInputLayout.GetAddressOf()
	);
	if (FAILED(hr)) { return E_FAIL; }

	//頂点シェーダー
	hr = device->CreateVertexShader(
		&g_vs_main,
		sizeof(g_vs_main),
		NULL,
		m_pVertexShader.GetAddressOf()
	);
	if (FAILED(hr)) { return E_FAIL; }

	//ピクセルシェーダー
	hr = device->CreatePixelShader(
		&g_ps_main,
		sizeof(g_ps_main),
		NULL,
		m_pPixelShader.GetAddressOf()
	);
	if (FAILED(hr)) { return E_FAIL; }

	//コンスタントバッファ
	D3D11_BUFFER_DESC pcb;
	pcb.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	pcb.ByteWidth = sizeof(D3D11::Graphic::Sprite::ConstantBuffer);
	pcb.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	pcb.MiscFlags = 0;
	pcb.StructureByteStride = 0;
	pcb.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer(
		&pcb,
		NULL,
		m_pConstantBuffer.GetAddressOf()
	);
	if (FAILED(hr)) { return E_FAIL; }

	//頂点バッファ
	DirectX::XMFLOAT2 leftTop, rightBottom;			// 頂点座標
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;		// UV座標
	leftTop.x = -0.5f * 10;//size.x / c_NormalizeSize;// 左
	rightBottom.x = 0.5f * 10;// size.x / c_NormalizeSize;// 右
	leftTop.y = 0.5f * 10;// size.y / c_NormalizeSize;// 上
	rightBottom.y = -0.5f * 10;// size.y / c_NormalizeSize;// 下
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = uvRightBottom.y = 1;
	SpriteVertex vertices[] = {
		// 右上
		{
			// 頂点
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				0
			),
		// UV座標
		DirectX::XMFLOAT2(
			uvRightBottom.x,
			uvLeftTop.y
		),
},
// 右下
{
	// 頂点
	DirectX::XMFLOAT3(
		rightBottom.x,
		rightBottom.y,
		0
	),
		// UV座標
		DirectX::XMFLOAT2(
			uvRightBottom.x,
			uvRightBottom.y
		),
},
// 左上
{
	// 頂点
	DirectX::XMFLOAT3(
		leftTop.x,
		leftTop.y,
		0
	),
		// UV座標
		DirectX::XMFLOAT2(
			uvLeftTop.x,
			uvLeftTop.y
		),
},
// 左下
{
	// 頂点
	DirectX::XMFLOAT3(
		leftTop.x,
		rightBottom.y,
		0
	),
		// UV座標
		DirectX::XMFLOAT2(
			uvLeftTop.x,
			uvRightBottom.y
		),
}
	};
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ
	m_pVertexBuffer.Reset();
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		m_pVertexBuffer.GetAddressOf()
	);
	if (FAILED(hr)) { return E_FAIL; }

	transform = std::make_shared<Transform>();
	//m_pShaderResourceView = pTex->GetShaderResourceView();
	return S_OK;
}

void API::Sprite::Render()
{
	//トポロジー
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	//頂点レイアウト
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		m_pInputLayout.Get()
	);

	//シェーダーセット
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		m_pVertexShader.Get(),
		NULL,
		NULL
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		m_pPixelShader.Get(),
		NULL,
		NULL
	);

	//コンスタントバッファ
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	D3D11::Graphic::Sprite::ConstantBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));
	HRESULT hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		m_pConstantBuffer.Get(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	if (FAILED(hr)) { 
		context->Unmap(m_pConstantBuffer.Get(), NULL);
		return; 
	}
	DirectX::XMMATRIX w, v, p;
	w = transform->GetWorldMatrix();
	v = Camera::GetInstance().GetViewMatrix();
	p = Camera::GetInstance().GetProjMatrix();
	//v = DirectX::XMMatrixIdentity();
	//p = DirectX::XMMatrixIdentity();

	cb.world = DirectX::XMMatrixTranspose(w);
	cb.view = DirectX::XMMatrixTranspose(v);
	cb.proj = DirectX::XMMatrixTranspose(p);
	cb.color = color.GetRGBA();

	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		m_pConstantBuffer.Get(),
		NULL
	);
	context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	//頂点バッファセット
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);


	//サンプラー
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		//m_pSamplerState.GetAddressOf()
		pTex->GetSamplerState()
	);

	//SRV
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		//m_pShaderResourceView.GetAddressOf()
		pTex->GetShaderResourceView()
	);

	//ブレンドステート
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	//描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,
		NULL
	);
}

/*!
	@fn			SetupBlendPreset
	@brief		指定したプリセットのブレンドステートをメンバに設定する
	@param[in]	指定するプリセットの列挙体
*/
void API::Sprite::SetupBlendPreset(BlendPreset preset)
{
	/*!
	※
	SRC:ソース(これから描画するピクセルの色)
	DEST:ディストネーション(レンダリングターゲットに描画されているピクセルの色)
	最終的な描画色は以下の「混合関数」によって決まる
		SRC × ブレンディング係数 ＋ DEST × ブレンディング係数
		SRCALPHA:	 SRC のα値
		INVSRCALPHA: 1 - SRC のα値
		DESTALPHA:	 DESTのα値
		Cr = Cd * (1 - As) + Cs * As
		   = Cs * As + Cd * (1 - As)
	*/

	//	メモリを解放し、再設定する
	if (m_pBlendState != nullptr) {
		m_pBlendState.Reset();
	}

	// αブレンド
	// αテスト設定
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));

	// ブレンドの有効・無効
	bd.RenderTarget[0].BlendEnable = preset != BlendPreset::Default;

	//	ブレンディング係数
	D3D11_BLEND src, dest, srcAlpha, destAlpha;

	//	ブレンディングオプション
	D3D11_BLEND_OP blendOp, blendOpAlpha;

	//	プリセットで分岐
	switch (preset)
	{
		//	デフォルト
	case API::Sprite::BlendPreset::Default:
	{
		//	係数
		src = D3D11_BLEND::D3D11_BLEND_ONE;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ZERO;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	オプション
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	線形
	case API::Sprite::BlendPreset::Linear:
	{
		//	係数
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	オプション
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	加算
	case API::Sprite::BlendPreset::Add:
	{
		//	係数
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ONE;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	オプション
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	減算
	case API::Sprite::BlendPreset::Subtraction:
	{
		//	係数
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ONE;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	オプション
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	乗算
	case API::Sprite::BlendPreset::Multiple:
	{
		//	係数
		src = D3D11_BLEND::D3D11_BLEND_ZERO;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	オプション
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	例外処理
	default:
		std::string error = "Invalid value for argument of SetupBlendPreset function!";
		ErrorLog(error);
		return;
	}

	//	係数
	bd.RenderTarget[0].SrcBlend = src;
	bd.RenderTarget[0].DestBlend = dest;
	bd.RenderTarget[0].SrcBlendAlpha = srcAlpha;
	bd.RenderTarget[0].DestBlendAlpha = destAlpha;

	// ブレンドオプション
	bd.RenderTarget[0].BlendOp = blendOp;
	bd.RenderTarget[0].BlendOpAlpha = blendOpAlpha;

	//	RGBA 全てをブレンドする
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	// アンチエイリアス処理
	bd.IndependentBlendEnable = false;

	HRESULT hr;

	// ブレンドステートの作成
	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
		&bd,
		m_pBlendState.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "BlendState is not create!";
		ErrorLog(error);
		return;
	}
}
