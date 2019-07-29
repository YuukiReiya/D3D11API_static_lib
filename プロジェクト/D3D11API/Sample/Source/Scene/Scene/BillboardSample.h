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

	/*!
		@fn		Reset
		@brief	リセット
		@detail	データ(座標や回転情報)の初期化
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
