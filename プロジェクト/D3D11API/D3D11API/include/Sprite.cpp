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
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "Debug.h"
#include "SpriteConstantBuffer.h"
#include "SpriteCompVS.h"
#include "SpriteCompPS.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@struct	Vertex
	@brief	スプライトの頂点データ構造体
*/
struct Vertex
{
	/*!
		@var	position
		@brief	座標
	*/
	DirectX::XMFLOAT3 position;

	/*!
		@var	uv
		@brief	UV
	*/
	DirectX::XMFLOAT2 uv;
};

/*!
	@brief	コンストラクタ
*/
Sprite::Sprite():
	m_pVertexBuffer(nullptr),
	m_pBlendState(nullptr),
	m_StencilMask(0xffffffff),
	transform(std::make_shared<Transform>())
{
	SetupBlendPreset(BlendPreset::Default);
}

/*!
	@brief	デストラクタ
*/
API::Sprite::~Sprite()
{
	Finalize();
}

/*!
	@fn			Initialize
	@brief		初期化
	@param[in]	テクスチャ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::Initialize(Texture * texture)
{
	try
	{
		if (texture == nullptr||!texture->GetSharedPtr()) { throw std::runtime_error("Texture"); }
		m_pTexture = texture->GetSharedPtr();
	}
	catch (const std::exception&e)
	{
		ErrorLog(std::string("Invalid value for ") + e.what());
		return E_FAIL;
	}

	HRESULT hr;

	//	頂点バッファ生成
	hr = CreateVertexBuffer(
		{
			static_cast<float>(texture->GetSize().x),
			static_cast<float>(texture->GetSize().y) 
		}
	);
	if(FAILED(hr)){
		ErrorLog("Failed to generate vertex buffer");
		return E_FAIL;
	}
	return S_OK;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な解放とメンバの初期化
*/
void API::Sprite::Finalize()
{
	//	メンバの初期化
	m_StencilMask = 0xffffffff;

	//	開放
	m_pShader.reset();
	m_pTexture.reset();
	m_pBlendState.Reset();
	m_pVertexBuffer.Reset();
}

/*!
	@fn		Render
	@brief	描画
*/
void API::Sprite::Render()
{
	//トポロジー
	SetupTopology();

	//	ウィークポインタに有効な参照を持っているか確認
	try
	{
		if (m_pShader.expired()) { throw std::runtime_error("Shader"); }
		if (m_pTexture.expired()) { throw std::runtime_error("Texture"); }
	}
	catch (const std::exception&error)
	{
		ErrorLog(error.what() + std::string(" reference is broken!"));
		return;
	}

	//	シェーダー
	auto shader = *m_pShader.lock();

	//	頂点レイアウト
	SetupInputLayout(shader);

	//	シェーダーのバインド
	SetupBindShader(shader);

	//	コンスタントバッファ
	SetupConstantBuffer(shader);

	//	頂点バッファセット
	SetupVertexBuffer();

	//	テクスチャ
	auto texture = *m_pTexture.lock();

	//サンプラー
	SetupSampler(texture);

	//SRV
	SetupSRV(texture);

	//ブレンドステート
	SetupBlendState();

	//描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,
		NULL
	);
}

