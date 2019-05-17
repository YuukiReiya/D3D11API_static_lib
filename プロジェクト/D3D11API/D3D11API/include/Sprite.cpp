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
#include "ShaderManager.h"
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "Debug.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace D3D11;
using namespace D3D11::Graphic;
using namespace API;

/*!
	@brief	コンストラクタ
*/
Sprite::Sprite()
{
	m_pVertexBuffer		= nullptr;
	m_pBlendState		= nullptr;
	//m_Pos = { 0,0,0 };
	//m_Scale = { 1,1 ,0 };
	//m_Rot = {0,0,0};
	m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;
	//Initialize();

	transform = std::make_shared<Transform>();
	SetupBlendPreset(BlendPreset::Default);
}

/*!
	@brief	デストラクタ
*/
Sprite::~Sprite()
{
	Finalize();
}

/*!
	@fn		イニシャライズ
	@brief	初期化
	@detail	ブレンドステートの作成を行う
*/
HRESULT Sprite::Initialize()
{
	HRESULT hr;

	/*!
	※
	SRC:ソース(これから描画するピクセルの色)
	DEST:ディストネーション(レンダリングターゲットに描画されているピクセルの色)
	
	最終的な描画色は以下の「混合関数」によって決まる
	
		SRC × ブレンディング係数 ＋ DEST × ブレンディング係数
	
		SRCALPHA:	 SRC のα値
		INVSRCALPHA: 1 - SRC のα値
		DESTALPHA:	 DESTのα値
	*/

	// αブレンド
	// αテスト設定
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));

	// ブレンドの有効・無効
	bd.RenderTarget[0].BlendEnable		= true;

	// ブレンディング係数の設定
	bd.RenderTarget[0].SrcBlend			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend		= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	//
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;


	// ブレンドオプション
	bd.RenderTarget[0].BlendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	//
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	// アンチエイリアス処理
	//bd.AlphaToCoverageEnable = true;	// 切り取った部分に対するアンチエイリアス処理の有無
	bd.IndependentBlendEnable = false;

	// ブレンドステートの作成
	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
		&bd,
		m_pBlendState.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "BlendState is not create!";
		ErrorLog(error);
		return E_FAIL;
	}

	// ブレンドステートの設定
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	return S_OK;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な解放とメンバの初期化
*/
void Sprite::Finalize()
{
	// メンバの初期化
	//m_Pos = { 0,0,0 };
	//m_Scale = { 1,1 ,0 };
	//m_Rot = { 0,0,0 };
	m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;

	// 開放
	m_pVertexBuffer.Reset();
	m_pBlendState.Reset();
}

