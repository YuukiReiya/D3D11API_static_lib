/*
	@file	Sprite.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�X�v���C�g
	@detail	�Q�l:http://zerogram.info/?p=2029
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
	@brief	���O���
	@detail	using�f�B���N�e�B�u
*/
using namespace D3D11;
using namespace D3D11::Graphic;
using namespace API;

/*!
	@brief	�R���X�g���N�^
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
	@brief	�f�X�g���N�^
*/
Sprite::~Sprite()
{
	Finalize();
}

/*!
	@fn		�C�j�V�����C�Y
	@brief	������
	@detail	�u�����h�X�e�[�g�̍쐬���s��
*/
HRESULT Sprite::Initialize()
{
	HRESULT hr;

	/*!
	��
	SRC:�\�[�X(���ꂩ��`�悷��s�N�Z���̐F)
	DEST:�f�B�X�g�l�[�V����(�����_�����O�^�[�Q�b�g�ɕ`�悳��Ă���s�N�Z���̐F)
	
	�ŏI�I�ȕ`��F�͈ȉ��́u�����֐��v�ɂ���Č��܂�
	
		SRC �~ �u�����f�B���O�W�� �{ DEST �~ �u�����f�B���O�W��
	
		SRCALPHA:	 SRC �̃��l
		INVSRCALPHA: 1 - SRC �̃��l
		DESTALPHA:	 DEST�̃��l
	*/

	// ���u�����h
	// ���e�X�g�ݒ�
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));

	// �u�����h�̗L���E����
	bd.RenderTarget[0].BlendEnable		= true;

	// �u�����f�B���O�W���̐ݒ�
	bd.RenderTarget[0].SrcBlend			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend		= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	//
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;


	// �u�����h�I�v�V����
	bd.RenderTarget[0].BlendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	//
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	// �A���`�G�C���A�X����
	//bd.AlphaToCoverageEnable = true;	// �؂����������ɑ΂���A���`�G�C���A�X�����̗L��
	bd.IndependentBlendEnable = false;

	// �u�����h�X�e�[�g�̍쐬
	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
		&bd,
		m_pBlendState.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::string error = "BlendState is not create!";
		ErrorLog(error);
		return E_FAIL;
	}

	// �u�����h�X�e�[�g�̐ݒ�
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);

	return S_OK;
}

/*!
	@fn		�t�@�C�i���C�Y
	@brief	�j������
	@detail	�����o�̖����I�ȉ���ƃ����o�̏�����
*/
void Sprite::Finalize()
{
	// �����o�̏�����
	//m_Pos = { 0,0,0 };
	//m_Scale = { 1,1 ,0 };
	//m_Rot = { 0,0,0 };
	m_Size = { -1,-1 };
	m_StencilMask = 0xffffffff;

	// �J��
	m_pVertexBuffer.Reset();
	m_pBlendState.Reset();
}

/*!
	@fn		Render
	@brief	�`��
*/
void API::Sprite::Render()
{
#if defined DEBUG_SPRITE

	//	�V�F�[�_�[�̎Q�Ɛ؂�
	if (m_pShader.expired()) {
		ErrorLog("The \"m_pShader\" value referenced in this class is invalid!");
		return;
	}

	//	�e�N�X�`���̎Q�Ɛ؂�
	if (m_pTexture.expired()) {
		ErrorLog("The \"m_pTexture\" value referenced in this class is invalid!");
		return;
	}
#endif

	//	�g�|���W�[�̃Z�b�g
	SetupTopology();

	//	���_���C�A�E�g�̃Z�b�g
	SetupInputLayout();

	//	�V�F�[�_�[�̃Z�b�g
	SetupBindShader();

	//	�T���v���[�X�e�[�g�̃Z�b�g
	SetupSampler();

	//	ShaderResourceView�̃Z�b�g
	SetupSRV();

	//	�R���X�^���g�o�b�t�@�̃Z�b�g
	SetupConstantBuffer();

	//	�u�����h�X�e�[�g�̃Z�b�g
	SetupBlendState();

	//	�`��
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,		// ���_��(�|���S���Ȃ̂Œ��_����4��)
		NULL
	);
}