void API::Sprite::RenderBillboard()
{
	//トポロジー
	SetupTopology();

	//	ウィークポインタに有効な参照を持っているか確認
	try
	{
		if (m_pShader.expired()) { throw std::runtime_error("Shader"); }
		if (m_pTexture.expired()) { throw std::runtime_error("Texture"); }
		if (m_pBillboardTarget.expired()) { throw std::runtime_error("Billboard Transform"); }
	}
	catch (const std::exception&error)
	{
		ErrorLog(error.what() + std::string(" reference is broken!"));
		return;
	}

	//	シェーダー
	auto shader = *m_pShader.lock();

	//	頂点レイアウト
	SetupInputLayout(shader);

	//	シェーダーのバインド
	SetupBindShader(shader);

	auto pos = m_pBillboardTarget.lock()->GetPosition();
	DirectX::XMVECTOR vec = { pos.x,pos.y,pos.z,0 };
	DirectX::XMVECTOR look = {
		Camera::GetInstance().GetLookAtPt().x,
		Camera::GetInstance().GetLookAtPt().y,
		Camera::GetInstance().GetLookAtPt().z,
		0
	};
	DirectX::XMVECTOR upvec = {
		Camera::GetInstance().GetUpVector().x,
		Camera::GetInstance().GetUpVector().y,
		Camera::GetInstance().GetUpVector().z,
		0
	};
	DirectX::XMMATRIX billboardMat = DirectX::XMMatrixLookAtLH(
		vec,
		look,
		upvec
	);
	//DirectX::XMMATRIX billboardMat = Camera::GetInstance().GetViewMatrix();

	billboardMat.r[3].m128_f32[0] = 0;
	billboardMat.r[3].m128_f32[1] = 0;
	billboardMat.r[3].m128_f32[2] = 0;
	billboardMat = DirectX::XMMatrixInverse(NULL, billboardMat);

	//	コンスタントバッファ
	SetupConstantBuffer(
		shader,
		billboardMat*transform->GetWorldMatrix(),
		//billboardMat.GetWorldMatrix(),
		//billboardMat,
		Camera::GetInstance().GetViewMatrix(),
		Camera::GetInstance().GetProjMatrix()
		);

	//	頂点バッファセット
	SetupVertexBuffer();

	//	テクスチャ
	auto texture = *m_pTexture.lock();

	//サンプラー
	SetupSampler(texture);

	//SRV
	SetupSRV(texture);

	//ブレンドステート
	SetupBlendState();

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

/*!
	@fn			SetupTexture
	@brief		テクスチャの設定
	@detail		弱参照でバインドし、この時点で頂点生成を行う
	@param[in]	登録するテクスチャのポインタ
*/
void API::Sprite::SetupTexture(Texture * texture)
{
	m_pTexture = texture->GetSharedPtr();
}

/*!
	@fn			SetupShader
	@brief		シェーダーの設定
	@detail		弱参照でバインドする
	@param[in]	登録するシェーダーのポインタ
*/
void API::Sprite::SetupShader(D3D11::Graphic::AbstractShader * shader)
{
	m_pShader = shader->GetSharedPtr();
}

/*!
	@fn		SetupTopology
	@brief	トポロジーの設定
	@NOTE	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
*/
void API::Sprite::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
}

