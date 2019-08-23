/*
	@file	AnimationClip.h
	@date	2019/08/14
	@author	番場 宥輝
	@brief	アニメーションのフレーム姿勢情報格納用の構造体(行列パレット)
*/
#pragma once
#include <vector>
#include <DirectXMath.h>
#include <memory>

namespace API
{
	/*!
		@using	MatrixPalette
		@brief	別名定義
	*/
	using MatrixPalette = std::vector<DirectX::XMMATRIX>;

	/*!
		@struct	AnimationClip
		@brief	スキンアニメーションのフレーム姿勢情報
	*/
	struct AnimationClip
	{
		/*!
			@var	matrixPalette
			@brief	フレーム姿勢の行列パレット
			@detail	matrixPalette[ジョイント番号][フレーム]
		*/
		std::vector<MatrixPalette>matrixPalette;

		/*!
			@fn			Load
			@brief		アニメーションデータの読み込み
			@param[in]	バインドするアニメーションクリップ
			@param[in]	ファイルパス
			@return		true:成功 false:失敗
		*/
		static bool Load(AnimationClip*animClip, std::string filePath);

		/*!
			@fn			Load
			@brief		アニメーションデータの読み込み
			@param[in]	ファイルパス
			@return		true:成功 false:失敗
		*/
		inline bool Load(std::string filePath) { return Load(this, filePath); }
	};
}