/*!
	@fn			SetupBlendPreset
	@brief		�w�肵���v���Z�b�g�̃u�����h�X�e�[�g�������o�ɐݒ肷��
	@param[in]	�w�肷��v���Z�b�g�̗񋓑�
*/
void API::Sprite::SetupBlendPreset(BlendPreset preset)
{
	/*!
	��
	SRC:�\�[�X(���ꂩ��`�悷��s�N�Z���̐F)
	DEST:�f�B�X�g�l�[�V����(�����_�����O�^�[�Q�b�g�ɕ`�悳��Ă���s�N�Z���̐F)

	�ŏI�I�ȕ`��F�͈ȉ��́u�����֐��v�ɂ���Č��܂�

		SRC �~ �u�����f�B���O�W�� �{ DEST �~ �u�����f�B���O�W��

		SRCALPHA:	 SRC �̃��l
		INVSRCALPHA: 1 - SRC �̃��l
		DESTALPHA:	 DEST�̃��l


		Cr = Cd * (1 - As) + Cs * As

		   = Cs * As + Cd * (1 - As)
	*/

	//	��������������A�Đݒ肷��
	if (m_pBlendState != nullptr) {
		m_pBlendState.Reset();
	}

	// ���u�����h
	// ���e�X�g�ݒ�
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));

	// �u�����h�̗L���E����
	bd.RenderTarget[0].BlendEnable = preset != BlendPreset::Default;

	//	�u�����f�B���O�W��
	D3D11_BLEND src, dest, srcAlpha, destAlpha;

	//	�u�����f�B���O�I�v�V����
	D3D11_BLEND_OP blendOp, blendOpAlpha;

	//	�v���Z�b�g�ŕ���
	switch (preset)
	{
		//	�f�t�H���g
		case API::Sprite::BlendPreset::Default:
		{
			//	�W��
			src			= D3D11_BLEND::D3D11_BLEND_ONE;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ZERO;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	�I�v�V����
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
		break;

		//	���`
		case API::Sprite::BlendPreset::Linear:
		{
			//	�W��
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	�I�v�V����
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	���Z
		case API::Sprite::BlendPreset::Add:
		{
			//	�W��
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	�I�v�V����
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	���Z
		case API::Sprite::BlendPreset::Subtraction:
		{
			//	�W��
			src			= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_ONE;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	�I�v�V����
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	��Z
		case API::Sprite::BlendPreset::Multiple:
		{
			//	�W��
			src			= D3D11_BLEND::D3D11_BLEND_ZERO;
			srcAlpha	= D3D11_BLEND::D3D11_BLEND_ONE;
			dest		= D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
			destAlpha	= D3D11_BLEND::D3D11_BLEND_ZERO;

			//	�I�v�V����
			blendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		}
			break;

		//	��O����
		default:
			std::string error = "Invalid value for argument of SetupBlendPreset function!";
			ErrorLog(error);
			return;
	}
	
	//	�W��
	bd.RenderTarget[0].SrcBlend			= src;
	bd.RenderTarget[0].DestBlend		= dest;
	bd.RenderTarget[0].SrcBlendAlpha	= srcAlpha;
	bd.RenderTarget[0].DestBlendAlpha	= destAlpha;

	// �u�����h�I�v�V����
	bd.RenderTarget[0].BlendOp		= blendOp;
	bd.RenderTarget[0].BlendOpAlpha = blendOpAlpha;

	//	RGBA �S�Ă��u�����h����
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	// �A���`�G�C���A�X����
	bd.IndependentBlendEnable = false;

	HRESULT hr;

	// �u�����h�X�e�[�g�̍쐬
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
	@brief		�e�N�X�`���̐ݒ�
	@detail		��Q�ƂŃo�C���h���A���̎��_�Œ��_�������s��
	@param[in]	�o�^����e�N�X�`���̃|�C���^
*/
void API::Sprite::SetupTexture(Texture*  texture)
{
	m_pTexture = texture->GetSharedPtr();
	m_Size = texture->GetSize();

	//	���_����
	SetupVertex();

	//	�����ς̒��_���璸�_�o�b�t�@��ݒ�
	SetupVertexBuffer();
}

/*!
	@fn			SetupShader
	@brief		�V�F�[�_�[�̐ݒ�
	@detail		��Q�ƂŃo�C���h����
	@param[in]	�o�^����V�F�[�_�[�̃|�C���^
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
	@fn			���_����
	@brief		�X�v���C�g�̒��_�𐶐�����
	@detail		���_�\���̂��g���āA���𒸓_�o�b�t�@�Ƀo�C���h
	@param[in]	�摜�T�C�Y
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT Sprite::CreateVertex(DirectX::XMINT2 size)
{
	// ���_�錾
	DirectX::XMFLOAT2 leftTop, rightBottom;			// ���_���W
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;		// UV���W

	// �e���_��`
	leftTop.x		= -0.5f*size.x / c_DefaultSize;// ��
	rightBottom.x	=  0.5f*size.x / c_DefaultSize;// �E
	leftTop.y		=  0.5f*size.y / c_DefaultSize;// ��
	rightBottom.y	= -0.5f*size.y / c_DefaultSize;// ��

	// UV��`
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = uvRightBottom.y = 1;

	// ���_�\���̒�`
	SpriteVertex vertices[] = {
		// �E��
		{
			// ���_
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				c_VertexZ
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvRightBottom.x,
				uvLeftTop.y
			),
	},
		// �E��
		{
			// ���_
			DirectX::XMFLOAT3(
				rightBottom.x,
				rightBottom.y,
				c_VertexZ
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvRightBottom.x,
				uvRightBottom.y
			),
	},
		// ����
		{
			// ���_
			DirectX::XMFLOAT3(
				leftTop.x,
				leftTop.y,
				c_VertexZ
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvLeftTop.x,
				uvLeftTop.y
			),
	},
		// ����
		{
			// ���_
			DirectX::XMFLOAT3(
				leftTop.x,
				rightBottom.y,
				c_VertexZ
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvLeftTop.x,
				uvRightBottom.y
			),
	}
	};

	// �|���S��(�l�p�`�|���S��)�̃o�b�t�@���`
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPU����ǂݍ��݂Ə������݂�����
	bd.ByteWidth = sizeof(vertices);			// �o�b�t�@�̃T�C�Y
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h

	// �T�u���\�[�X�̃f�[�^���`
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// �������f�[�^�ւ̃|�C���^

	// ���_�o�b�t�@�̊J��
	m_pVertexBuffer.Reset();

	HRESULT hr;

	// ���_�o�b�t�@����
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

	// ���_�o�b�t�@�Z�b�g
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
	@brief		���_�̐ݒ�
*/
void API::Sprite::SetupVertex()
{
	// ���_�錾
	DirectX::XMFLOAT2 leftTop, rightBottom;				// ���_���W
	DirectX::XMFLOAT2 uvLeftTop, uvRightBottom;			// UV���W

	// �e���_��`
	leftTop.x		= -0.5f*m_Size.x / c_DefaultSize;	// ��
	rightBottom.x	=  0.5f*m_Size.x / c_DefaultSize;	// �E
	leftTop.y		=  0.5f*m_Size.y / c_DefaultSize;	// ��
	rightBottom.y	= -0.5f*m_Size.y / c_DefaultSize;	// ��

	// UV��`
	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = uvRightBottom.y = 1;

	// ���_�\���̒�`
	SpriteVertex vertices[] = {
		// �E��
		{
			// ���_
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				c_VertexZ
			),
		// UV���W
		DirectX::XMFLOAT2(
			uvRightBottom.x,
			uvLeftTop.y
		),
},
// �E��
{
	// ���_
	DirectX::XMFLOAT3(
		rightBottom.x,
		rightBottom.y,
		c_VertexZ
	),
		// UV���W
		DirectX::XMFLOAT2(
			uvRightBottom.x,
			uvRightBottom.y
		),
},
// ����
{
	// ���_
	DirectX::XMFLOAT3(
		leftTop.x,
		leftTop.y,
		c_VertexZ
	),
		// UV���W
		DirectX::XMFLOAT2(
			uvLeftTop.x,
			uvLeftTop.y
		),
},
// ����
{
	// ���_
	DirectX::XMFLOAT3(
		leftTop.x,
		rightBottom.y,
		c_VertexZ
	),
		// UV���W
		DirectX::XMFLOAT2(
			uvLeftTop.x,
			uvRightBottom.y
		),
}
	};

	// �|���S��(�l�p�`�|���S��)�̃o�b�t�@���`
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPU����ǂݍ��݂Ə������݂�����
	bd.ByteWidth = sizeof(vertices);			// �o�b�t�@�̃T�C�Y
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h

	// �T�u���\�[�X�̃f�[�^���`
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// �������f�[�^�ւ̃|�C���^

	// ���_�o�b�t�@�̊J��
	m_pVertexBuffer.Reset();

	HRESULT hr;

	// ���_�o�b�t�@����
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

	// ���_�o�b�t�@�Z�b�g
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
	@brief	�g�|���W�[�̐ݒ�
	@NOTE	�X�v���C�g�͔|���S�������Ȃ̂ŁA����������TRIANGLESTRIP���w��
*/
void API::Sprite::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
}

/*!
	@fn		CreateInputLayout
	@brief	���_���C�A�E�g�̐ݒ�
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
	@brief	�V�F�[�_�[�̐ݒ�
*/
void API::Sprite::SetupBindShader()
{
	auto shader = *m_pShader.lock();

	//	���_�V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->VSSetShader(
		*shader->GetVertexShaderPtr(),
		NULL,
		NULL
	);

	//	�s�N�Z���V�F�[�_�[
	Direct3D11::GetInstance().GetImmediateContext()->PSSetShader(
		*shader->GetPixelShaderPtr(),
		NULL,
		NULL
	);

}

/*!
	@fn		SetupSampler
	@brief	�T���v���[�X�e�[�g�̐ݒ�
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
	@brief	ShaderResourceView�̐ݒ�
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
	@brief	�R���X�^���g�o�b�t�@�̐ݒ�
*/
void API::Sprite::SetupConstantBuffer()
{
	auto shader = *m_pShader.lock();
	auto& device = Direct3D11::GetInstance();

	//	���[���h�s��
	//DirectX::XMMATRIX world = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
	//	*
	//	DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
	//	*
	//	DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);

	DirectX::XMMATRIX world = transform->GetMatrix();

	//	�r���[�s��
	DirectX::XMMATRIX view = Camera::GetInstance().GetViewMatrix();

	//	�v���W�F�N�V�����s��
	DirectX::XMMATRIX proj = Camera::GetInstance().GetProjMatrix();

	//	���_�V�F�[�_�[�p��C�o�b�t�@�o�^
	Direct3D11::GetInstance().GetImmediateContext()->VSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBufferPtr()
	);

	//	�s�N�Z���V�F�[�_�[�p��C�o�b�t�@�o�^
	Direct3D11::GetInstance().GetImmediateContext()->PSSetConstantBuffers(
		0,
		1,
		shader->GetConstantBufferPtr()
	);

	//	�}�b�s���O�p�ϐ��̐錾
	D3D11_MAPPED_SUBRESOURCE pMapData;

	//	�o�b�t�@�ւ̃A�N�Z�X(��������)����
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
		//	�A�N�Z�X������Ĕ�����
		device.GetImmediateContext()->Unmap(*shader->GetConstantBufferPtr(), NULL);
		return;
	}

	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	auto ptex = *m_pTexture.lock();

	//	�o�b�t�@�ɑ��
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

	//	�������R�s�[
	memcpy_s(pMapData.pData, pMapData.RowPitch, (void*)(&cb), sizeof(cb));

	//	�A�N�Z�X���I��
	device.GetImmediateContext()->Unmap(
		*shader->GetConstantBufferPtr(),
		NULL
	);
}

/*!
	@fn		CreateVertexBuffer
	@brief	���_�o�b�t�@�ݒ�
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
	@brief	�u�����h�X�e�[�g��ݒ�
*/
void API::Sprite::SetupBlendState()
{
	Direct3D11::GetInstance().GetImmediateContext()->OMSetBlendState(
		m_pBlendState.Get(),
		NULL,
		m_StencilMask
	);
}