/*!
	@fn		Render
	@brief	描画
*/
void API::Sprite::Render()
{
#if defined DEBUG_SPRITE

	//	シェーダーの参照切れ
	if (m_pShader.expired()) {
		ErrorLog("The \"m_pShader\" value referenced in this class is invalid!");
		return;
	}

	//	テクスチャの参照切れ
	if (m_pTexture.expired()) {
		ErrorLog("The \"m_pTexture\" value referenced in this class is invalid!");
		return;
	}
#endif

	//	トポロジーのセット
	SetupTopology();

	//	頂点レイアウトのセット
	SetupInputLayout();

	//	シェーダーのセット
	SetupBindShader();

	//	サンプラーステートのセット
	SetupSampler();

	//	ShaderResourceViewのセット
	SetupSRV();

	//	コンスタントバッファのセット
	SetupConstantBuffer();

	//	ブレンドステートのセット
	SetupBlendState();

	//	描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,		// 頂点数(板ポリゴンなので頂点数は4つ)
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
			src			= D3D11_BLEND::D3D11_BLEND_ONE;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ZERO;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	オプション
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
		break;

		//	線形
		case API::Sprite::BlendPreset::Linear:
		{
			//	係数
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	オプション
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	加算
		case API::Sprite::BlendPreset::Add:
		{
			//	係数
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	オプション
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	減算
		case API::Sprite::BlendPreset::Subtraction:
		{
			//	係数
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	オプション
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	乗算
		case API::Sprite::BlendPreset::Multiple:
		{
			//	係数
			src			= D3D11_BLEND::D3D11_BLEND_ZERO;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	オプション
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	例外処理
		default:
			std::string error = "Invalid value for argument of SetupBlendPreset function!";
			ErrorLog(error);
			return;
	}
	
	//	係数
	bd.RenderTarget[0].SrcBlend			= src;
	bd.RenderTarget[0].DestBlend		= dest;
	bd.RenderTarget[0].SrcBlendAlpha	= srcAlpha;
	bd.RenderTarget[0].DestBlendAlpha	= destAlpha;

	// ブレンドオプション
	bd.RenderTarget[0].BlendOp		= blendOp;
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

/*!
	@fn			SetupTexture
	@brief		テクスチャの設定
	@detail		弱参照でバインドし、この時点で頂点生成を行う
	@param[in]	登録するテクスチャのポインタ
*/
void API::Sprite::SetupTexture(Texture*  texture)
{
	m_pTexture = texture->GetSharedPtr();
	m_Size = texture->GetSize();

	//	頂点生成
	SetupVertex();

	//	生成済の頂点から頂点バッファを設定
	SetupVertexBuffer();
}

/*!
	@fn			SetupShader
	@brief		シェーダーの設定
	@detail		弱参照でバインドする
	@param[in]	登録するシェーダーのポインタ
*/
void API::Sprite::SetupShader(D3D11::Graphic::AbstractShader * shader)
{
	//m_pVertexShader		= shader->GetVertexShaderPtr();
	//m_pVertexLayout		= shader->GetInputLayoutPtr();
	//m_pPixelShader		= shader->GetPixelShaderPtr();
	//m_pConstantBuffer	= shader->GetConstantBufferPtr();
	m_pShader = shader->GetSharedPtr();
}

/*!
	@fn			頂点生成
	@brief		スプライトの頂点を生成する
	@detail		頂点構造体を使って、情報を頂点バッファにバインド
	@param[in]	画像サイズ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT Sprite::CreateVertex(DirectX::XMINT2 size)
{
	// 頂点宣言
	DirectX::XMFLOAT2 leftTop, rightBottom;			// 頂点座標
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;		// UV座標

	// 各頂点定義
	leftTop.x		= -0.5f*size.x / c_DefaultSize;// 左
	rightBottom.x	=  0.5f*size.x / c_DefaultSize;// 右
	leftTop.y		=  0.5f*size.y / c_DefaultSize;// 上
	rightBottom.y	= -0.5f*size.y / c_DefaultSize;// 下

	// UV定義
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = uvRightBottom.y = 1;

	// 頂点構造体定義
	SpriteVertex vertices[] = {
		// 右上
		{
			// 頂点
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				c_VertexZ
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
				c_VertexZ
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
				c_VertexZ
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
				c_VertexZ
			),
			// UV座標
			DirectX::XMFLOAT2(
				uvLeftTop.x,
				uvRightBottom.y
			),
	}
	};

	// 板ポリゴン(四角形ポリゴン)のバッファを定義
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド

	// サブリソースのデータを定義
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ

	// 頂点バッファの開放
	m_pVertexBuffer.Reset();

	HRESULT hr;

	// 頂点バッファ生成
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		m_pVertexBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "SpriteBuffer is not Create!";
		ErrorLog(error);
		return E_FAIL;
	}

	// 頂点バッファセット
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	return S_OK;
}

/*!
	@fn			CreateVertexBuffer
	@brief		頂点の設定
*/
void API::Sprite::SetupVertex()
{
	// 頂点宣言
	DirectX::XMFLOAT2 leftTop, rightBottom;				// 頂点座標
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;			// UV座標

	// 各頂点定義
	leftTop.x		= -0.5f*m_Size.x / c_DefaultSize;	// 左
	rightBottom.x	=  0.5f*m_Size.x / c_DefaultSize;	// 右
	leftTop.y		=  0.5f*m_Size.y / c_DefaultSize;	// 上
	rightBottom.y	= -0.5f*m_Size.y / c_DefaultSize;	// 下

	// UV定義
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = uvRightBottom.y = 1;

	// 頂点構造体定義
	SpriteVertex vertices[] = {
		// 右上
		{
			// 頂点
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				c_VertexZ
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
		c_VertexZ
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
		c_VertexZ
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
		c_VertexZ
	),
		// UV座標
		DirectX::XMFLOAT2(
			uvLeftTop.x,
			uvRightBottom.y
		),
}
	};

	// 板ポリゴン(四角形ポリゴン)のバッファを定義
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド

	// サブリソースのデータを定義
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ

	// 頂点バッファの開放
	m_pVertexBuffer.Reset();

	HRESULT hr;

	// 頂点バッファ生成
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		m_pVertexBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "SpriteBuffer is not Create!";
		ErrorLog(error);
		return;
	}

	// 頂点バッファセット
	//uint32_t stride = sizeof(SpriteVertex);
	//uint32_t offset = 0;
	//Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
	//	0,
	//	1,
	//	m_pVertexBuffer.GetAddressOf(),
	//	&stride,
	//	&offset
	//);
}

/*!
	@fn		SetupTopology
	@brief	トポロジーの設定
	@NOTE	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
*/
void API::Sprite::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
}

/*!
	@fn		CreateInputLayout
	@brief	頂点レイアウトの設定
*/
void API::Sprite::SetupInputLayout()
{	
	auto shader = *m_pShader.lock();
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		*shader->GetInputLayoutPtr()
	);

}

