#include "stdafx.h"
#include "RenderQueue.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@fn		Render
	@brief	描画オブジェクトの一括描画
	@detail	積まれた描画オブジェクトのDraw()関数を一括呼び出し
*/
void D3D11::Graphic::RenderQueue::Render()
{
	//	優先度:低
	Loop(m_pLowRenders);

	//	優先度:高
	Loop(m_pHighRenders);

	//	不透明描画オブジェクトの描画
	Loop(m_pOpacityRenders);
}

/*!
	@fn			Add
	@brief		描画コンテナへの追加
	@param[in]	描画オブジェクト
	@param[in]	描画の優先度
*/
void D3D11::Graphic::RenderQueue::Add(API::AbstractRender* render, Priority priority)
{
	switch (priority)
	{
		//	不透明
		case D3D11::Graphic::RenderQueue::Opacity:
			m_pOpacityRenders.push_back(render);
		break;

		//	優先度:高
		case D3D11::Graphic::RenderQueue::High:
			m_pHighRenders.push_back(render);
		break;

		//	優先度:低
		case D3D11::Graphic::RenderQueue::Low:
			m_pLowRenders.push_back(render);
		break;

		//	例外
		default:
			std::string error = "RenderQueueのコンテナに追加することが出来ませんでした。";
			ErrorLog(error);
		break;
	}
}

/*!
	@fn		Clear
	@brief	描画コンテナのクリア
*/
void D3D11::Graphic::RenderQueue::Clear()
{
	m_pOpacityRenders.clear();
	m_pLowRenders.clear();
	m_pHighRenders.clear();
}

/*!
	@fn			Loop
	@brief		描画ループ
	@param[in]	描画オブジェクトのコンテナ
*/
void D3D11::Graphic::RenderQueue::Loop(std::vector<API::AbstractRender*>& renders)
{
	//	コンテナ参照用のイテレータ
	auto it = renders.rbegin();

	//	ループ
	while (true)
	{
		//	コンテナは空？
		if (renders.empty())		{ break; }

		//	最後の要素？
		if (renders.rend() == it)	{ break; }

		//	要素参照
		const auto render = *it;

		//	処理
		render->Draw();

		//	イテレータ更新
		it++;
	}
}
