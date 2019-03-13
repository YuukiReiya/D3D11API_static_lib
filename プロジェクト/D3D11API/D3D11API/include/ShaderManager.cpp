#include "stdafx.h"
#pragma comment(lib,"d3dCompiler.lib")
#include <d3dcompiler.h>

#include "Direct3D11.h"
#include "ShaderManager.h"
#include "Sprite.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace D3D11;
using namespace D3D11::Graphic;

/*!
	@var	c_szSimpleTextureShader
	@brief	通常のテクスチャで使うシェーダーを登録したハッシュ
	@value	"SIMPLE_TEXTURE"
*/
const std::string ShaderManager::c_szSimpleTextureShader = "SIMPLE_TEXTURE";/*!< シンプルテクスチャのシェーダー */

/*!
	@var	c_szTextureAtlasShader
	@brief	アトラステクスチャのシェーダーを登録したハッシュ
	@value	"ATLAS_TEXTURE"
*/
const std::string ShaderManager::c_szTextureAtlasShader = "ATLAS_TEXTURE";	/*!< アトラステクスチャのシェーダー */

/*!
	@var	c_DefaultMeshShader
	@brief	メッシュのデフォルト設定
*/
const std::string ShaderManager::c_DefaultMeshShader	 = "MESH_DEFAULT";	/*!< メッシュシェーダーのデフォルト設定 */

/*!
	@var	c_VertexProfile
	@brief	頂点シェーダーのプロファイル情報
*/
const std::string c_VertexProfile		= "vs";						/*!< 頂点シェーダー */

/*!
	@var	c_PixelProfile
	@brief	ピクセルシェーダーのプロファイル情報
*/
const std::string c_PixelProfile		= "ps";						/*!< ピクセルシェーダー */

/*!
	@var	c_GeometryProfile
	@brief	ジオメトリシェーダーのプロファイル情報
*/
const std::string c_GeometryProfile		= "gs";						/*!< ジオメトリシェーダー */

/*!
	@var	c_HullProfile
	@brief	ハルシェーダーのプロファイル情報
*/
const std::string c_HullProfile			= "hs";						/*!< ハルシェーダー */

/*!
	@var	c_DomainProfile
	@brief	ドメインシェーダーのプロファイル情報
*/
const std::string c_DomainProfile		= "ds";						/*!< ドメインシェーダー */

/*!
	@var	c_ComputeProfile
	@brief	コンピュートシェーダー
*/
const std::string c_ComputeProfile		= "cs";						/*!< コンピュートシェーダー */

/*!
	@var	c_ShaderModelStringArraySize
	@brief	シェーダーモデルの文字列サイズ
	@detail	シェーダーモデルの設定文字列(\0含む)の配列のサイズ ※上記Profileの文字列サイズ+終端文字分
*/
constexpr int c_ShaderModelStringArraySize	= 3;

/*!
	@brief	コンストラクタ
*/
ShaderManager::ShaderManager() 
{
}

/*!
	@brief	デストラクタ
*/
ShaderManager::~ShaderManager()
{
	Finalize();
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		通常テクスチャ/アトラステクスチャで使うシェーダーの生成
	@param[in]	シェーダー(HLSL)の含まれているディレクトリのパス
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT ShaderManager::Initialize(std::string directoryPath)
{
	HRESULT hr;
	ID3DBlob* pCompiledShader = NULL;// コンパイル用ブロブ

	// リソースデイレクトリーの設定
	m_DirectoryPath = directoryPath;


	// 単純テクスチャのシェーダー設定作成
	m_pAddDataRef = new ShaderData;
	{
		// バーテックスシェーダーの作成
		hr = MakeShader("DefaultSprite.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SimpleTextureShader\" vertex shader is not create!");
			return E_FAIL;
		}

		// 頂点インプットレイアウト定義
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);// ポインタ

		// 頂点インプットレイアウトを作成
		hr = Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			m_pAddDataRef->m_pVertexLayout.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLog("\"SimpleTextureShader\" input layout is not create!");
			return E_FAIL;
		}

		// ピクセルシェーダーの作成
		hr = ShaderManager::MakeShader("DefaultSprite.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SimpleTextureShader\" pixel shader is not create!");
			return E_FAIL;
		}

		// コンスタントバッファ定義
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		// コンスタントバッファ作成
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"SimpleTextureShader\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}

	}
	AddNewShaderData(c_szSimpleTextureShader, m_pAddDataRef);
	

	// アトラステクスチャのシェーダー設定作成
	m_pAddDataRef = new ShaderData;
	{
		/*!
		*	・インプットレイアウト
		*	・頂点シェーダー
		*	・ピクセルシェーダー
		*	この三つは単純なテクスチャのShaderと同じものを使う
		*/

		// インプットレイアウト
		m_pAddDataRef->m_pVertexLayout = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pVertexLayout;

		// 頂点シェーダー
		m_pAddDataRef->m_pVertexShader = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pVertexShader;

		// ピクセルシェーダー
		m_pAddDataRef->m_pPixelShader = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pPixelShader;

		// コンスタントバッファ定義
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = 0;						//updateSubResource
		cb.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;//updateSubResource

		// コンスタントバッファ作成
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"TextureAtlasShader\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}
	}
	AddNewShaderData(c_szTextureAtlasShader, m_pAddDataRef);

	// ブロブの解放
	pCompiledShader->Release();
	pCompiledShader = NULL;

	// メッシュ用のデフォルト設定
	{
	}

	return S_OK;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	明示的なメンバの開放処理
