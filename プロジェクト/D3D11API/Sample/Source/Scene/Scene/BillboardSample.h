#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <Sprite.h>

class BillboardSample
	:public IScene
{
public:
	/*!
		@brief	コンストラクタ	
	*/
	explicit BillboardSample();

	/*!
		@brief	デストラクタ
	*/
	~BillboardSample();

	/*!
		@fn		Initialize
		@brief	初期化処理
*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	破棄処理
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	描画処理
	*/
	void IScene::Render();
private:
	std::shared_ptr<API::Texture>m_pTexture;
	std::shared_ptr<API::Sprite>m_pSprite;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
	std::shared_ptr<Transform>m_pBillboradTarget;
};
