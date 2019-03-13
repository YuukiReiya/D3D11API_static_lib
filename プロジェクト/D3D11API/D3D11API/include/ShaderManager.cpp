#include "stdafx.h"
#pragma comment(lib,"d3dCompiler.lib")
#include <d3dcompiler.h>

#include "Direct3D11.h"
#include "ShaderManager.h"
#include "Sprite.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	���O���
	@detail	using�f�B���N�e�B�u
*/
using namespace D3D11;
using namespace D3D11::Graphic;

/*!
	@var	c_szSimpleTextureShader
	@brief	�ʏ�̃e�N�X�`���Ŏg���V�F�[�_�[��o�^�����n�b�V��
	@value	"SIMPLE_TEXTURE"
*/
const std::string ShaderManager::c_szSimpleTextureShader = "SIMPLE_TEXTURE";/*!< �V���v���e�N�X�`���̃V�F�[�_�[ */

/*!
	@var	c_szTextureAtlasShader
	@brief	�A�g���X�e�N�X�`���̃V�F�[�_�[��o�^�����n�b�V��
	@value	"ATLAS_TEXTURE"
*/
const std::string ShaderManager::c_szTextureAtlasShader = "ATLAS_TEXTURE";	/*!< �A�g���X�e�N�X�`���̃V�F�[�_�[ */

/*!
	@var	c_DefaultMeshShader
	@brief	���b�V���̃f�t�H���g�ݒ�
*/
const std::string ShaderManager::c_DefaultMeshShader	 = "MESH_DEFAULT";	/*!< ���b�V���V�F�[�_�[�̃f�t�H���g�ݒ� */

/*!
	@var	c_VertexProfile
	@brief	���_�V�F�[�_�[�̃v���t�@�C�����
*/
const std::string c_VertexProfile		= "vs";						/*!< ���_�V�F�[�_�[ */

/*!
	@var	c_PixelProfile
	@brief	�s�N�Z���V�F�[�_�[�̃v���t�@�C�����
*/
const std::string c_PixelProfile		= "ps";						/*!< �s�N�Z���V�F�[�_�[ */

/*!
	@var	c_GeometryProfile
	@brief	�W�I���g���V�F�[�_�[�̃v���t�@�C�����
*/
const std::string c_GeometryProfile		= "gs";						/*!< �W�I���g���V�F�[�_�[ */

/*!
	@var	c_HullProfile
	@brief	�n���V�F�[�_�[�̃v���t�@�C�����
*/
const std::string c_HullProfile			= "hs";						/*!< �n���V�F�[�_�[ */

/*!
	@var	c_DomainProfile
	@brief	�h���C���V�F�[�_�[�̃v���t�@�C�����
*/
const std::string c_DomainProfile		= "ds";						/*!< �h���C���V�F�[�_�[ */

/*!
	@var	c_ComputeProfile
	@brief	�R���s���[�g�V�F�[�_�[
*/
const std::string c_ComputeProfile		= "cs";						/*!< �R���s���[�g�V�F�[�_�[ */

/*!
	@var	c_ShaderModelStringArraySize
	@brief	�V�F�[�_�[���f���̕�����T�C�Y
	@detail	�V�F�[�_�[���f���̐ݒ蕶����(\0�܂�)�̔z��̃T�C�Y ����LProfile�̕�����T�C�Y+�I�[������
*/
constexpr int c_ShaderModelStringArraySize	= 3;

/*!
	@brief	�R���X�g���N�^
*/
ShaderManager::ShaderManager() 
{
}

/*!
	@brief	�f�X�g���N�^
*/
ShaderManager::~ShaderManager()
{
	Finalize();
}

