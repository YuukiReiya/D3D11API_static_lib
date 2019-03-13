/*
	@file	IRender.h
	@date	2019/03/13
	@author	番場 宥輝
	@brief	描画の抽象クラス
	@detail	スプライトやモデルなどの描画オブジェクトの基底クラス
*/
#pragma once

/*!
	@namespace	API
	@brief		API名前空間に含める
*/
namespace API
{
	class IRender abstract 
	{
	public :
		/*!
			@brief	コンストラクタ
		*/
		IRender();

		/*!
			@brief	デストラクタ
		*/
		~IRender();

		/*!
			@fn		Render
			@brief	仮想描画関数
			@detail	純粋仮想関数として実装
		*/
		virtual void Render()abstract;
	};
}