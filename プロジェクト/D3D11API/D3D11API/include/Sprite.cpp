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

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace D3D11;
using namespace D3D11::Graphic;
using namespace API;

/*!
	@def	定数宣言
*/
const float Sprite::c_NormalizeSize = 100.0f;	/*!< スケールが1の時の基準となるサイズ(100×100が大きさの基準) */
const float Sprite::c_ScaleZ	= 0;			/*!< Z方向のスケールは常にゼロで計算する */
const float Sprite::c_VertexZ	= 0;			/*!< 板ポリの頂点座標のz値 */

/*!
	@brief	コンストラクタ
*/
Sprite::Sprite()
{
	m_pVertexBuffer		= nullptr;
	m_pBlendState		= nullptr;
	m_Pos = { 0,0,0 };
	m_Scale = { 1,1 ,0 };
	m_Rot = {0,0,0};
	m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;
	//Initialize();

	SetupBlendState(BlendPreset::Linear);
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
	m_Pos = { 0,0,0 };
	m_Scale = { 1,1 ,0 };
	m_Rot = { 0,0,0 };
	m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;

	// 開放
	Release();
}

/*!
	@fn		リリース
	@brief	解放処理
	@detail	メンバの明示的な解放
*/
void Sprite::Release()
{
	m_pVertexBuffer.Reset();
	m_pBlendState.Reset();
}

/*!
	@fn			ブレンドステートの作成
	@brief		作成したブレンドステートは描画時にバインドされる
	@detail		メンバのブレンドステートをを変更
	@param[in]	自己定義のブレンドステート
*/
void Sprite::CreateAlphaBlendState(D3D11_BLEND_DESC desc)
{
	HRESULT hr;

	// メモリ開放
	m_pBlendState.Reset();

	// ブレンドステート作成
	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
		&desc,
		m_pBlendState.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "BlendState is not create!";
		ErrorLog(error);
	}
}

/*!
	@fn			SetupBlendState
	@brief		指定したプリセットのブレンドステートをメンバに設定する
	@param[in]	指定するプリセットの列挙体
*/
void API::Sprite::SetupBlendState(BlendPreset preset)
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

	// ブレンディング係数の設定
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	//bd.RenderTarget[0].BlendOp		= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//bd.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
	//bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	//bd.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;


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
			std::string error = "Invalid value for argument of SetupBlendState function!";
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
		return;
	}

	// ブレンドステートの設定
	//Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
	//	m_pBlendState.Get(),
	//	NULL,
	//	m_StencilMask
	//);

}

/*!
	@fn			テクスチャの描画
	@brief		シンプルテクスチャの描画
	@detail		map,unmapを使ってテクスチャマッピングをする描画
	@param[in]	描画するテクスチャ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::Render(Texture * pTexture)
{
	HRESULT hr;

	const auto size = pTexture->GetSize();

	// 異なるサイズのテクスチャが渡されたら頂点生成
	if (m_Size.x != size.x || m_Size.y != size.y) {

		// 頂点バッファ生成
		hr = CreateVertex(size);
		if (FAILED(hr)) {
			std::string error = "Create vertex is failed!";
			ErrorLog(error);
			return E_FAIL;
		}

		// テクスチャのサイズをキャッシュしておく
		m_Size = size;
	}

	// トポロジーセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	auto shaderData = ShaderManager::GetInstance().GetShaderData(ShaderManager::c_szSimpleTextureShader);

	// 頂点インプットレイアウトセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		shaderData->m_pVertexLayout.Get()
	);

	// シェーダーの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		shaderData->m_pVertexShader.Get(),
		NULL,
		NULL
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		shaderData->m_pPixelShader.Get(),
		NULL,
		NULL
	);

	// コンスタントバッファの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);

	// サンプラー取得
	auto ppSampler = pTexture->GetSamplerState();

	// SRV取得
	auto ppSRV = pTexture->GetShaderResourceView();

	// テクスチャ
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		ppSampler
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		ppSRV
	);

	// 座標変換
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld	= DirectX::XMMatrixIdentity();
	mTran	= DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot	= DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale	= DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, c_ScaleZ);

	// ワールド変換
	mWorld = mScale * mRot * mTran;

	// マッピング用変数宣言
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	// シェーダー側に渡すコンスタントバッファ宣言
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	// バッファへのアクセス許可(書き換え)
	hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		shaderData->m_pConstantBuffer.Get(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	if (FAILED(hr)) {
		std::string error = "Texture mapping is failed!";
		ErrorLog(error);
		Direct3D11::GetInstance().GetImmediateContext()->Unmap(shaderData->m_pConstantBuffer.Get(), NULL);/*!< アクセス権を閉じて抜ける */
		return E_FAIL;
	}

	auto camera = &Camera::GetInstance();
	const Color& color = pTexture->m_Color;
	// コンスタントバッファにデータを送る
	DirectX::XMMATRIX m = mWorld*camera->GetViewMatrix()*camera->GetProjMatrix();
	cb.m_WVP = m;						// ワールド行列
	cb.m_DivNum = { 1,1 };
	cb.m_Index = { 0,0 };
	cb.m_Color = color.GetRGBA();
	
	// メモリコピー
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	
	// アクセス許可終了
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		shaderData->m_pConstantBuffer.Get(), 
		NULL
	);
	
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

	// ブレンドステートの設定
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	// 描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,		// 頂点数(板ポリゴンなので頂点数は4つ)
		NULL
	);

	return S_OK;
}