/*!
	@fn			�C�j�V�����C�Y
	@brief		������
	@detail		�ʏ�e�N�X�`��/�A�g���X�e�N�X�`���Ŏg���V�F�[�_�[�̐���
	@param[in]	�V�F�[�_�[(HLSL)�̊܂܂�Ă���f�B���N�g���̃p�X
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT ShaderManager::Initialize(std::string directoryPath)
{
	HRESULT hr;
	ID3DBlob* pCompiledShader = NULL;// �R���p�C���p�u���u

	// ���\�[�X�f�C���N�g���[�̐ݒ�
	m_DirectoryPath = directoryPath;


	// �P���e�N�X�`���̃V�F�[�_�[�ݒ�쐬
	m_pAddDataRef = new ShaderData;
	{
		// �o�[�e�b�N�X�V�F�[�_�[�̍쐬
		hr = MakeShader("DefaultSprite.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SimpleTextureShader\" vertex shader is not create!");
			return E_FAIL;
		}

		// ���_�C���v�b�g���C�A�E�g��`
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);// �|�C���^

		// ���_�C���v�b�g���C�A�E�g���쐬
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

		// �s�N�Z���V�F�[�_�[�̍쐬
		hr = ShaderManager::MakeShader("DefaultSprite.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SimpleTextureShader\" pixel shader is not create!");
			return E_FAIL;
		}

		// �R���X�^���g�o�b�t�@��`
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		// �R���X�^���g�o�b�t�@�쐬
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"SimpleTextureShader\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}

	}
	AddNewShaderData(c_szSimpleTextureShader, m_pAddDataRef);
	

	// �A�g���X�e�N�X�`���̃V�F�[�_�[�ݒ�쐬
	m_pAddDataRef = new ShaderData;
	{
		/*!
		*	�E�C���v�b�g���C�A�E�g
		*	�E���_�V�F�[�_�[
		*	�E�s�N�Z���V�F�[�_�[
		*	���̎O�͒P���ȃe�N�X�`����Shader�Ɠ������̂��g��
		*/

		// �C���v�b�g���C�A�E�g
		m_pAddDataRef->m_pVertexLayout = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pVertexLayout;

		// ���_�V�F�[�_�[
		m_pAddDataRef->m_pVertexShader = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pVertexShader;

		// �s�N�Z���V�F�[�_�[
		m_pAddDataRef->m_pPixelShader = m_pShaderDataUMap[c_szSimpleTextureShader]->m_pPixelShader;

		// �R���X�^���g�o�b�t�@��`
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = 0;						//updateSubResource
		cb.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;//updateSubResource

		// �R���X�^���g�o�b�t�@�쐬
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"TextureAtlasShader\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}
	}
	AddNewShaderData(c_szTextureAtlasShader, m_pAddDataRef);

	// �u���u�̉��
	pCompiledShader->Release();
	pCompiledShader = NULL;

	// ���b�V���p�̃f�t�H���g�ݒ�
	{
	}

	return S_OK;
}

/*!
	@fn		�t�@�C�i���C�Y
	@brief	�j������
	@detail	�����I�ȃ����o�̊J������
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
	@fn			�V�F�[�_�[�f�[�^�̒ǉ�
	@brief		unordered_map�ɍ쐬�����V�F�[�_�[�f�[�^��ǉ��o�^
	@param[in]	�n�b�V����
	@param[in]	�o�^����V�F�[�_�[�f�[�^�̎Q��
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT ShaderManager::AddNewShaderData(std::string szKeyName, ShaderData * pNewShaderData)
{
	// �o�^�\������
	bool isFound = m_pShaderDataUMap.find(szKeyName) == m_pShaderDataUMap.end();

	// �o�^���悤�Ƃ����L�[���͊��ɓo�^�ς݂̂��ߒǉ����Ȃ�
	if (!isFound) { return E_FAIL; }

	// �L�[���Ń��X�g(map)�ɒǉ�
	m_pShaderDataUMap[szKeyName] = pNewShaderData;

	return S_OK;
}

/*!
	@fn			�V�F�[�_�[�쐬
	@detail		ShaderData��ID3D11Shader���쐬
	@param[in]	�ݒ肵���f�B���N�g������̃V�F�[�_�[�t�@�C���̃p�X(.hlsl)
	@param[in]	�V�F�[�_�[�̃G���g���[�|�C���g
	@param[in]	�V�F�[�_�[�̃v���t�@�C�����(������_�o�[�W����) ex) vs_5_0
	@param[in]	ID3D11*****Shader�̃|�C���^���L���X�g���ēn�� ex)(void**)pVertexShader
	@param[in]	�R���p�C���p�̃u���u���|�C���^��
	@return		S_OK:���� E_FAIL:���s
*/
HRESULT ShaderManager::MakeShader(std::string szFileName, std::string szFuncName, std::string szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	// UNICODE�A�}���`�o�C�g���Ή��p������ϊ�
	std::string sFilePath = m_DirectoryPath + szFileName;

	// D3D11CompileFromFile�̈����̓}���`�o�C�g
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

	// �u���u�̃R���p�C��
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

	// �I�[�������܂܂Ȃ��o�b�t�@�̃�������ϐ��ɃR�s�[
	strncpy_s(szProfile, profileName, (c_ShaderModelStringArraySize - 1));// �I�[�����̃o�b�t�@�[����m�ۂ��Ă���

	// ���_�V�F�[�_�[(Vertex Shader)
	if (strcmp(szProfile, c_VertexProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// �s�N�Z���V�F�[�_�[(Pixel Shader)
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// �W�I���g���V�F�[�_�[(Geometry Shader)
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// �n���V�F�[�_�[(Hull Shader)
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	// �h���C���V�F�[�_�[(Domain Shader)
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	// �R���s���[�g�V�F�[�_�[(Compute Shader)
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
	@brief	�L�[(������)����o�^�ς݂̃V�F�[�_�[�����擾����
	@detail	�T�������񂩂�����擾���邱�ƂɎ��s�����ꍇNULL��Ԃ�
*/
ShaderData * ShaderManager::GetShaderData(std::string szKeyName)
{
	return m_pShaderDataUMap[szKeyName];
}
