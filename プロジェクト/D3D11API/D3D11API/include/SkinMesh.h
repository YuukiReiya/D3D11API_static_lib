/*
	@file	SkinMesh.h
	@date	2019/08/14
	@author	番場 宥輝
	@brief	スキンアニメーションするメッシュクラス
*/
#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include "SkinnedVertex.h"
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "Transform.h"
#include "Joint.h"
#include "AnimationClip.h"
#include <algorithm>

namespace API
{
	class SkinMesh
		: public API::AbstractRender
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		explicit SkinMesh();

		/*!
			@brief	デストラクタ
		*/
		~SkinMesh();

		/*!
			@fn			Initialize
			@brief		初期化
			@param[in]	読み込み用のファイルパス
			@return		S_OK:成功 E_FAIL:失敗
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	描画
		*/
		void Render()override;

		/*!
			@var	transform
			@brief	トランスフォーム
		*/
		std::shared_ptr<Transform>transform;

		/*!
			@fn			AddAnimationFrame
			@brief		アニメーションフレームの加算
			@detail		現在参照しているフレーム番号を加算。
						※参照切れなら処理しない。
			@param[in]	true:アニメーション終了時に最初から。 false:アニメーション終了時そのまま
		*/
		void AddAnimationFrame(bool isLoop = true)
		{
			if (m_AnimClip.expired()) { return; }
			auto ptr = m_AnimClip.lock();
			frameIndex = frameIndex < ptr->matrixPalette[0].size() - 1 ? ++frameIndex : isLoop ? 0 : ptr->matrixPalette[0].size() - 1;
		}

		/*!
			@fn			SetupAnimationFrame
			@brief		アニメーションの参照するフレームを指定
			@detail		範囲外を設定されたら範囲内に丸める。
						※参照切れなら処理しない。
			@param[in]	参照するフレーム番号
		*/
		void SetupAnimationFrame(uint32_t frame)
		{
			if (m_AnimClip.expired()) { return; }
			auto ptr = m_AnimClip.lock();
			frameIndex = std::clamp(frame, static_cast<uint32_t>(0), static_cast<uint32_t>(ptr->matrixPalette[0].size() - 1));
		}

		/*!
			@fn			SetupAnimation
			@brief		アニメーションの設定
			@param[in]	登録するアニメーションクリップの共有ポインタ
		*/
		void SetupAnimation(std::shared_ptr<API::AnimationClip> clip) {
			m_AnimClip = clip;
		};
	private:
		/*!
			@enum	SkinningMode
			@brief	スキニング方法
		*/
		enum SkinningMode:BYTE
		{
			/*!
				@var	SOFTWARE
				@brief	ソフトウェア側でスキニング
			*/
			SOFTWARE,

			/*!
				@var	SHADER
				@brief	シェーダー側でスキニング

				@note	調整中(現在は全てソフトウェアスキニング)
			*/
			SHADER
		}
		/*!
			@var	m_eSkinningMode
			@brief	スキニング方法を保持するメンバ
			@detail	ロードした情報からスキニングのやり方を変える
		*/
		m_eSkinningMode;


		/*!
			@var	m_pIndexBuffer
			@brief	インデックスバッファ
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		
		/*!
			@var	m_pVertexBuffer
			@brief	頂点バッファ
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pShader
			@brief	Meshクラスで使用するシェーダーの弱参照
			@detail	weak_ptr
		*/
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
		
		/*!
			@var	m_Indices
			@brief	頂点インデックス
		*/
		std::vector<uint32_t>m_Indices;

		/*!
			@var	m_Vertices
			@brief	頂点
		*/
		std::vector<D3D11::Graphic::SkinnedVertex>m_Vertices;

		/*!
			@var	m_AnimClip
			@brief	アニメーションクリップの弱参照
		*/
		std::weak_ptr<API::AnimationClip>m_AnimClip;

		/*!
			@fn		SetupTopology
			@brief	トポロジーのセットアップ
		*/
		void SetupTopology();

		/*!
			@fn		SetupInputLayout
			@brief	頂点レイアウトのセットアップ
		*/
		void SetupInputLayout();

		/*!
			@fn		SetupVertexBuffer
			@brief	頂点バッファのセットアップ
		*/
		void SetupVertexBuffer();

		/*!
			@fn		SetupIndexBuffer
			@brief	インデックスバッファのセットアップ
		*/
		void SetupIndexBuffer();

		/*!
			@fn			Load
			@brief		ファイルの読み込み
			@param[in]	バインド先メッシュ
			@param[in]	ファイルのパス
			@return		true:成功 false:失敗
		*/
		static bool Load(SkinMesh*mesh,std::string filePath);

		/*!
			@fn		SoftwareSkinning
			@brief	ソフトウェアスキニング
		*/
		void SoftwareSkinning();

		/*!
			@fn		ShaderSkinning
			@brief	シェーダー側でのスキニング
		*/
		void ShaderSkinning();

		/*!
			@fn		CreateIndexBuffer
			@brief	インデックスバッファの作成
			@return	S_OK:成功 E_FAIL:失敗
		*/
		HRESULT CreateIndexBuffer();

		/*!
			@fn		CreateVertexBuffer
			@brief	頂点バッファの作成
			@return	S_OK:成功 E_FAIL:失敗
		*/
		HRESULT CreateVertexBuffer();

		/*!
			@var	frameIndex
			@brief	アニメーションのフレーム
		*/
		uint32_t frameIndex;
	};
}