*/
void ShaderManager::Finalize()
{
	for (auto it : m_pShaderDataUMap) {
		delete it.second;
	}
	m_pShaderDataUMap.clear();
	m_pAddDataRef = nullptr;
}


/*!
	@fn			シェーダーデータの追加
	@brief		unordered_mapに作成したシェーダーデータを追加登録
	@param[in]	ハッシュ名
	@param[in]	登録するシェーダーデータの参照
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT ShaderManager::AddNewShaderData(std::string szKeyName, ShaderData * pNewShaderData)
{
	// 登録可能か判定
	bool isFound = m_pShaderDataUMap.find(szKeyName) == m_pShaderDataUMap.end();

	// 登録しようとしたキー名は既に登録済みのため追加しない
	if (!isFound) { return E_FAIL; }

	// キー名でリスト(map)に追加
	m_pShaderDataUMap[szKeyName] = pNewShaderData;

	return S_OK;
}

/*!
	@fn			シェーダー作成
	@detail		ShaderDataのID3D11Shaderを作成
	@param[in]	設定したディレクトリからのシェーダーファイルのパス(.hlsl)
	@param[in]	シェーダーのエントリーポイント
	@param[in]	シェーダーのプロファイル情報(小文字_バージョン) ex) vs_5_0
	@param[in]	ID3D11*****Shaderのポインタをキャストして渡す ex)(void**)pVertexShader
	@param[in]	コンパイル用のブロブをポインタで
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT ShaderManager::MakeShader(std::string szFileName, std::string szFuncName, std::string szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	// UNICODE、マルチバイト両対応用文字列変換
	std::string sFilePath = m_DirectoryPath + szFileName;

	// D3D11CompileFromFileの引数はマルチバイト
	auto tmp = To_WString(sFilePath);
	auto path = const_cast<LPCWSTR>(tmp.c_str());

	DWORD shaderFlags;
#ifdef DEBUG_SHADER
	shaderFlags = D3DCOMPILE_DEBUG;
#else
	shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	HRESULT hr;

	LPCSTR funcName		= szFuncName.c_str();
	LPCSTR profileName	= szProfileName.c_str();

	// ブロブのコンパイル
	ID3DBlob* pErrors = NULL;
	hr = D3DCompileFromFile(
		path,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName,
		profileName,
		shaderFlags,
		NULL,
		ppBlob,
		&pErrors
	);
	if (FAILED(hr)) {
		std::string bufferError = (char*)pErrors->GetBufferPointer();
		std::string error = "\"" + sFilePath + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}
	if (pErrors != NULL) {
		pErrors->Release();
		pErrors = NULL;
	}

	char szProfile[c_ShaderModelStringArraySize] = { 0 };

	// 終端文字を含まないバッファのメモリを変数にコピー
	strncpy_s(szProfile, profileName, (c_ShaderModelStringArraySize - 1));// 終端文字のバッファーを一つ確保しておく

	// 頂点シェーダー(Vertex Shader)
	if (strcmp(szProfile, c_VertexProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// ピクセルシェーダー(Pixel Shader)
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// ジオメトリシェーダー(Geometry Shader)
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// ハルシェーダー(Hull Shader)
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	// ドメインシェーダー(Domain Shader)
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// コンピュートシェーダー(Compute Shader)
	if (strcmp(szProfile, c_ComputeProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Compute Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	std::string error = "\"ProfileName\" is not ShaderModel!";
	ErrorLog(error);
	return E_FAIL;
}

/*!
	@brief	キー(文字列)から登録済みのシェーダー情報を取得する
	@detail	探索文字列から情報を取得することに失敗した場合NULLを返す
*/
ShaderData * ShaderManager::GetShaderData(std::string szKeyName)
{
	return m_pShaderDataUMap[szKeyName];
}
