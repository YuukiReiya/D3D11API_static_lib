/*
	@file	RenderQueue.h
	@date	2019/04/08
	@author	番場 宥輝
	@brief	描画オブジェクトのレンダリング順の制御
	@detail	シングルトン
*/
#pragma once
#include "Singleton.h"
#include <vector>
#include "AbstractRender.h"

/*! Direct3D11関連 */
namespace D3D11 {

	/*! 描画関連の名前空間に含める */
	namespace Graphic {

		/*!
			@class	RenderQueue
			@brief	描画オブジェクトのレンダリング順を制御するためのコンテナクラス
			@detail	シングルトン
		*/
		class RenderQueue
			:public Singleton<RenderQueue>
		{
		public:
			/*!
				@brief	デストラクタ
			*/
			~RenderQueue();

			/*!
				@fn		Render
				@brief	描画オブジェクトの一括描画
				@detail	積まれた描画オブジェクトのDraw()関数を一括呼び出し
			*/
			void Render();


			/*!
				@enum	Priority
				@brief	優先度
			*/
			enum Priority
			{
				/*!
					@var	Opacity
					@brief	不透明
				*/
				Opacity,

				/*!
					@var	High
					@brief	優先度:高
				*/
				High,

				/*!
					@var	Low
					@brief	優先度:低
				*/
				Low,
			};

			/*!
				@fn			Add
				@brief		描画コンテナへの追加
				@param[in]	描画オブジェクト
				@param[in]	描画の優先度
			*/
			void Add(API::AbstractRender* render,Priority priority = Priority::Low);

			/*!
				@fn		Clear
				@brief	描画コンテナのクリア
			*/
			void Clear();
		private:
			/*!
				@brief	コンストラクタ
			*/
			RenderQueue();

			/*!
				@fn			Loop
				@brief		描画ループ
				@param[in]	描画オブジェクトのコンテナ
			*/
			void Loop(std::vector<API::AbstractRender*>&renders);

			/*!
				@brief	シングルトンデザインパターンのテンプレート継承
			*/
			friend class Singleton<RenderQueue>;

			/*!
				@var	m_pOpacityRenders
				@brief	不透明な描画オブジェクトを入れるコンテナ
			*/
			std::vector<API::AbstractRender*>m_pOpacityRenders;

			/*!
				@var	m_pHighRenders
				@brief	優先度の高い描画オブジェクトを入れるコンテナ
			*/
			std::vector<API::AbstractRender*>m_pHighRenders;

			/*!
				@var	m_pLowRenders
				@brief	優先度の低い描画オブジェクトを入れるコンテナ
			*/
			std::vector<API::AbstractRender*>m_pLowRenders;
		};
	}
}
