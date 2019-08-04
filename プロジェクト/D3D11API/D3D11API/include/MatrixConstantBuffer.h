/*!
	@file	MatrixConstantBuffer.h
	@date	2019/05/18
	@author	番場 宥輝
	@brief	シェーダー(GPU)側に送る行列(w,v,p)の構造体
	@detail	継承するよりバッファ分けて送るか、メンバに持たせて使う
*/
#pragma once
#include <DirectXMath.h>

namespace D3D11 {
	namespace Graphic {

		/*!
			@struct	MatrixConstantBuffer
			@brief	行列情報をまとめたコンスタントバッファ用の構造体
			@detail	定数バッファのため16バイト境界にアライメントしておく。
		*/
		struct alignas(16) MatrixConstantBuffer
		{
			/*!
				@var	world
				@brief	ワールド行列
			*/
			DirectX::XMMATRIX world;

			/*!
				@var	view
				@brief	ビュー行列
			*/
			DirectX::XMMATRIX view;

			/*!
				@var	proj
				@brief	プロジェクション行列
			*/
			DirectX::XMMATRIX proj;
		};
	}
}