/*!
	@fn			SetupInputLayout
	@brief		頂点レイアウトの設定
	@param[in]	シェーダー
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupInputLayout(D3D11::Graphic::AbstractShader * shader)
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		*shader->GetInputLayout()
	);
}

/*!
	@fn			SetupBindShader
	@brief		シェーダーのバインド
	@param[in]	シェーダー
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupBindShader(D3D11::Graphic::AbstractShader * shader)
{
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*shader->GetVertexShader(),
		NULL,
		NULL
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*shader->GetPixelShader(),
		NULL,
		NULL
	);
}

/*!
	@fn			SetupConstantBuffer
	@brief		コンスタントバッファの設定
	@param[in]	シェーダー
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupConstantBuffer(D3D11::Graphic::AbstractShader * shader)
{
	//	マッピング用変数の宣言
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	//	バッファへのアクセス(書き換え)許可
	HRESULT hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		*shader->GetConstantBuffer(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	if (FAILED(hr)) {
		//	アクセス権を閉じて抜ける
		context->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	//	定数バッファ宣言
	D3D11::Graphic::Sprite::ConstantBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	WVP行列
	DirectX::XMMATRIX w, v, p;
	w = transform->GetWorldMatrix();
	v = Camera::GetInstance().GetViewMatrix();
	p = Camera::GetInstance().GetProjMatrix();

	//	定数バッファ用の変数に受け渡し
	cb.matrix.world = DirectX::XMMatrixTranspose(w);
	cb.matrix.view = DirectX::XMMatrixTranspose(v);
	cb.matrix.proj = DirectX::XMMatrixTranspose(p);
	cb.color = color.GetRGBA();

	//	メモリコピー
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	
	//	アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);

	//	シェーダーに定数バッファをバインド
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
}

/*!
	@fn			SetupConstantBuffer
	@brief		コンスタントバッファの設定
	@param[in]	シェーダー
	@param[in]	ワールド行列
	@param[in]	ビュー行列
	@param[in]	プロジェクション行列
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupConstantBuffer(D3D11::Graphic::AbstractShader * shader, DirectX::XMMATRIX w, DirectX::XMMATRIX v, DirectX::XMMATRIX p)
{
	//	マッピング用変数の宣言
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	//	バッファへのアクセス(書き換え)許可
	HRESULT hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		*shader->GetConstantBuffer(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	if (FAILED(hr)) {
		//	アクセス権を閉じて抜ける
		context->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	//	定数バッファ宣言
	D3D11::Graphic::Sprite::ConstantBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	定数バッファ用の変数に受け渡し
	cb.matrix.world = DirectX::XMMatrixTranspose(w);
	cb.matrix.view = DirectX::XMMatrixTranspose(v);
	cb.matrix.proj = DirectX::XMMatrixTranspose(p);
	cb.color = color.GetRGBA();

	//	メモリコピー
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	//	アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);

	//	シェーダーに定数バッファをバインド
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
}

/*!
	@fn			SetupConstantBuffer
	@brief		コンスタントバッファの設定
	@param[in]	シェーダー
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupVertexBuffer()
{
	uint32_t stride = sizeof(Vertex);
	static constexpr uint32_t offset = 0;
	Direct3D11::GetInstance().GetImmediateContext()->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);
}

/*!
	@fn			SetupSampler
	@brief		サンプラーステートの設定
	@param[in]	テクスチャ
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupSampler(Texture * texture)
{
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		texture->GetSamplerState()
	);

}

/*!
	@fn			SetupSRV
	@brief		ShaderResourceViewの設定
	@param[in]	テクスチャ
	@note		弱参照で取得したポインタから取得
*/
void API::Sprite::SetupSRV(Texture * texture)
{
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		texture->GetShaderResourceView()
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

/*!
	@fn			CreateVertexBuffer
	@brief		頂点の生成
	@detail		画像のサイズに合わせて板ポリゴンを生成する
	@param[in]	画像のサイズ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::CreateVertexBuffer(const DirectX::XMFLOAT2 size)
{
	//	画像比率
	float x, y;
	x = size.x <= size.y ? size.x*c_NormalizeSize / size.y : c_NormalizeSize;
	y = size.x <= size.y ? c_NormalizeSize : size.y*c_NormalizeSize / size.x;

	//	座標
	DirectX::XMFLOAT2 lt, rb;//	左上,右下
	lt.x = -0.5f * x;//	左
	rb.x =  0.5f * x;//	右
	lt.y =  0.5f * y;//	上
	rb.y = -0.5f * y;//	下

	//	UV
	DirectX::XMFLOAT2 uvLT, uvRT;//	左上,右下
	uvLT.x = uvLT.y = 0;
	uvRT.x = uvRT.y = 1;

	//	頂点宣言
	Vertex vertices[] = {
		// 右上
		{
			// 頂点
			DirectX::XMFLOAT3(
				rb.x,
				lt.y,
				0
			),
			// UV座標
			DirectX::XMFLOAT2(
				uvRT.x,
				uvLT.y
			),
		},
		// 右下
		{
			// 頂点
			DirectX::XMFLOAT3(
				rb.x,
				rb.y,
				0
			),
			
			// UV座標
			DirectX::XMFLOAT2(
				uvRT.x,
				uvRT.y
			),
		},
		// 左上
		{
			// 頂点
			DirectX::XMFLOAT3(
				lt.x,
				lt.y,
				0
			),
			// UV座標
			DirectX::XMFLOAT2(
				uvLT.x,
				uvLT.y
			),
		},
		// 左下
		{
			// 頂点
			DirectX::XMFLOAT3(
				lt.x,
				rb.y,
				0
			),
			// UV座標
			DirectX::XMFLOAT2(
				uvLT.x,
				uvRT.y
			),
		}
	};
	
	//	バッファ仕様
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPUから読み込みと書き込みを許可
	bd.ByteWidth = sizeof(vertices);			// バッファのサイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてレンダリングパイプラインにバインド

	//	サブリソース
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// 初期化データへのポインタ

	//	頂点の開放
	m_pVertexBuffer.Reset();

	//	頂点の生成
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		m_pVertexBuffer.GetAddressOf()
	);
}
