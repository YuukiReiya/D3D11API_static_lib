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
#include "MemoryLeaks.h"
#include "MyGame.h"
#include "Debug.h"
#include "SpriteConstantBuffer.h"
#include "SpriteCompVS.h"
#include "SpriteCompPS.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@struct	Vertex
	@brief	�X�v���C�g�̒��_�f�[�^�\����
*/
struct Vertex
{
	/*!
		@var	position
		@brief	���W
	*/
	DirectX::XMFLOAT3 position;

	/*!
		@var	uv
		@brief	UV
	*/
	DirectX::XMFLOAT2 uv;
};

/*!
	@brief	�R���X�g���N�^
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
	@brief	�f�X�g���N�^
*/
API::Sprite::~Sprite()
{
	Finalize();
}

/*!
	@fn			Initialize
	@brief		������
	@param[in]	�e�N�X�`��
	@return		����:S_OK ���s:E_FAIL
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

	//	���_�o�b�t�@����
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
	@fn		�t�@�C�i���C�Y
	@brief	�j������
	@detail	�����o�̖����I�ȉ���ƃ����o�̏�����
*/
void API::Sprite::Finalize()
{
	//	�����o�̏�����
	m_StencilMask = 0xffffffff;

	//	�J��
	m_pShader.reset();
	m_pTexture.reset();
	m_pBlendState.Reset();
	m_pVertexBuffer.Reset();
}

/*!
	@fn		Render
	@brief	�`��
*/
void API::Sprite::Render()
{
	//�g�|���W�[
	SetupTopology();

	//	�E�B�[�N�|�C���^�ɗL���ȎQ�Ƃ������Ă��邩�m�F
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

	//	�V�F�[�_�[
	auto shader = *m_pShader.lock();

	//	���_���C�A�E�g
	SetupInputLayout(shader);

	//	�V�F�[�_�[�̃o�C���h
	SetupBindShader(shader);

	//	�R���X�^���g�o�b�t�@
	SetupConstantBuffer(shader);

	//	���_�o�b�t�@�Z�b�g
	SetupVertexBuffer();

	//	�e�N�X�`��
	auto texture = *m_pTexture.lock();

	//�T���v���[
	SetupSampler(texture);

	//SRV
	SetupSRV(texture);

	//�u�����h�X�e�[�g
	SetupBlendState();

	//�`��
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,
		NULL
	);
}