/*!
	@fn			テクスチャの描画
	@brief		アトラステクスチャの描画
	@detail		SubResourceを使ってUpdateSubResourceをする描画
	@param[in]	描画するテクスチャ
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::Render(TextureAtlas * pTexture)
{
	
	HRESULT hr;

	auto size = pTexture->GetSize();

	// 頂点バッファ生成
	hr = CreateVertex(size);
	if (FAILED(hr)) {
		std::string error = "Create vertex is failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	// テクスチャのサイズをキャッシュしておく
	m_Size = size;

	// トポロジーセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	auto shaderData = ShaderManager::GetInstance().GetShaderData(ShaderManager::c_szTextureAtlasShader);

	// 頂点インプットレイアウトセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		shaderData->m_pVertexLayout.Get()
	);

	// シェーダーの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		shaderData->m_pVertexShader.Get(),
		NULL,
		NULL
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		shaderData->m_pPixelShader.Get(),
		NULL,
		NULL
	);

	// コンスタントバッファの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);

	// サンプラー取得
	auto ppSampler = pTexture->GetSamplerState();

	// SRV取得
	auto ppSRV = pTexture->GetShaderResourceView();

	// テクスチャ
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		ppSampler
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		ppSRV
	);

	// 座標変換
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld	= DirectX::XMMatrixIdentity();
	mTran	= DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot	= DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale	= DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, c_ScaleZ);

	// ワールド変換
	mWorld = mScale * mRot * mTran;

	// シェーダー側に渡すコンスタントバッファ宣言
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	// コンスタントバッファのデータ書き換え
	auto camera = &Camera::GetInstance();
	const Color& color = pTexture->m_Color;
	DirectX::XMMATRIX m = mWorld * camera->GetViewMatrix()*camera->GetProjMatrix();
	
	cb.m_WVP		= m;				// ワールド行列
	cb.m_DivNum		= pTexture->GetDivNum();
	cb.m_Index		= pTexture->GetAtlasIndex();
	cb.m_Color		= color.GetRGBA();

	// UpdateSubResource
	Direct3D11::GetInstance().GetImmediateContext()->UpdateSubresource(
		shaderData->m_pConstantBuffer.Get(),
		NULL,
		nullptr,
		&cb,
		NULL,
		NULL
	);



	// ブレンドステートの設定
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	// 描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,		// 頂点数(板ポリゴンなので頂点数は4つ)
		NULL
	);


	return S_OK;
}

/*!
	@fn			タイリング描画
	@brief		テクスチャをタイリングして描画
	@detail		頂点情報のUVを範囲外にすることで実現
				描画サイズはテクスチャのサイズに依存する
	@param[in]	描画するテクスチャ
	@param[in]	タイル表示する割合
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::RenderTile(Texture * pTexture, const DirectX::XMFLOAT2 ratio)
{
	HRESULT hr;

	const auto size = pTexture->GetSize();

	// 頂点生成
	hr = CreateTilingVertex(size, ratio);
	if (FAILED(hr)) {
		std::string error = "Create tile vertex is failed!";
		ErrorLog(error);
		return E_FAIL;
	}

	// テクスチャのサイズをキャッシュしておく
	m_Size = size;

	// トポロジーセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	auto shaderData = ShaderManager::GetInstance().GetShaderData(ShaderManager::c_szTextureAtlasShader);

	// 頂点インプットレイアウトセット
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		shaderData->m_pVertexLayout.Get()
	);

	// シェーダーの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		shaderData->m_pVertexShader.Get(),
		NULL,
		NULL
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		shaderData->m_pPixelShader.Get(),
		NULL,
		NULL
	);

	// コンスタントバッファの登録
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shaderData->m_pConstantBuffer.GetAddressOf()
	);

	// サンプラー取得
	auto ppSampler = pTexture->GetSamplerState();

	// SRV取得
	auto ppSRV = pTexture->GetShaderResourceView();

	// テクスチャ
	Direct3D11::GetInstance().GetImmediateContext()->PSSetSamplers(
		0,
		1,
		ppSampler
	);
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShaderResources(
		0,
		1,
		ppSRV
	);

	// 座標変換
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, c_ScaleZ);

	// ワールド変換
	mWorld = mScale * mRot * mTran;

	// シェーダー側に渡すコンスタントバッファ宣言
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	// コンスタントバッファのデータ書き換え
	auto camera = &Camera::GetInstance();
	const Color& color = pTexture->m_Color;
	DirectX::XMMATRIX m = mWorld * camera->GetViewMatrix()*camera->GetProjMatrix();

	cb.m_WVP = m;					// ワールド行列
	cb.m_DivNum = { 1,1 };
	cb.m_Index = { 0,0 };
	cb.m_Color = color.GetRGBA();

	// UpdateSubResource
	Direct3D11::GetInstance().GetImmediateContext()->UpdateSubresource(
		shaderData->m_pConstantBuffer.Get(),
		NULL,
		nullptr,
		&cb,
		NULL,
		NULL
	);

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

	// ブレンドステートの設定
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	// 描画
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,		// 頂点数(板ポリゴンなので頂点数は4つ)
		NULL
	);

	return S_OK;
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
	leftTop.x		= -0.5f*size.x / c_NormalizeSize;// 左
	rightBottom.x	=  0.5f*size.x / c_NormalizeSize;// 右
	leftTop.y		=  0.5f*size.y / c_NormalizeSize;// 上
	rightBottom.y	= -0.5f*size.y / c_NormalizeSize;// 下

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
	@fn			タイリングテクスチャの頂点生成
	@brief		タイリング用の頂点生成
	@detail		タイリング用のUVを拡張した頂点を生成する
	@param[in]	画像サイズ
	@param[in]	タイリングする割合
	@return		成功:S_OK 失敗:E_FAIL
*/
HRESULT API::Sprite::CreateTilingVertex(DirectX::XMINT2 size, DirectX::XMFLOAT2 ratio)
{
	// 頂点宣言
	DirectX::XMFLOAT2 leftTop, rightBottom;			// 頂点座標
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;		// UV座標


#ifdef DEBUG_SPRITE
	// タイリング無し
	if (ratio.x == 1 && ratio.y == 1) {
		std::string error = "タイリングする必要がありません。";
		ErrorLog(error);
		return E_FAIL;
	}

	// 不正な値
	if (ratio.x <= 0 || ratio.y <= 0) {
		std::string error = "ratio is invalid value!";
		ErrorLog(error);
		return E_FAIL;
	}

#endif // DEBUG_SPRITE


	// UV定義
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = ratio.x;
	uvRightBottom.y = ratio.y;

	// 各頂点定義
	leftTop.x		= -0.5f*size.x / c_NormalizeSize * ratio.x;// 左
	rightBottom.x	=  0.5f*size.x / c_NormalizeSize * ratio.x;// 右
	leftTop.y		=  0.5f*size.y / c_NormalizeSize * ratio.y;// 上
	rightBottom.y	= -0.5f*size.y / c_NormalizeSize * ratio.y;// 下

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
	return S_OK;
}