/*!
	@fn		SetupBindShader
	@brief	シェーダーの設定
*/
void API::Sprite::SetupBindShader()
{
	auto shader = *m_pShader.lock();

	//	頂点シェーダー
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*shader->GetVertexShaderPtr(),
		NULL,
		NULL
	);

	//	ピクセルシェーダー
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*shader->GetPixelShaderPtr(),
		NULL,
		NULL
	);

}

/*!
	@fn		SetupSampler
	@brief	サンプラーステートの設定
*/
void API::Sprite::SetupSampler()
{
	auto ptex = *m_pTexture.lock();
	auto psampler = ptex->GetSamplerState();

	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		psampler
	);
}

/*!
	@fn		SetupSRV
	@brief	ShaderResourceViewの設定
*/
void API::Sprite::SetupSRV()
{
	auto ptex = *m_pTexture.lock();
	auto psrv = ptex->GetShaderResourceView();

	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		psrv
	);

}

/*!
	@fn		SetupConstantBuffer
	@brief	コンスタントバッファの設定
*/
void API::Sprite::SetupConstantBuffer()
{
	auto shader = *m_pShader.lock();
	auto& device = Direct3D11::GetInstance();

	//	ワールド行列
	//DirectX::XMMATRIX world = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
	//	*
	//	DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
	//	*
	//	DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);

	DirectX::XMMATRIX world = transform->GetMatrix();

	//	ビュー行列
	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();

	//	プロジェクション行列
	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();

	//	頂点シェーダー用のCバッファ登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBufferPtr()
	);

	//	ピクセルシェーダー用のCバッファ登録
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBufferPtr()
	);

	//	マッピング用変数の宣言
	D3D11_MAPPED_SUBRESOURCE pMapData;

	//	バッファへのアクセス(書き換え)許可
	HRESULT hr;
	hr = device.GetImmediateContext()->Map(
		*shader->GetConstantBufferPtr(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pMapData
	);
	if (FAILED(hr)) {
		std::string error = "Texture mapping is failed!";
		ErrorLog(error);
		//	アクセス権を閉じて抜ける
		device.GetImmediateContext()->Unmap(*shader->GetConstantBufferPtr(), NULL);
		return;
	}

	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	auto ptex = *m_pTexture.lock();

	//	バッファに代入
	cb.m_WorldMatrix		= world;
	cb.m_ViewMatrix			= view;
	cb.m_ProjectionMatrix	= proj;
	cb.m_DivNum = DirectX::XMFLOAT2(
		static_cast<float>(ptex->GetDivNum().x),
		static_cast<float>(ptex->GetDivNum().y));
	cb.m_Index = DirectX::XMFLOAT2(
		static_cast<float>(ptex->GetActiveDiv().x),
		static_cast<float>(ptex->GetActiveDiv().y));
	cb.m_Color				= ptex->m_Color.GetRGBA();

	//	メモリコピー
	memcpy_s(pMapData.pData, pMapData.RowPitch, (void*)(&cb), sizeof(cb));

	//	アクセス許可終了
	device.GetImmediateContext()->Unmap(
		*shader->GetConstantBufferPtr(),
		NULL
	);
}

/*!
	@fn		CreateVertexBuffer
	@brief	頂点バッファ設定
*/
void API::Sprite::SetupVertexBuffer()
{
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);
}

/*!
	@fn		SetupBlendState
	@brief	ブレンドステートを設定
*/
void API::Sprite::SetupBlendState()
{
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);
}