void API::Sprite::RenderBillboard()
{
	//�g�|���W�[
	SetupTopology();

	//	�E�B�[�N�|�C���^�ɗL���ȎQ�Ƃ������Ă��邩�m�F
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

	//	�V�F�[�_�[
	auto shader = *m_pShader.lock();

	//	���_���C�A�E�g
	SetupInputLayout(shader);

	//	�V�F�[�_�[�̃o�C���h
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

	//	�R���X�^���g�o�b�t�@
	SetupConstantBuffer(
		shader,
		billboardMat*transform->GetWorldMatrix(),
		//billboardMat.GetWorldMatrix(),
		//billboardMat,
		Camera::GetInstance().GetViewMatrix(),
		Camera::GetInstance().GetProjMatrix()
		);

	//	���_�o�b�t�@�Z�b�g
	SetupVertexBuffer();

	//	�e�N�X�`��
	auto texture = *m_pTexture.lock();

	//�T���v���[
	SetupSampler(texture);

	//SRV
	SetupSRV(texture);

	//�u�����h�X�e�[�g
	SetupBlendState();

	//�`��
	Direct3D11::GetInstance().GetImmediateContext()->Draw(
		4,
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
		src = D3D11_BLEND::D3D11_BLEND_ONE;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ZERO;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	�I�v�V����
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	���`
	case API::Sprite::BlendPreset::Linear:
	{
		//	�W��
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	�I�v�V����
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	���Z
	case API::Sprite::BlendPreset::Add:
	{
		//	�W��
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ONE;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	�I�v�V����
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	���Z
	case API::Sprite::BlendPreset::Subtraction:
	{
		//	�W��
		src = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_ONE;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	�I�v�V����
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	��Z
	case API::Sprite::BlendPreset::Multiple:
	{
		//	�W��
		src = D3D11_BLEND::D3D11_BLEND_ZERO;
		srcAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		dest = D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
		destAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		//	�I�v�V����
		blendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}
	break;

	//	��O����
	default:
		std::string error = "Invalid value for argument of SetupBlendPreset function!";
		ErrorLog(error);
		return;
	}

	//	�W��
	bd.RenderTarget[0].SrcBlend = src;
	bd.RenderTarget[0].DestBlend = dest;
	bd.RenderTarget[0].SrcBlendAlpha = srcAlpha;
	bd.RenderTarget[0].DestBlendAlpha = destAlpha;

	// �u�����h�I�v�V����
	bd.RenderTarget[0].BlendOp = blendOp;
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
void API::Sprite::SetupTexture(Texture * texture)
{
	m_pTexture = texture->GetSharedPtr();
}

/*!
	@fn			SetupShader
	@brief		�V�F�[�_�[�̐ݒ�
	@detail		��Q�ƂŃo�C���h����
	@param[in]	�o�^����V�F�[�_�[�̃|�C���^
*/
void API::Sprite::SetupShader(D3D11::Graphic::AbstractShader * shader)
{
	m_pShader = shader->GetSharedPtr();
}

/*!
	@fn		SetupTopology
	@brief	�g�|���W�[�̐ݒ�
	@NOTE	�X�v���C�g�͔|���S�������Ȃ̂ŁA����������TRIANGLESTRIP���w��
*/
void API::Sprite::SetupTopology()
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
}

/*!
	@fn			SetupInputLayout
	@brief		���_���C�A�E�g�̐ݒ�
	@param[in]	�V�F�[�_�[
	@note		��Q�ƂŎ擾�����|�C���^����擾
*/
void API::Sprite::SetupInputLayout(D3D11::Graphic::AbstractShader * shader)
{
	Direct3D11::GetInstance().GetImmediateContext()->IASetInputLayout(
		*shader->GetInputLayout()
	);
}

/*!
	@fn			SetupBindShader
	@brief		�V�F�[�_�[�̃o�C���h
	@param[in]	�V�F�[�_�[
	@note		��Q�ƂŎ擾�����|�C���^����擾
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
	@brief		�R���X�^���g�o�b�t�@�̐ݒ�
	@param[in]	�V�F�[�_�[
	@note		��Q�ƂŎ擾�����|�C���^����擾
*/
void API::Sprite::SetupConstantBuffer(D3D11::Graphic::AbstractShader * shader)
{
	//	�}�b�s���O�p�ϐ��̐錾
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	//	�o�b�t�@�ւ̃A�N�Z�X(��������)����
	HRESULT hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		*shader->GetConstantBuffer(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	if (FAILED(hr)) {
		//	�A�N�Z�X������Ĕ�����
		context->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	//	�萔�o�b�t�@�錾
	D3D11::Graphic::Sprite::ConstantBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	WVP�s��
	DirectX::XMMATRIX w, v, p;
	w = transform->GetWorldMatrix();
	v = Camera::GetInstance().GetViewMatrix();
	p = Camera::GetInstance().GetProjMatrix();

	//	�萔�o�b�t�@�p�̕ϐ��Ɏ󂯓n��
	cb.matrix.world = DirectX::XMMatrixTranspose(w);
	cb.matrix.view = DirectX::XMMatrixTranspose(v);
	cb.matrix.proj = DirectX::XMMatrixTranspose(p);
	cb.color = color.GetRGBA();

	//	�������R�s�[
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	
	//	�A�N�Z�X���I��
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);

	//	�V�F�[�_�[�ɒ萔�o�b�t�@���o�C���h
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
}

/*!
	@fn			SetupConstantBuffer
	@brief		�R���X�^���g�o�b�t�@�̐ݒ�
	@param[in]	�V�F�[�_�[
	@param[in]	���[���h�s��
	@param[in]	�r���[�s��
	@param[in]	�v���W�F�N�V�����s��
	@note		��Q�ƂŎ擾�����|�C���^����擾
*/
void API::Sprite::SetupConstantBuffer(D3D11::Graphic::AbstractShader * shader, DirectX::XMMATRIX w, DirectX::XMMATRIX v, DirectX::XMMATRIX p)
{
	//	�}�b�s���O�p�ϐ��̐錾
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	//	�o�b�t�@�ւ̃A�N�Z�X(��������)����
	HRESULT hr = Direct3D11::GetInstance().GetImmediateContext()->Map(
		*shader->GetConstantBuffer(),
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	auto context = Direct3D11::GetInstance().GetImmediateContext();
	if (FAILED(hr)) {
		//	�A�N�Z�X������Ĕ�����
		context->Unmap(*shader->GetConstantBuffer(), NULL);
		return;
	}

	//	�萔�o�b�t�@�錾
	D3D11::Graphic::Sprite::ConstantBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	//	�萔�o�b�t�@�p�̕ϐ��Ɏ󂯓n��
	cb.matrix.world = DirectX::XMMatrixTranspose(w);
	cb.matrix.view = DirectX::XMMatrixTranspose(v);
	cb.matrix.proj = DirectX::XMMatrixTranspose(p);
	cb.color = color.GetRGBA();

	//	�������R�s�[
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	//	�A�N�Z�X���I��
	Direct3D11::GetInstance().GetImmediateContext()->Unmap(
		*shader->GetConstantBuffer(),
		NULL
	);

	//	�V�F�[�_�[�ɒ萔�o�b�t�@���o�C���h
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer());
}

/*!
	@fn			SetupConstantBuffer
	@brief		�R���X�^���g�o�b�t�@�̐ݒ�
	@param[in]	�V�F�[�_�[
	@note		��Q�ƂŎ擾�����|�C���^����擾
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
	@brief		�T���v���[�X�e�[�g�̐ݒ�
	@param[in]	�e�N�X�`��
	@note		��Q�ƂŎ擾�����|�C���^����擾
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
	@brief		ShaderResourceView�̐ݒ�
	@param[in]	�e�N�X�`��
	@note		��Q�ƂŎ擾�����|�C���^����擾
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

/*!
	@fn			CreateVertexBuffer
	@brief		���_�̐���
	@detail		�摜�̃T�C�Y�ɍ��킹�Ĕ|���S���𐶐�����
	@param[in]	�摜�̃T�C�Y
	@return		����:S_OK ���s:E_FAIL
*/
HRESULT API::Sprite::CreateVertexBuffer(const DirectX::XMFLOAT2 size)
{
	//	�摜�䗦
	float x, y;
	x = size.x <= size.y ? size.x*c_NormalizeSize / size.y : c_NormalizeSize;
	y = size.x <= size.y ? c_NormalizeSize : size.y*c_NormalizeSize / size.x;

	//	���W
	DirectX::XMFLOAT2 lt, rb;//	����,�E��
	lt.x = -0.5f * x;//	��
	rb.x =  0.5f * x;//	�E
	lt.y =  0.5f * y;//	��
	rb.y = -0.5f * y;//	��

	//	UV
	DirectX::XMFLOAT2 uvLT, uvRT;//	����,�E��
	uvLT.x = uvLT.y = 0;
	uvRT.x = uvRT.y = 1;

	//	���_�錾
	Vertex vertices[] = {
		// �E��
		{
			// ���_
			DirectX::XMFLOAT3(
				rb.x,
				lt.y,
				0
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvRT.x,
				uvLT.y
			),
		},
		// �E��
		{
			// ���_
			DirectX::XMFLOAT3(
				rb.x,
				rb.y,
				0
			),
			
			// UV���W
			DirectX::XMFLOAT2(
				uvRT.x,
				uvRT.y
			),
		},
		// ����
		{
			// ���_
			DirectX::XMFLOAT3(
				lt.x,
				lt.y,
				0
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvLT.x,
				uvLT.y
			),
		},
		// ����
		{
			// ���_
			DirectX::XMFLOAT3(
				lt.x,
				rb.y,
				0
			),
			// UV���W
			DirectX::XMFLOAT2(
				uvLT.x,
				uvRT.y
			),
		}
	};
	
	//	�o�b�t�@�d�l
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				// GPU����ǂݍ��݂Ə������݂�����
	bd.ByteWidth = sizeof(vertices);			// �o�b�t�@�̃T�C�Y
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h

	//	�T�u���\�[�X
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			// �������f�[�^�ւ̃|�C���^

	//	���_�̊J��
	m_pVertexBuffer.Reset();

	//	���_�̐���
	return Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		m_pVertexBuffer.GetAddressOf()
	);
}