/*!
	@brief	座標のセッター
*/
void Sprite::SetPos(DirectX::XMFLOAT3 pos)
{
#ifdef DEBUG_SPRITE
	// クリップ距離のオフセット(float型の丸め誤差対策)
	const float offset = 0.1f;

	// クリップ距離(z)が描画範囲外なら警告
	float nearClip = Camera::GetInstance().GetEyePt().z + Camera::c_NearClip - offset;
	float farClip = Camera::GetInstance().GetEyePt().z + Camera::c_FarClip - offset;

	try
	{
		std::string error = "スプライトはカメラのクリップの描画範囲外のため描画されません。\n";
		if (pos.z < nearClip) {

			error += "\nNearClip > Pos.z\n";
			error += std::to_string(nearClip + offset) + " > " + std::to_string(pos.z);
			error += "\n\nスプライトはカメラより手前にあります";
			throw error;
		}
		if (pos.z >= farClip) {

			error += "\nFarClip <= Pos.z\n";
			error += std::to_string(farClip - offset) + " <= " + std::to_string(pos.z);
			error += "\n\nスプライトはカメラの描画範囲より奥にあります";
			throw error;
		}
	}
	catch (std::string error)
	{
		ErrorLog(error);
		int ret = 0;
		if (ret == IDOK) {
			exit(NULL);
		}
	}

	
#endif // DEBUG_SPRITE
	m_Pos = pos;
}

/*!
	@brief	座標のセッター
	@detail	オーバーロード
*/
void Sprite::SetPos(DirectX::XMFLOAT2 pos)
{
	m_Pos = { pos.x,pos.y,m_Pos.z };
}

/*!
	@brief	スケールのセッター
*/
void API::Sprite::SetScale(DirectX::XMFLOAT2 scale)
{
	m_Scale = { scale.x,scale.y,c_ScaleZ };
}

