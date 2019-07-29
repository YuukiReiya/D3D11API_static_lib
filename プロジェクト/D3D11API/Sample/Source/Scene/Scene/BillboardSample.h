#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <Sprite.h>
#include <Mesh.h>

class BillboardSample
	:public IScene
{
public:
	/*!
		@brief	�R���X�g���N�^	
	*/
	explicit BillboardSample();

	/*!
		@brief	�f�X�g���N�^
	*/
	~BillboardSample();

	/*!
		@fn		Initialize
		@brief	����������
	*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	�j������
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	�X�V����
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	�`�揈��
	*/
	void IScene::Render();

	/*!
		@fn		Reset
		@brief	���Z�b�g
		@detail	�f�[�^(���W���]���)�̏�����
	*/
	void Reset();
private:
	enum EyeType
	{
		FPS,
		TPS,
	};

	EyeType m_eEyeType;
	DirectX::XMFLOAT3 m_CameraPos;
	DirectX::XMFLOAT3 m_MeshRot;
	std::shared_ptr<API::Mesh>m_pMesh;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pMeshShader;
	std::shared_ptr<API::Material>m_pMeshMaterial;
	std::shared_ptr<API::Texture>m_pTexture;
	std::shared_ptr<API::Sprite>m_pSprite;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
	std::shared_ptr<Transform>m_pBillboradTarget;
};
