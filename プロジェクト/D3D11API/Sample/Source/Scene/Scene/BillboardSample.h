#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <Sprite.h>

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
private:
	std::shared_ptr<API::Texture>m_pTexture;
	std::shared_ptr<API::Sprite>m_pSprite;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
	std::shared_ptr<Transform>m_pBillboradTarget;
};
